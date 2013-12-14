#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

//------------------------------------------------------------------------------

struct ClassDescriptor;
typedef std::set<const ClassDescriptor*>       class_set;
typedef std::map<std::string,ClassDescriptor*> class_map;
typedef std::vector<const ClassDescriptor*>    class_seq;
typedef std::set<class_seq>                    path_set;

//------------------------------------------------------------------------------

struct ClassDescriptor
{
    typedef class_seq                bases_type;
    typedef std::vector<std::string> attributes_type;
    typedef std::vector<std::string> methods_type;

    ClassDescriptor(const std::string& n) : name(n), is_defined(false)/*, derived_count(1)*/ {}

    bool operator<(const ClassDescriptor& cd) const 
    { 
        if (this == &cd)
            return false;
        else
            return is_derived_from(&cd) || (!cd.is_derived_from(this) && name < cd.name);
    }

    bool is_root() const { return bases.empty(); }
    bool is_leaf() const { return derived.empty(); }
    bool is_util() const { return is_root() && is_leaf(); }
    size_t depth() const;

    bool is_derived_from(const ClassDescriptor*) const;
    path_set derivation_paths_to(const ClassDescriptor*) const;
    void derivation_paths_to_recursive(const ClassDescriptor* c, class_seq& path, path_set& paths) const;

    void add_base(ClassDescriptor*);
    void add_attr(const std::string&);
    void add_mthd(const std::string&);
private:
    //void add_derived() const;
    void add_derived(const ClassDescriptor*);
public:
    void finalize() { is_defined = true; }
    void remove_inaccessible_bases();

    std::string     name;       ///< Name of the class described
    bases_type      bases;      ///< List of base classes in the order defined
    attributes_type attributes; ///< Data-members (names only)
    methods_type    methods;    ///< Member-functions (names only)
    bool            is_defined; ///< The entire definition in COU file has been seen
//    mutable size_t  derived_count; ///< Number of derived classes, including itself
    class_set       derived;    ///< Set of immediately derived classes
};

//------------------------------------------------------------------------------

extern class_map all_classes;

//------------------------------------------------------------------------------

std::ostream&    operator<<(std::ostream& os, const ClassDescriptor& cd);
std::string      fix_name(std::string);
ClassDescriptor* get_class(const std::string& name);
inline bool      cd_ptr_less(const ClassDescriptor* c1, const ClassDescriptor* c2) { return *c1 < *c2; }

//------------------------------------------------------------------------------
