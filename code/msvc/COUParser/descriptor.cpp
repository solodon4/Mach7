#include "descriptor.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>

//------------------------------------------------------------------------------

ClassDescriptor* get_class(const std::string& name)
{
    std::string n = fix_name(name);
    ClassDescriptor*& result = all_classes[n];

    if (!result)
        result = new ClassDescriptor(n);

    return result;
}

//------------------------------------------------------------------------------

std::string fix_name(std::string s)
{
    for (std::string::iterator p = s.begin(); p != s.end(); ++p)
    {
        if (!(isalpha(*p) || isdigit(*p) || *p == '_'))
            *p = '_';
    }

    return s;
}

//------------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& os, const ClassDescriptor& cd)
{
    os << "// Class c_"    << cd.name 
       << " is at depth "  << cd.depth()
       << " and has "      << cd.derived.size() << " children"
       << std::endl;

    if (!cd.derived.empty())
    {
        os << "// Children: ";

        for (class_set::const_iterator p = cd.derived.begin(); p != cd.derived.end(); ++p)
            os << (p == cd.derived.begin() ? "c_" : ", c_") << (*p)->name;

        os << std::endl;
    }
    
    os << "struct c_" << cd.name;

    if (!cd.bases.empty())
        for (ClassDescriptor::bases_type::const_iterator p = cd.bases.begin(); p != cd.bases.end(); ++p)
            os << (p == cd.bases.begin() ? " : XTL_VIRTUAL c_" : ", XTL_VIRTUAL c_") << (*p)->name;
    
    os << "\n{\n";

    if (cd.is_root())
        os << "    virtual ~c_" << cd.name << "() {};\n";

    for (ClassDescriptor::attributes_type::const_iterator p = cd.attributes.begin(); p != cd.attributes.end(); ++p)
        os << "    int m_" << *p << ";\n";

    os << std::endl;

    for (ClassDescriptor::methods_type::const_iterator p = cd.methods.begin(); p != cd.methods.end(); ++p)
        os << "    virtual int f_" << *p << "() \t{ return " << rand() % 100 << "; }\n";

    return os << "};\n";
}

//------------------------------------------------------------------------------

size_t ClassDescriptor::depth() const
{
    if (is_root()) return 1;

    size_t result = 0;

    for (bases_type::const_iterator p = bases.begin(); p != bases.end(); ++p)
    {
        size_t base_depth = (*p)->depth();

        if (base_depth > result)
            result = base_depth;
    }

    return result+1;
}

//------------------------------------------------------------------------------

bool ClassDescriptor::is_derived_from(const ClassDescriptor* c) const
{
    if (c == this || std::find(bases.begin(), bases.end(), c) != bases.end())
        return true;

    for (bases_type::const_iterator p = bases.begin(); p != bases.end(); ++p)
    {
        if ((*p)->is_derived_from(c))
            return true;
    }

    return false;
}

//------------------------------------------------------------------------------

path_set ClassDescriptor::derivation_paths_to(const ClassDescriptor* c) const
{
    class_seq path;
    path_set paths;

    derivation_paths_to_recursive(c, path, paths);
    return paths;
}

//------------------------------------------------------------------------------

void ClassDescriptor::derivation_paths_to_recursive(const ClassDescriptor* c, class_seq& path, path_set& paths) const
{
    path.push_back(this);

    if (c == this)
        paths.insert(path);

    for (bases_type::const_iterator p = bases.begin(); p != bases.end(); ++p)
        (*p)->derivation_paths_to_recursive(c, path, paths);

    path.pop_back();
}

//------------------------------------------------------------------------------

void ClassDescriptor::add_base(ClassDescriptor* b)
{
    if (is_defined)
    {
        std::cerr << "Warning: Class `c_" << name << "' has already been defined. Ignoring addition of base class " << b->name << std::endl;
        return;
    }

//    b->add_derived();
    b->add_derived(this);
    bases.push_back(b);
}

//------------------------------------------------------------------------------

void ClassDescriptor::add_attr(const std::string& nm)
{
    std::string n = fix_name(nm);

    if (is_defined)
    {
        std::cerr << "Warning: Class `c_" << name << "' has already been defined. Ignoring addition of attribute " << n << std::endl;
        return;
    }

    if (std::find(attributes.begin(), attributes.end(), n) == attributes.end())
        attributes.push_back(n);
    //else
    //    std::cerr << "Warning: Class `c_" << name << "' already has attribute " << n << std::endl;
}

//------------------------------------------------------------------------------

void ClassDescriptor::add_mthd(const std::string& nm)
{
    std::string n = fix_name(nm);

    if (is_defined)
    {
        std::cerr << "Warning: Class `c_" << name << "' has already been defined. Ignoring addition of method " << n << std::endl;
        return;
    }

    if (std::find(methods.begin(), methods.end(), n) == methods.end())
        methods.push_back(n);
    //else
    //    std::cerr << "Warning: Class `c_" << name << "' already has method " << n << std::endl;
}

//------------------------------------------------------------------------------

//void ClassDescriptor::add_derived() const
//{
//    ++derived_count;
//
//    for (bases_type::const_iterator p = bases.begin(); p != bases.end(); ++p)
//        (*p)->add_derived();
//}

//------------------------------------------------------------------------------

void ClassDescriptor::add_derived(const ClassDescriptor* c)
{
    derived.insert(c);
}

//------------------------------------------------------------------------------

void ClassDescriptor::remove_inaccessible_bases()
{
    assert(is_defined); // This can only be called once the class and all its bases are finalized

    for (bases_type::iterator p = bases.begin(); p != bases.end();)
    {
        assert((*p)->is_defined); // This can only be called once the class and all its bases are finalized

        bool is_ambiguous_base = false;

        for (bases_type::const_iterator q = bases.begin(); q != bases.end(); ++q)
            if (*p != *q && (*q)->is_derived_from(*p))
            {
                is_ambiguous_base = true;
                std::cerr << "Warning: base class `c_" << (*p)->name 
                          << "' is an ambiguous (inaccessible) base class of `c_" << name 
                          << "' because `c_" << (*q)->name 
                          << "' already derives from it. Removed from base classes." 
                          << std::endl;
                break;
            }
        
        if (is_ambiguous_base)
            p = bases.erase(p);
        else
            ++p;
    }
}

//------------------------------------------------------------------------------
