#include "set-operations.hpp"
#include "descriptor.hpp"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <queue>
#include <sstream>

#if !defined(XTL_TEST_ALL_NON_LEAFS)
#define XTL_TEST_ALL_NON_LEAFS 1
#endif

//------------------------------------------------------------------------------

extern int yyparse(void);
extern int yy_flex_debug;
int yydebug;
class_map all_classes;
size_t max_classes_per_unit  = 700;
size_t number_of_arguments   = 1;
size_t max_clauses_per_match = 128;

//------------------------------------------------------------------------------

class_set all()
{
    class_set classes;

    for (class_map::const_iterator p = all_classes.begin(); p != all_classes.end(); ++p)
        classes.insert(p->second);

    return classes;
}

//------------------------------------------------------------------------------

class_set all(bool (ClassDescriptor::*pred)() const)
{
    class_set classes;

    for (class_map::const_iterator p = all_classes.begin(); p != all_classes.end(); ++p)
        if ((p->second->*pred)())
            classes.insert(p->second);

    return classes;
}

//------------------------------------------------------------------------------

inline class_set all_roots() { return all(&ClassDescriptor::is_root); }
inline class_set all_leafs() { return all(&ClassDescriptor::is_leaf); }
inline class_set all_utils() { return all(&ClassDescriptor::is_util); }

//------------------------------------------------------------------------------

class_set all_derived_from(const ClassDescriptor* b)
{
    class_set result;

    for (class_map::const_iterator p = all_classes.begin(); p != all_classes.end(); ++p)
        if (p->second->is_derived_from(b))
            result.insert(p->second);

    return result;
}

//------------------------------------------------------------------------------

class_set some_derived_from(const ClassDescriptor* b, size_t n)
{
    class_set result;

    std::queue<const ClassDescriptor*> work;

    work.push(b);

    while (!work.empty())
    {
        const ClassDescriptor* c = work.front();
        work.pop();
        result.insert(c);

        if (result.size() >= n)
            break;

        for (class_set::const_iterator p = c->derived.begin(); p != c->derived.end(); ++p)
            work.push(*p);
    }

    return result;
}

//------------------------------------------------------------------------------

path_set derivation_paths_from_to(const ClassDescriptor* r, class_set& derived)
{
    path_set paths;

    for (class_set::const_iterator p = derived.begin(); p != derived.end(); ++p)
    {
        path_set derived_paths = (*p)->derivation_paths_to(r);
        paths.insert(derived_paths.begin(), derived_paths.end());
    }

    assert(paths.size() >= derived.size());
    return paths;
}

//------------------------------------------------------------------------------

inline path_set derivation_paths_from(const ClassDescriptor* r)
{
    class_set derived = all_derived_from(r);
    return derivation_paths_from_to(r, derived);
}

//------------------------------------------------------------------------------

size_t count(bool (ClassDescriptor::*pred)() const, const class_set& classes)
{
    size_t result = 0;

    for (class_set::const_iterator q = classes.begin(); q != classes.end(); ++q)
        if (((*q)->*pred)())
            ++result;

    return result;
}

//------------------------------------------------------------------------------

class_set bases(const ClassDescriptor* cd)
{
    class_set result;

    for (ClassDescriptor::bases_type::const_iterator p = cd->bases.begin(); p != cd->bases.end(); ++p)
        result.insert(*p);

    return result;
}

//------------------------------------------------------------------------------

class_set complete(class_set classes)
{
    class_set result = classes;

    for (class_set::const_iterator q = classes.begin(); q != classes.end(); ++q)
        if (!(*q)->bases.empty())
            result = result | complete(bases(*q));

    return result;
}

//------------------------------------------------------------------------------

template <typename Iterator>
void print_class_names(Iterator b, Iterator e, std::ostream& os = std::cout)
{
    for (Iterator q = b; q != e; ++q)
        os << (q == b ? "" : ", ") << (*q)->name;
}

//------------------------------------------------------------------------------

void print_classes(class_set classes, std::ostream& os = std::cout)
{
    while (!classes.empty())
    {
        for (class_set::const_iterator q = classes.begin(); q != classes.end(); ++q)
        {
            bool can_be_printed = true;
            const ClassDescriptor& cd = **q;

            for (ClassDescriptor::bases_type::const_iterator p = cd.bases.begin(); p != cd.bases.end(); ++p)
                if (!(can_be_printed = classes.find(*p) == classes.end()))
                    break;

            if (can_be_printed)
            {
                os << **q << std::endl;
                classes.erase(*q);
                break;
            }
        }
    }
}

//------------------------------------------------------------------------------

size_t print_make_root(const ClassDescriptor* r, std::ostream& os = std::cout)
{
    class_set derived = all_derived_from(r);

    if (derived.size() < 2)
        return 0; // Ignore hierarchies with one class only

    path_set paths = derivation_paths_from_to(r, derived);

    os << "// There are " << derived.size() << " classes derived via " << paths.size() << " pathes from c_" << r->name << std::endl;
    os << "c_" << r->name << "* make_" << r->name << "(int i)\n{\n    switch (i % " << paths.size() << ")\n    {\n";

    size_t i = 0;

    for (path_set::const_iterator p = paths.begin(); p != paths.end(); ++p)
    {
        os << "        case " << i++ << ": return ";

        if (p->size() > 1)
        {
            class_seq::const_reverse_iterator qb = p->rbegin(), qe = p->rend();
            ++qb; // Skip last conversion to root to have it implicit
            --qe; // Skip first conversion to itself
                
            for (class_seq::const_reverse_iterator q = qb; q != qe; ++q)
                os << "(c_" << (*q)->name << "*)";
        }

        os << " new c_" << (*p->begin())->name << ';' << std::endl;
    }

    os << "    }\n    return 0;\n}\n\n";
    return i;
}

//------------------------------------------------------------------------------

void print_make_roots(std::ostream& os = std::cout)
{
    class_set roots = all_roots();

    while (!roots.empty())
    {
        const ClassDescriptor* r = *roots.begin();
        roots.erase(roots.begin());
        print_make_root(r,os);
    }
}

//------------------------------------------------------------------------------

size_t print_match_root_n(size_t n, const ClassDescriptor* r, std::ostream& os = std::cout)
{
    size_t m = (size_t)std::pow(double(max_clauses_per_match),1.0/double(n));
    class_set derived = some_derived_from(r, m); //all_derived_from(r);

    if (derived.size() < 2)
        return 0; // Ignore hierarchies with one class only

    class_seq top_sorted(derived.begin(),derived.end());

    std::sort(top_sorted.begin(), top_sorted.end(), cd_ptr_less);

    os << "int match_" << r->name /*<< '_' << n*/ << '(';
    for (size_t i = 0; i < n; ++i) os << (i ? ", c_" : "c_") << r->name << "* s" << (i+1);
    os << ")\n{\n    Match(";
    for (size_t i = 0; i < n; ++i) os << (i ? ",s" : "s") << (i+1);
    os << ")\n    {\n";

    std::vector<size_t> x(n+1);
    size_t i = 0, k = 0, t = top_sorted.size();

    do
    {
        os << "    Case(";
        for (size_t j = 0; j < n; ++j) os << (j ? ",c_" : "c_") << top_sorted[x[n-j-1]]->name;
        os << ") return " << i++ << ";\n";

        while (x[k] == t-1)
            x[k++] = 0;

        ++x[k];
        k = 0;
    } while (x[n]==0);

    os << "    }\n    EndMatch // There was" << std::setw(5) << i << " case clauses\n    return -1;\n}\n\n";
    return i;
}

//------------------------------------------------------------------------------

size_t print_match_root(const ClassDescriptor* r, std::ostream& os = std::cout)
{
    return print_match_root_n(number_of_arguments, r, os);
    //class_set derived = all_derived_from(r);

    //if (derived.size() < 2)
    //    return 0; // Ignore hierarchies with one class only

    //class_seq top_sorted(derived.begin(),derived.end());

    //std::sort(top_sorted.begin(), top_sorted.end(), cd_ptr_less);

    //os << "int match_" << r->name << "(c_" << r->name << "* s)\n{\n    Match(s)\n    {\n";

    //size_t i = 0;

    //for (class_seq::const_iterator p = top_sorted.begin(); p != top_sorted.end(); ++p)
    //    os << "    Case(c_" << (*p)->name << ") return " << i++ << ";\n";

    //os << "    }\n    EndMatch\n    return -1;\n}\n\n";
    //return i;
}

//------------------------------------------------------------------------------

void print_match_roots(std::ostream& os = std::cout)
{
    class_set roots = all_roots();

    while (!roots.empty())
    {
        const ClassDescriptor* r = *roots.begin();
        roots.erase(roots.begin());
        print_match_root(r,os);
    }
}

//------------------------------------------------------------------------------

void  open_translation_unit(
    size_t             n, 
    const std::string& base_name, 
    std::ostream&      hpp, 
    std::fstream&      cpp, 
    std::stringstream& fnc
)
{
    std::stringstream ss;
    ss << base_name << std::setfill('0') << std::setw(2) << n << ".cpp";
    cpp.open(ss.str(), std::ios::out);
    cpp << "#include \"" + base_name + ".hpp\"\n\n";
    fnc.str("");
    ss.str("");
    ss  << "void test" << std::setfill('0') << std::setw(2) << n << "()";
    hpp << "extern " << ss.str() << ";\n";
    fnc << ss.str() << "\n{\n";
}

//------------------------------------------------------------------------------

void close_translation_unit(
    std::fstream&      cpp, 
    std::stringstream& fnc
)
{
    fnc << "}\n";
    cpp << fnc.str() << std::endl;
    cpp.close();
}

//------------------------------------------------------------------------------

void print_preprocessor_directives(
    std::ostream&    hpp,
    const class_set& classes
)
{
    size_t all   = classes.size();
    size_t roots = 0;
    size_t leafs = 0;
    size_t utils = 0;
    size_t depth = 0;
    size_t paths = 0;
    size_t p_min =-1;
    size_t p_avg = 0;
    size_t p_max = 0;
    size_t c_min =-1;
    size_t c_avg = 0;
    size_t c_max = 0;

    for (class_set::const_iterator q = classes.begin(); q != classes.end(); ++q)
    {
        const ClassDescriptor* cd = *q;

        if (cd->is_root()) ++roots;
        if (cd->is_leaf()) ++leafs;
        if (cd->is_util()) ++utils;

        if (!cd->is_root())
        {
            size_t n = cd->bases.size();
            p_avg += n;
            if (p_min > n) p_min = n;
            if (p_max < n) p_max = n;
        }

        if (!cd->is_leaf())
        {
            size_t n = cd->derived.size();
            c_avg += n;
            if (c_min > n) c_min = n;
            if (c_max < n) c_max = n;
        }

        size_t d = cd->depth();

        if (d > depth)
            depth = d;

        if (cd->is_root())
            paths += derivation_paths_from(cd).size();
    }

    hpp << "// There are " << std::setw(4) << all   << " classes in this benchmark of which: \n"
        << "//           " << std::setw(4) << roots << " are roots (have no base classes)\n"
        << "//           " << std::setw(4) << leafs << " are leafs (have no derived classes)\n"
        << "//           " << std::setw(4) << utils << " are utils (have neither base nor derived classes)\n"
        << "//   Height: " << std::setw(4) << depth << " (max depth of a class with root having depth 1)\n"
        << "//    Paths: " << std::setw(4) << paths << " (derivation paths from all the roots)\n"
        << "//  Parents: " << std::setw(4) << p_min << " -- " << std::setw(7) << std::setprecision(5) << double(p_avg)/(all-roots) << " -- " << std::setw(4) << p_max << " (min/avg/max) based on " << std::setw(4) << all-roots << " non roots\n"
        << "// Children: " << std::setw(4) << c_min << " -- " << std::setw(7) << std::setprecision(5) << double(c_avg)/(all-leafs) << " -- " << std::setw(4) << c_max << " (min/avg/max) based on " << std::setw(4) << all-leafs << " non leafs\n"
           "\n"
           "#include \"testutils.hpp\"\n"
           "#include \"type_switchN.hpp\"\n\n"
           "#if !defined(XTL_VIRTUAL)\n"
           "#define XTL_VIRTUAL\n"
           "#endif // XTL_VIRTUAL\n\n";
}

//------------------------------------------------------------------------------

void print_multiple_units(const std::string& base_name)
{
    std::map<const ClassDescriptor*,size_t> num_derived;
    std::map<size_t,size_t>                 histogram;
    class_set all_classes = all();
    class_set hpp_classes;

    for (class_set::const_iterator p = all_classes.begin(); p != all_classes.end(); ++p)
    {
        const ClassDescriptor* cd = *p;
        class_set derived = all_derived_from(cd);
        size_t    nd = num_derived[cd] = derived.size();
        ++histogram[nd];

        if (nd > max_classes_per_unit)
            hpp_classes.insert(cd);
    }

    hpp_classes = complete(hpp_classes);
    class_set cpp_classes = all_classes ^ hpp_classes;

    std::fstream hpp(base_name+".hpp",std::ios::out);
    print_preprocessor_directives(hpp, all_classes);

    hpp << std::endl;
    hpp << "// Num classes in hierarchy -> Num hierarchies with that number" << std::endl;

    for (std::map<size_t,size_t>::const_iterator p = histogram.begin(); p != histogram.end(); ++p)
        hpp << "//" << std::setw(5) << p->first << " -> " << p->second << std::endl;

    hpp << std::endl;
    print_classes(hpp_classes,hpp);

    int n = 1;
    int m = max_classes_per_unit;
    class_set printed = hpp_classes;
    std::stringstream fnc;
    std::fstream      cpp;

    open_translation_unit(n, base_name, hpp, cpp, fnc);

    while (!cpp_classes.empty())
    {
        const ClassDescriptor* root = 0;

        for (class_set::const_iterator q = cpp_classes.begin(); q != cpp_classes.end(); ++q)
        {
            bool all_bases_are_printed = true;
            const ClassDescriptor& cd = **q;

            for (ClassDescriptor::bases_type::const_iterator p = cd.bases.begin(); p != cd.bases.end(); ++p)
                if (!(all_bases_are_printed = cpp_classes.find(*p) == cpp_classes.end()))
                    break;

            if (all_bases_are_printed && (complete(all_derived_from(root)) ^ printed).size() <= max_classes_per_unit)
            {
                root = &cd;
                break;
            }
        }

        assert(root);
        assert(num_derived[root] < max_classes_per_unit);

        if (num_derived[root] > m)
        {
            close_translation_unit(cpp, fnc);
            open_translation_unit(++n, base_name, hpp, cpp, fnc);
            printed = hpp_classes;
            m = max_classes_per_unit;
        }

        class_set derived = complete(all_derived_from(root)) ^ printed;
        print_classes(derived,cpp);
        cpp_classes = cpp_classes ^ derived;
        printed = printed | derived;
        m -= derived.size();

#if XTL_TEST_ALL_NON_LEAFS // This should print make and match for all non-leafs instead of just roots
        for (class_set::const_iterator q = derived.begin(); q != derived.end(); ++q)
        if (const ClassDescriptor* root = *q)
        if ((all_derived_from(root) ^ printed).empty())
#endif
        if (!root->is_leaf())
        {
            size_t m1 = print_make_root(root,cpp);
            size_t m2 = print_match_root(root,cpp);
            //hpp << std::endl << "struct c_" << root->name << ";";
            //hpp << std::endl << "extern c_" << root->name << "*  make_" << root->name << "(int i); // has " << std::setw(2) << m1 << " cases for ";
            //print_class_names(root->bases.begin(), root->bases.end(), hpp);
            //hpp << std::endl << "extern int match_" << root->name << "(c_" << root->name << "* s); // has " << std::setw(2) << m2 << " cases for ";
            //print_class_names(root->bases.begin(), root->bases.end(), hpp);
            //hpp << std::endl;
            fnc << "    std::cout << mch::test(&make_" << root->name << ",&match_" << root->name << ") << std::endl;\n";
        }
    }

    close_translation_unit(cpp, fnc);

    cpp.open(base_name+".cpp", std::ios::out);
    cpp << "#include \"" + base_name + ".hpp\"\n\n";
    cpp << "int main()\n{\n";

    for (int i = 1; i <= n; ++i)
        cpp << "    test" << std::setfill('0') << std::setw(2) << i << "();\n";

    cpp << "    mch::print_xtl_macros();\n}\n";
    cpp.close();
}

//------------------------------------------------------------------------------

void print_single_unit(const std::string& base_name)
{
    class_set all_classes = all();
    class_set roots       = all_roots();
    std::fstream cpp(base_name+".cpp",std::ios::out);
    print_preprocessor_directives(cpp, all_classes);
    print_classes(all_classes, cpp);
    print_make_roots(cpp);
    print_match_roots(cpp);
    cpp << "int main()\n{\n";

    while (!roots.empty())
    {
        const ClassDescriptor* r = *roots.begin();
        roots.erase(roots.begin());

        if (r->is_leaf())
            continue; // Ignore hierarchies with one class only

        cpp << "    std::cout << mch::test(&make_" << r->name << ",&match_" << r->name << ") << std::endl;\n";
    }
    
    cpp << "    mch::print_xtl_macros();\n}\n";
}

//------------------------------------------------------------------------------

void print_dot(std::ostream& os = std::cout)
{
    os << "digraph COU {\nrankdir=BT;\nratio=0.44;\n";

    class_set roots = all_roots();

    for (class_map::const_iterator p = all_classes.begin(); p != all_classes.end(); ++p)
    {
        const ClassDescriptor& cd = *p->second;

        for (ClassDescriptor::bases_type::const_iterator q = cd.bases.begin(); q != cd.bases.end(); ++q)
            os << cd.name << " -> " << (*q)->name << ";" << std::endl;
    }

    os << "}\n";
}

//------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    yydebug = 0;       // disable Yacc debugging mode
    yy_flex_debug = 0; // disable Flex debugging mode
    std::string base_name = "undefined";

    try
    {
        if (argc < 2)
        {
            std::cerr << "Usage: " << argv[0] << " filename.cou [max_classes_per_unit [number_of_arguments]]" << std::endl;
            return 1;
        }

        if (!freopen(argv[1],"r",stdin)) //redirect standard input
        {
            std::cerr << "Error: Can't open file " << argv[1] << std::endl;
            return 2;
        }

        bool max_classes_per_unit_specified = argc > 2;

        if (max_classes_per_unit_specified)
        {
            std::stringstream ss(argv[2]);

            if (!(ss >> max_classes_per_unit))
            {
                std::cerr << "Error: max_classes_per_unit (" << argv[2] << ") has to be integer" << std::endl;
                return 3;
            }
        }

        if (argc > 3)
        {
            std::stringstream ss(argv[3]);

            if (!(ss >> number_of_arguments))
            {
                std::cerr << "Error: number_of_arguments (" << argv[3] << ") has to be integer" << std::endl;
                return 3;
            }
        }

        base_name = argv[1];
        base_name.erase(base_name.find(".cou"));

        int result = yyparse();
        std::clog.flush();

        for (class_map::const_iterator p = all_classes.begin(); p != all_classes.end(); ++p)
            p->second->remove_inaccessible_bases();

        //print_dot();

        if (max_classes_per_unit_specified)
            print_multiple_units(base_name);
        else
            print_single_unit(base_name);

        return result;
    }
    catch (...)
    {
        std::cerr << "ERROR: Unhandled exception caught while parsing" << std::endl;
    	return 4;
    }
}

//------------------------------------------------------------------------------
