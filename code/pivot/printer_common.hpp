/// This is a comon header whose definitions do not depend on whether printer's
/// implementation is visitor or pattern matching based and thus is shared 
/// between the two.

#pragma once

#define eval_classic_ex eval_expr

//==============================================================================
// String manipulation functions
//==============================================================================

#include "iprutils.hpp"          // Helper functions to work with IPR
#include <algorithm>
#include <iomanip>
#include <iterator>
#include <vector>

//------------------------------------------------------------------------------

inline std::vector<std::string> tokenize_with(const std::string& str, const std::string& delimiters)
{
    std::vector<std::string> res;
    tokenize(str, std::back_inserter(res), delimiters);
    return res;
}

//------------------------------------------------------------------------------

inline std::ostream& indent(std::ostream& os, int indent_level = 0, bool empty_line = false)
{
    os << std::endl;

    if (empty_line)
        os << std::endl;

    if (indent_level)
        os << std::setw(indent_level*4) << ' ';

    return os;
}

//------------------------------------------------------------------------------

inline std::string postfix(const std::string& declarator)
{
    return !declarator.empty() && isid(declarator[0]) ? ' '+declarator : declarator;
}

//------------------------------------------------------------------------------

inline std::string prefix(const std::string& declarator)
{
    return !declarator.empty() && isid(declarator[declarator.length()-1]) ? declarator+' ' : declarator;
}

//==============================================================================
// cxx_printer_base
//==============================================================================

#include "printer.hpp"

//------------------------------------------------------------------------------

/// Class that holds context needed to pass around while printing IPR
class cxx_printer_base
{
protected:

    const std::ostream& base() const { return m_os; }
          std::ostream& base()       { return m_os; }

public:

    cxx_printer_base(
        std::ostream&       os, 
        bool                part_of_expression
    ) :
        m_os(os),
        m_part_of_expression(part_of_expression),
        m_indent_level(0),
        m_commenter(0)
    {}

    cxx_printer_base(
        std::ostream&       os,
        int                 indent_level = 0
    ) :
        m_os(os),
        m_part_of_expression(false),
        m_indent_level(indent_level),
        m_commenter(0)
    {}
    
    cxx_printer_base(
        std::ostream&       os,
        comments_interface& c,
        int                 indent_level = 0
    ) :
        m_os(os),
        m_part_of_expression(false),
        m_indent_level(indent_level),
        m_commenter(&c)
    {}

    bool is_part_of_expression() const { return m_part_of_expression; }
    int  indent_level() const { return m_indent_level; }

//protected:

    /// The actual stream we are wrapping
    std::ostream& m_os;

    /// Indicates that corresponding statement is printed as part of expression
    /// and thus trailing ; as well as new-lines and comments should be avoided.
    const bool m_part_of_expression;

    /// A level of indentation currently used.
    int m_indent_level;

    /// A user-supplied object used to generate comments to the source
    comments_interface* m_commenter;

};

//==============================================================================
// cxx_printer_of
//==============================================================================

/// Class that holds context needed to pass around while printing IPR
template <typename Derived>
class cxx_printer_of : public cxx_printer_base
{
public:

    /// Forward all constructors to the base class
    cxx_printer_of(std::ostream& os, bool part_of_expression)                     : cxx_printer_base(os,part_of_expression) {}
    cxx_printer_of(std::ostream& os, int  indent_level = 0)                       : cxx_printer_base(os,indent_level)       {}
    cxx_printer_of(std::ostream& os, comments_interface& c, int indent_level = 0) : cxx_printer_base(os,c,indent_level)     {}

    const Derived& derived() const { return *static_cast<const Derived*>(this); }
          Derived& derived()       { return *static_cast<      Derived*>(this); }

    //cxx_printer_of& operator<<(cxx_printer_of& (*pfn)(cxx_printer_of&)) { base() << pfn; return *this; }
    //cxx_printer_of& operator<<(basic_ios<char_type, traits_type>& (*pfn)(basic_ios<char_type, traits_type>&)) { base() << pfn; return *this; }
    //cxx_printer_of& operator<<(ios_base& (*pfn)(ios_base&)) { base() << pfn; return *this; }

    Derived& operator<<(char c)                 { base() << c;   return derived(); }
    Derived& operator<<(const char* str)        { base() << str; return derived(); }
    Derived& operator<<(const std::string& str) { base() << str; return derived(); }

    //template <typename T> Derived& operator<<(const T& t) { base() << t; return derived(); }

    Derived& operator<<(Derived& (&manipulator)(Derived&)) { return manipulator(derived()); }

    template <typename T> 
    inline Derived& operator<<(const ipr::Sequence<T>& s) 
    {
        for (typename ipr::Sequence<T>::iterator p = s.begin(); p != s.end(); ++p)
            derived() << *p;

        return derived();
    }

    friend Derived&    increase(Derived& cxx) { ++cxx.m_indent_level; return cxx; }
    friend Derived&    decrease(Derived& cxx) { --cxx.m_indent_level; return cxx; }
    friend Derived& indentation(Derived& cxx) { return cxx.indent(); }
    friend Derived&  empty_line(Derived& cxx) { return cxx.indent(true); }
    friend Derived&   semicolon(Derived& cxx) { if (!cxx.is_part_of_expression()) cxx << ';'; return cxx; }

//protected:

    // A syntactic sugar to show the intent that we would like to forward
    // evaluation to a subcomponent.
    template <typename IPRNode>
    void forward_to(const IPRNode& n, bool indent = true)
    {
        if (indent)
            ++m_indent_level;

        derived() << n;

        if (indent)
            --m_indent_level;
    }

    template <class T>
    void sequence_forward_to(const ipr::Sequence<T>& s, bool indent = true)
    {
        for (typename ipr::Sequence<T>::iterator p = s.begin(); p != s.end(); ++p)
            forward_to(*p, indent);
    }

    Derived& indent(bool empty_line = false)
    {
        if (!m_part_of_expression)
        {
            base() << std::endl;

            if (empty_line)
                base() << std::endl;

            if (m_indent_level)
                base() << std::setw(m_indent_level*4) << ' ';
        }

        return derived();
    }

    Derived& beforeF(const ipr::Stmt& s) { return before(s,false); }
    Derived& beforeT(const ipr::Stmt& s) { return before(s,true); }
    Derived& before(const ipr::Stmt& s, bool empty_line = false)
    {
        // FIX: Add support for printing a line that shows which file the source
        //      is coming from till that file becomes different.
        if (m_part_of_expression)
            return derived();

        // Print comments that user generated
        if (m_commenter)
        {
            // Separate into strings line-by-line
            std::vector<std::string> tokens = tokenize_with(m_commenter->before(s), "\n");
            // Get rid of duplicates
            std::vector<std::string>::iterator new_end = std::unique(tokens.begin(), tokens.end());
            tokens.resize(new_end-tokens.begin());
            // Show each line with markers as to which statement it refers to
            for (std::vector<std::string>::const_iterator p = tokens.begin(); p != tokens.end(); ++p)
            {
                indent(empty_line) << "//-> " << *p;
                empty_line = false;
            }
        }

        // Print comments/annotations that were in the original source file
        const ipr::Sequence<ipr::Annotation>& annotations = s.annotation();

        for (ipr::Sequence<ipr::Annotation>::iterator p = annotations.begin(); p != annotations.end(); ++p)
        {
            indent(empty_line) << to_str(p->name());
            empty_line = false;
        }

        return indent(empty_line);
    }

    void after(const ipr::Stmt& s, bool new_line = false)
    {
        if (m_part_of_expression)
            return;

        if (m_commenter)
        {
            // Separate into strings line-by-line
            std::vector<std::string> tokens = tokenize_with(m_commenter->after(s), "\n");
            // Get rid of duplicates
            std::vector<std::string>::iterator new_end = std::unique(tokens.begin(), tokens.end());
            tokens.resize(new_end-tokens.begin());
            // Show each line with markers as to which statement it refers to
            for (std::vector<std::string>::const_iterator p = tokens.begin(); p != tokens.end(); ++p)
            {
                if (new_line)
                    indent();

                base() << (new_line ? "//<- " : " //<- ") << *p;
                new_line = true;
            }
        }
    }

};

//==============================================================================
// IPR helper functions
//==============================================================================

#include "predicates.hpp"        // Helper predicates on IPR nodes

//------------------------------------------------------------------------------

inline std::string prefix_specifiers(const ipr::Decl& s, ipr::Decl::Specifier mask = ipr::Decl::Specifier(-1))
{
    ipr::Decl::Specifier spec = mask & s.specifiers() & ipr::applicable_specifiers(s);
    std::string result;

    if (ipr::is_in_class_declaration(s))
    {
        if (spec & ipr::Decl::Public)    result += "public: ";
        if (spec & ipr::Decl::Protected) result += "protected: ";
        if (spec & ipr::Decl::Private)   result += "private: ";
    }

    if (spec & ipr::Decl::Auto)      result += "auto ";
    if (spec & ipr::Decl::Register)  result += "register ";
    if (spec & ipr::Decl::Static)    result += "static ";
    if (spec & ipr::Decl::Extern)    result += "extern ";
    if (spec & ipr::Decl::Mutable)   result += "mutable ";
    if (spec & ipr::Decl::Inline)    result += "inline ";
    if (spec & ipr::Decl::Virtual)   result += "virtual ";
    if (spec & ipr::Decl::Explicit)  result += "explicit ";
  //if (spec & ipr::Decl::Pure)      result += "pure ";
    if (spec & ipr::Decl::Friend)    result += "friend ";
    if (spec & ipr::Decl::Typedef)   result += "typedef ";
    if (spec & ipr::Decl::Export)    result += "export ";
    if (spec & ipr::Decl::Constexpr) result += "constexpr ";

    return result;
}

//------------------------------------------------------------------------------
