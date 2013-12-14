///
/// \file constructor.hpp
///
/// This file defines constructor/type pattern support by our library.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of Mach7 library (http://parasol.tamu.edu/mach7/).
/// Copyright (C) 2011-2012 Texas A&M University.
/// All rights reserved.
///

#pragma once

#include "bindings.hpp"
#include "primitive.hpp"
#include <cstddef>

//------------------------------------------------------------------------------

/// Constructor/Type pattern of 0 arguments
template <typename T, size_t layout>
struct constr0
{
    typedef T accepted_type; ///< Type accepted by the pattern. Requirement of #Pattern concept

    ///@{
    /// Constructor patterns can be uniformly matched against pointers and 
    /// references to polymorphic objects. This uniformity is needed for 
    /// composability of constructor patterns, since application operators
    /// return a pointer to indicate whether pattern was accepted or refuted.
    /// The following set of application operators distinguishes:
    ///  - the arguments passed by reference from those passed by pointers
    ///  - whether the target type matches the subject type (to avoid dynamic_cast)
    ///  - const from non-const arguments to propagate constness further.
    template <typename U> const T* operator()(const U* u) const noexcept { return dynamic_cast<const T*>(u); }
    template <typename U>       T* operator()(      U* u) const noexcept { return dynamic_cast<      T*>(u); }
    template <typename U> const T* operator()(const U& u) const noexcept { return operator()(&u); }
    template <typename U>       T* operator()(      U& u) const noexcept { return operator()(&u); }
                          const T* operator()(const T* t) const noexcept { return t; }
                                T* operator()(      T* t) const noexcept { return t; }
    ///@}

    //template <typename E> expr_or<constr0,E> operator|(const E& e) const noexcept { return expr_or<constr0,E>(*this,e); }
};

//------------------------------------------------------------------------------

/// Constructor/Type pattern of 1 arguments
template <typename T, size_t layout, typename P1, typename Condition = void>
struct constr1
{
    static_assert(is_pattern<P1>::value, "Argument P1 of constructor-pattern must be a pattern");

    typedef T accepted_type; ///< Type accepted by the pattern. Requirement of #Pattern concept

    explicit constr1(const P1& p1) : m_p1(p1) {}
    explicit constr1(P1&& p1) noexcept : m_p1(std::move(p1)) {}
    constr1(constr1&& src) noexcept : m_p1(std::move(src.m_p1)) {}

    /// Helper function that does the actual structural matching once we have
    /// uncovered a value of the target type. Applies to a const argument!
    const T* match_structure(const T* t) const
    {
        XTL_ASSERT(t); // This helper function assumes t cannot be a nullptr
        return apply_expression(m_p1, t, bindings<T,layout>::member0()) // error C2027: use of undefined type 'bindings<type_being_matched,layout>'
             ? t                                                             // here means you did not provide bindings for type_being_matched and layout
             : 0;                                                            // described in the details of error message. See #bindings and #CM
                                                                             // error: incomplete type 'bindings<type_being_matched, layout>' used in nested name specifier (see above description for Visual C++)
    }
    /// Helper function that does the actual structural matching once we have
    /// uncovered a value of the target type. Applies to a non-const argument!
          T* match_structure(      T* t) const
    {
        XTL_ASSERT(t); // This helper function assumes t cannot be a nullptr
        return apply_expression(m_p1, t, bindings<T,layout>::member0()) // error C2027: use of undefined type 'bindings<type_being_matched,layout>'
             ? t                                                             // here means you did not provide bindings for type_being_matched and layout
             : 0;                                                            // described in the details of error message. See #bindings and #CM
                                                                             // error: incomplete type 'bindings<type_being_matched, layout>' used in nested name specifier (see above description for Visual C++)
    }

    ///@{
    /// Constructor patterns can be uniformly matched against pointers and 
    /// references to polymorphic objects. This uniformity is needed for 
    /// composability of constructor patterns, since application operators
    /// return a pointer to indicate whether pattern was accepted or refuted.
    /// The following set of application operators distinguishes:
    ///  - the arguments passed by reference from those passed by pointers
    ///  - whether the target type matches the subject type (to avoid dynamic_cast)
    ///  - const from non-const arguments to propagate constness further.
    template <typename U> const T* operator()(const U* u) const { return operator()(dynamic_cast<const T*>(u)); }
    template <typename U>       T* operator()(      U* u) const { return operator()(dynamic_cast<      T*>(u)); }
    template <typename U> const T* operator()(const U& u) const { return operator()(&u); }
    template <typename U>       T* operator()(      U& u) const { return operator()(&u); }
                          const T* operator()(const T* t) const { return t ? match_structure(t) : 0; }
                                T* operator()(      T* t) const { return t ? match_structure(t) : 0; }
    ///@}

    //template <typename E> expr_or<constr1,E> operator|(const E& e) const noexcept { return expr_or<constr1,E>(*this,e); }

    P1 m_p1; ///< Expression representing 1st operand
};

/// This is a specialization of the general one-argument construction pattern
/// for the case when the type of that argument matches the target type. This
/// is needed to be able to apply algebraic patterns to the entire object for
/// those cases when object already provides its structural decomposition.
/// For example std::complex<T> can be decomposed into two subcomponents and
/// accordingly to our logic when second argument is omitted, we should only 
/// match against the first one. Now if that first argument is of type 
/// complex<T> and not just T, we are trying to match against the whole object,
/// not against its first subcomponent! One can imagine a recursive data 
/// structure however where the type of the first subcomponent also matches
/// the actual type (e.g. list), so we must give preference in such case to
/// the structurally matched subcomponent instead of matching against the whole
/// object. 
/// FIX: Add extra condition that makes sure the type of first subcomponent is
///      not the same as the type itself!
template <typename T, size_t layout, typename P1>
struct constr1<T,layout,P1,typename std::enable_if<std::is_same<T,typename P1::accepted_type>::value>::type>
{
    static_assert(is_pattern<P1>::value, "Argument P1 of constructor-pattern must be a pattern");

    typedef T accepted_type; ///< Type accepted by the pattern. Requirement of #Pattern concept

    explicit constr1(const P1& p1) : m_p1(p1) {}
    explicit constr1(P1&& p1) noexcept : m_p1(std::move(p1)) {}
    constr1(constr1&& src) noexcept : m_p1(std::move(src.m_p1)) {}

    /// Helper function that does the actual structural matching once we have
    /// uncovered a value of the target type. Applies to a const argument!
    const T* match_structure(const T* t) const
    {
        XTL_ASSERT(t); // This helper function assumes t cannot be a nullptr
        return m_p1(*t) ? t : 0;
    }
    /// Helper function that does the actual structural matching once we have
    /// uncovered a value of the target type. Applies to a non-const argument!
          T* match_structure(      T* t) const
    {
        XTL_ASSERT(t); // This helper function assumes t cannot be a nullptr
        return m_p1(*t) ? t : 0;
    }

    ///@{
    /// Constructor patterns can be uniformly matched against pointers and 
    /// references to polymorphic objects. This uniformity is needed for 
    /// composability of constructor patterns, since application operators
    /// return a pointer to indicate whether pattern was accepted or refuted.
    /// The following set of application operators distinguishes:
    ///  - the arguments passed by reference from those passed by pointers
    ///  - whether the target type matches the subject type (to avoid dynamic_cast)
    ///  - const from non-const arguments to propagate constness further.
    template <typename U> const T* operator()(const U* u) const { return operator()(dynamic_cast<const T*>(u)); }
    template <typename U>       T* operator()(      U* u) const { return operator()(dynamic_cast<      T*>(u)); }
    template <typename U> const T* operator()(const U& u) const { return operator()(&u); }
    template <typename U>       T* operator()(      U& u) const { return operator()(&u); }
                          const T* operator()(const T* t) const { return t ? match_structure(t) : 0; }
                                T* operator()(      T* t) const { return t ? match_structure(t) : 0; }
    ///@}

    //template <typename E> expr_or<constr1,E> operator|(const E& e) const noexcept { return expr_or<constr1,E>(*this,e); }

    P1 m_p1; ///< Expression representing 1st operand
};

//------------------------------------------------------------------------------

/// Constructor/Type pattern of 2 arguments
template <typename T, size_t layout, typename P1, typename P2>
struct constr2
{
    static_assert(is_pattern<P1>::value, "Argument P1 of constructor-pattern must be a pattern");
    static_assert(is_pattern<P2>::value, "Argument P2 of constructor-pattern must be a pattern");

    typedef T accepted_type; ///< Type accepted by the pattern. Requirement of #Pattern concept

    constr2(const P1& p1, const P2& p2) : m_p1(p1), m_p2(p2) {}
    constr2(P1&& p1, P2&& p2) noexcept : m_p1(std::move(p1)), m_p2(std::move(p2)) {}
    constr2(constr2&& src) noexcept : m_p1(std::move(src.m_p1)), m_p2(std::move(src.m_p2)) {}

    /// Helper function that does the actual structural matching once we have
    /// uncovered a value of the target type. Applies to a const argument!
    const T* match_structure(const T* t) const
    {
        XTL_ASSERT(t); // This helper function assumes t cannot be a nullptr
        return apply_expression(m_p1, t, bindings<T,layout>::member0()) // error C2027: use of undefined type 'bindings<type_being_matched,layout>'
            && apply_expression(m_p2, t, bindings<T,layout>::member1()) // here means you did not provide bindings for type_being_matched and layout
             ? t                                                             // described in the details of error message. See #bindings and #CM
             : 0;                                                            // error: incomplete type 'bindings<type_being_matched, layout>' used in nested name specifier (see above description for Visual C++)
    }
    /// Helper function that does the actual structural matching once we have
    /// uncovered a value of the target type. Applies to a non-const argument!
          T* match_structure(      T* t) const
    {
        XTL_ASSERT(t); // This helper function assumes t cannot be a nullptr
        return apply_expression(m_p1, t, bindings<T,layout>::member0()) // error C2027: use of undefined type 'bindings<type_being_matched,layout>'
            && apply_expression(m_p2, t, bindings<T,layout>::member1()) // here means you did not provide bindings for type_being_matched and layout
             ? t                                                             // described in the details of error message. See #bindings and #CM
             : 0;                                                            // error: incomplete type 'bindings<type_being_matched, layout>' used in nested name specifier (see above description for Visual C++)
    }

    ///@{
    /// Constructor patterns can be uniformly matched against pointers and 
    /// references to polymorphic objects. This uniformity is needed for 
    /// composability of constructor patterns, since application operators
    /// return a pointer to indicate whether pattern was accepted or refuted.
    /// The following set of application operators distinguishes:
    ///  - the arguments passed by reference from those passed by pointers
    ///  - whether the target type matches the subject type (to avoid dynamic_cast)
    ///  - const from non-const arguments to propagate constness further.
    template <typename U> const T* operator()(const U* u) const { return operator()(dynamic_cast<const T*>(u)); }
    template <typename U>       T* operator()(      U* u) const { return operator()(dynamic_cast<      T*>(u)); }
    template <typename U> const T* operator()(const U& u) const { return operator()(&u); }
    template <typename U>       T* operator()(      U& u) const { return operator()(&u); }
                          const T* operator()(const T* t) const { return t ? match_structure(t) : 0; }
                                T* operator()(      T* t) const { return t ? match_structure(t) : 0; }
    ///@}

    //template <typename E> expr_or<constr2,E> operator|(const E& e) const noexcept { return expr_or<constr2,E>(*this,e); }

    P1 m_p1; ///< Expression representing 1st operand
    P2 m_p2; ///< Expression representing 2nd operand
};

//------------------------------------------------------------------------------

/// Constructor/Type pattern of 3 arguments
template <typename T, size_t layout, typename P1, typename P2, typename P3>
struct constr3
{
    static_assert(is_pattern<P1>::value, "Argument P1 of constructor-pattern must be a pattern");
    static_assert(is_pattern<P2>::value, "Argument P2 of constructor-pattern must be a pattern");
    static_assert(is_pattern<P3>::value, "Argument P3 of constructor-pattern must be a pattern");

    typedef T accepted_type; ///< Type accepted by the pattern. Requirement of #Pattern concept

    constr3(const P1& p1, const P2& p2, const P3& p3) : m_p1(p1), m_p2(p2), m_p3(p3) {}
    constr3(P1&& p1, P2&& p2, P3&& p3) noexcept : m_p1(std::move(p1)), m_p2(std::move(p2)), m_p3(std::move(p3)) {}
    constr3(constr3&& src) noexcept : m_p1(std::move(src.m_p1)), m_p2(std::move(src.m_p2)), m_p3(std::move(src.m_p3)) {}

    /// Helper function that does the actual structural matching once we have
    /// uncovered a value of the target type. Applies to a const argument!
    const T* match_structure(const T* t) const
    {
        XTL_ASSERT(t); // This helper function assumes t cannot be a nullptr
        return apply_expression(m_p1, t, bindings<T,layout>::member0()) // error C2027: use of undefined type 'bindings<type_being_matched,layout>'
            && apply_expression(m_p2, t, bindings<T,layout>::member1()) // here means you did not provide bindings for type_being_matched and layout
            && apply_expression(m_p3, t, bindings<T,layout>::member2()) // described in the details of error message. See #bindings and #CM
             ? t                                                             // error: incomplete type 'bindings<type_being_matched, layout>' used in nested name specifier (see above description for Visual C++)
             : 0;
    }
    /// Helper function that does the actual structural matching once we have
    /// uncovered a value of the target type. Applies to a non-const argument!
          T* match_structure(      T* t) const
    {
        XTL_ASSERT(t); // This helper function assumes t cannot be a nullptr
        return apply_expression(m_p1, t, bindings<T,layout>::member0()) // error C2027: use of undefined type 'bindings<type_being_matched,layout>'
            && apply_expression(m_p2, t, bindings<T,layout>::member1()) // here means you did not provide bindings for type_being_matched and layout
            && apply_expression(m_p3, t, bindings<T,layout>::member2()) // described in the details of error message. See #bindings and #CM
             ? t                                                             // error: incomplete type 'bindings<type_being_matched, layout>' used in nested name specifier (see above description for Visual C++)
             : 0;
    }

    ///@{
    /// Constructor patterns can be uniformly matched against pointers and 
    /// references to polymorphic objects. This uniformity is needed for 
    /// composability of constructor patterns, since application operators
    /// return a pointer to indicate whether pattern was accepted or refuted.
    /// The following set of application operators distinguishes:
    ///  - the arguments passed by reference from those passed by pointers
    ///  - whether the target type matches the subject type (to avoid dynamic_cast)
    ///  - const from non-const arguments to propagate constness further.
    template <typename U> const T* operator()(const U* u) const { return operator()(dynamic_cast<const T*>(u)); }
    template <typename U>       T* operator()(      U* u) const { return operator()(dynamic_cast<      T*>(u)); }
    template <typename U> const T* operator()(const U& u) const { return operator()(&u); }
    template <typename U>       T* operator()(      U& u) const { return operator()(&u); }
                          const T* operator()(const T* t) const { return t ? match_structure(t) : 0; }
                                T* operator()(      T* t) const { return t ? match_structure(t) : 0; }
    ///@}

    //template <typename E> expr_or<constr3,E> operator|(const E& e) const noexcept { return expr_or<constr3,E>(*this,e); }

    P1 m_p1; ///< Expression representing 1st operand
    P2 m_p2; ///< Expression representing 2nd operand
    P3 m_p3; ///< Expression representing 3rd operand
};

//------------------------------------------------------------------------------

/// Constructor/Type pattern of 4 arguments
template <typename T, size_t layout, typename P1, typename P2, typename P3, typename P4>
struct constr4
{
    static_assert(is_pattern<P1>::value, "Argument P1 of constructor-pattern must be a pattern");
    static_assert(is_pattern<P2>::value, "Argument P2 of constructor-pattern must be a pattern");
    static_assert(is_pattern<P3>::value, "Argument P3 of constructor-pattern must be a pattern");
    static_assert(is_pattern<P4>::value, "Argument P4 of constructor-pattern must be a pattern");

    typedef T accepted_type; ///< Type accepted by the pattern. Requirement of #Pattern concept

    constr4(const P1& p1, const P2& p2, const P3& p3, const P4& p4) : m_p1(p1), m_p2(p2), m_p3(p3), m_p4(p4) {}
    constr4(P1&& p1, P2&& p2, P3&& p3, P4&& p4) noexcept : m_p1(std::move(p1)), m_p2(std::move(p2)), m_p3(std::move(p3)), m_p4(std::move(p4)) {}
    constr4(constr4&& src) noexcept : m_p1(std::move(src.m_p1)), m_p2(std::move(src.m_p2)), m_p3(std::move(src.m_p3)), m_p4(std::move(src.m_p4)) {}

    /// Helper function that does the actual structural matching once we have
    /// uncovered a value of the target type. Applies to a const argument!
    const T* match_structure(const T* t) const
    {
        XTL_ASSERT(t); // This helper function assumes t cannot be a nullptr
        return apply_expression(m_p1, t, bindings<T,layout>::member0()) // error C2027: use of undefined type 'bindings<type_being_matched,layout>'
            && apply_expression(m_p2, t, bindings<T,layout>::member1()) // here means you did not provide bindings for type_being_matched and layout
            && apply_expression(m_p3, t, bindings<T,layout>::member2()) // described in the details of error message. See #bindings and #CM
            && apply_expression(m_p4, t, bindings<T,layout>::member3()) // error: incomplete type 'bindings<type_being_matched, layout>' used in nested name specifier (see above description for Visual C++)
             ? t
             : 0;
    }
    /// Helper function that does the actual structural matching once we have
    /// uncovered a value of the target type. Applies to a non-const argument!
          T* match_structure(      T* t) const
    {
        XTL_ASSERT(t); // This helper function assumes t cannot be a nullptr
        return apply_expression(m_p1, t, bindings<T,layout>::member0()) // error C2027: use of undefined type 'bindings<type_being_matched,layout>'
            && apply_expression(m_p2, t, bindings<T,layout>::member1()) // here means you did not provide bindings for type_being_matched and layout
            && apply_expression(m_p3, t, bindings<T,layout>::member2()) // described in the details of error message. See #bindings and #CM
            && apply_expression(m_p4, t, bindings<T,layout>::member3()) // error: incomplete type 'bindings<type_being_matched, layout>' used in nested name specifier (see above description for Visual C++)
             ? t
             : 0;
    }

    ///@{
    /// Constructor patterns can be uniformly matched against pointers and 
    /// references to polymorphic objects. This uniformity is needed for 
    /// composability of constructor patterns, since application operators
    /// return a pointer to indicate whether pattern was accepted or refuted.
    /// The following set of application operators distinguishes:
    ///  - the arguments passed by reference from those passed by pointers
    ///  - whether the target type matches the subject type (to avoid dynamic_cast)
    ///  - const from non-const arguments to propagate constness further.
    template <typename U> const T* operator()(const U* u) const { return operator()(dynamic_cast<const T*>(u)); }
    template <typename U>       T* operator()(      U* u) const { return operator()(dynamic_cast<      T*>(u)); }
    template <typename U> const T* operator()(const U& u) const { return operator()(&u); }
    template <typename U>       T* operator()(      U& u) const { return operator()(&u); }
                          const T* operator()(const T* t) const { return t ? match_structure(t) : 0; }
                                T* operator()(      T* t) const { return t ? match_structure(t) : 0; }
    ///@}

    //template <typename E> expr_or<constr4,E> operator|(const E& e) const noexcept { return expr_or<constr4,E>(*this,e); }

    P1 m_p1; ///< Expression representing 1st operand
    P2 m_p2; ///< Expression representing 2nd operand
    P3 m_p3; ///< Expression representing 3rd operand
    P4 m_p4; ///< Expression representing 4th operand
};

//------------------------------------------------------------------------------

/// 0-argument version of a helper function to #cons that accepts arguments that
/// have been already preprocessed with #filter to convert regular variables into
/// #var_ref and constants into #value.
/// \note This version will be called from #cons with a non-#view target type
template <typename T, size_t layout>
inline constr0<T,layout> cons_ex(const view<T,layout>&) noexcept
{
    return constr0<T,layout>();
}

/// 0-argument version of a helper function to #cons that accepts arguments that
/// have been already preprocessed with #filter to convert regular variables into
/// #var_ref and constants into #value.
/// \note This version will be called from #cons that had its target type a #view
template <typename T, size_t layout>
inline constr0<T,layout> cons_ex(const view<view<T,layout>>&) noexcept
{
    return constr0<T,layout>();
}

/// A 0-argument version of a tree-pattern constructor. Target type is allowed
/// to be a #view here.
template <typename T>
inline auto cons() noexcept -> XTL_RETURN(cons_ex(view<T>()))

/// A 0-argument version of a tree-pattern constructor that takes layout in
/// addition to the target type.
/// \note #view is not supposed to be passed as a target type to this version
///       of the function because we will then have two potentially conflicting
///       layouts. Any layout different from #default_layout passed here will
///       result in a compile time error.
template <typename T, size_t layout>
inline auto cons() noexcept -> XTL_RETURN(cons_ex(view<T,layout>()))

//------------------------------------------------------------------------------

/// 1-argument version of a helper function to #cons that accepts arguments that
/// have been already preprocessed with #filter to convert regular variables into
/// #var_ref and constants into #value.
/// \note This version will be called from #cons with a non-#view target type
template <typename T, size_t layout, typename P1>
inline constr1<T,layout,P1> cons_ex(const view<T,layout>&, P1&& p1) noexcept
{
    return constr1<T,layout,P1>(std::forward<P1>(p1));
}

/// 1-argument version of a helper function to #cons that accepts arguments that
/// have been already preprocessed with #filter to convert regular variables into
/// #var_ref and constants into #value.
/// \note This version will be called from #cons that had its target type a #view
template <typename T, size_t layout, typename P1>
inline constr1<T,layout,P1> cons_ex(const view<view<T,layout>>&, P1&& p1) noexcept
{
    return constr1<T,layout,P1>(std::forward<P1>(p1));
}

/// A 1-argument version of a tree-pattern constructor. Target type is allowed
/// to be a #view here.
template <typename T, typename P1>
inline auto cons(P1&& p1) noexcept -> XTL_RETURN
(
    cons_ex(
        view<T>(),
        filter(std::forward<P1>(p1))
    )
)

/// A 1-argument version of a tree-pattern constructor that takes layout in
/// addition to the target type.
/// \note #view is not supposed to be passed as a target type to this version
///       of the function because we will then have two potentially conflicting
///       layouts. Any layout different from #default_layout passed here will
///       result in a compile time error.
template <typename T, size_t layout, typename P1>
inline auto cons(P1&& p1) noexcept -> XTL_RETURN
(
    cons_ex(
        view<T,layout>(),
        filter(std::forward<P1>(p1))
    )
)

//------------------------------------------------------------------------------

/// 2-argument version of a helper function to #cons that accepts arguments that
/// have been already preprocessed with #filter to convert regular variables into
/// #var_ref and constants into #value.
/// \note This version will be called from #cons with a non-#view target type
template <typename T, size_t layout, typename P1, typename P2>
inline constr2<T,layout,P1,P2> cons_ex(const view<T,layout>&, P1&& p1, P2&& p2) noexcept
{
    return constr2<T,layout,P1,P2>(
            std::forward<P1>(p1),
            std::forward<P2>(p2)
           );
}

/// 2-argument version of a helper function to #cons that accepts arguments that
/// have been already preprocessed with #filter to convert regular variables into
/// #var_ref and constants into #value.
/// \note This version will be called from #cons that had its target type a #view
template <typename T, size_t layout, typename P1, typename P2>
inline constr2<T,layout,P1,P2> cons_ex(const view<view<T,layout>>&, P1&& p1, P2&& p2) noexcept
{
    return constr2<T,layout,P1,P2>(
            std::forward<P1>(p1),
            std::forward<P2>(p2)
           );
}

/// A 2-argument version of a tree-pattern constructor. Target type is allowed
/// to be a #view here.
template <typename T, typename P1, typename P2>
inline auto cons(P1&& p1, P2&& p2) noexcept -> XTL_RETURN
(
    cons_ex(
        view<T>(),
        filter(std::forward<P1>(p1)),
        filter(std::forward<P2>(p2))
    )
)

/// A 2-argument version of a tree-pattern constructor that takes layout in
/// addition to the target type.
/// \note #view is not supposed to be passed as a target type to this version
///       of the function because we will then have two potentially conflicting
///       layouts. Any layout different from #default_layout passed here will
///       result in a compile time error.
template <typename T, size_t layout, typename P1, typename P2>
inline auto cons(P1&& p1, P2&& p2) noexcept -> XTL_RETURN
(
    cons_ex(
        view<T,layout>(),
        filter(std::forward<P1>(p1)),
        filter(std::forward<P2>(p2))
    )
)

//------------------------------------------------------------------------------

/// 3-argument version of a helper function to #cons that accepts arguments that
/// have been already preprocessed with #filter to convert regular variables into
/// #var_ref and constants into #value.
/// \note This version will be called from #cons with a non-#view target type
template <typename T, size_t layout, typename P1, typename P2, typename P3>
inline constr3<T,layout,P1,P2,P3> cons_ex(const view<T,layout>&, P1&& p1, P2&& p2, P3&& p3) noexcept
{
    return constr3<T,layout,P1,P2,P3>(
            std::forward<P1>(p1),
            std::forward<P2>(p2),
            std::forward<P3>(p3)
           );
}

/// 3-argument version of a helper function to #cons that accepts arguments that
/// have been already preprocessed with #filter to convert regular variables into
/// #var_ref and constants into #value.
/// \note This version will be called from #cons that had its target type a #view
template <typename T, size_t layout, typename P1, typename P2, typename P3>
inline constr3<T,layout,P1,P2,P3> cons_ex(const view<view<T,layout>>&, P1&& p1, P2&& p2, P3&& p3) noexcept
{
    return constr3<T,layout,P1,P2,P3>(
            std::forward<P1>(p1),
            std::forward<P2>(p2),
            std::forward<P3>(p3)
           );
}

/// A 3-argument version of a tree-pattern constructor. Target type is allowed
/// to be a #view here.
template <typename T, typename P1, typename P2, typename P3>
inline auto cons(P1&& p1, P2&& p2, P3&& p3) noexcept -> XTL_RETURN
(
    cons_ex(
        view<T>(),
        filter(std::forward<P1>(p1)),
        filter(std::forward<P2>(p2)),
        filter(std::forward<P3>(p3))
    )
)

/// A 3-argument version of a tree-pattern constructor that takes layout in
/// addition to the target type.
/// \note #view is not supposed to be passed as a target type to this version
///       of the function because we will then have two potentially conflicting
///       layouts. Any layout different from #default_layout passed here will
///       result in a compile time error.
template <typename T, size_t layout, typename P1, typename P2, typename P3>
inline auto cons(P1&& p1, P2&& p2, P3&& p3) noexcept -> XTL_RETURN
(
    cons_ex(
        view<T,layout>(),
        filter(std::forward<P1>(p1)),
        filter(std::forward<P2>(p2)),
        filter(std::forward<P3>(p3))
    )
)

//------------------------------------------------------------------------------

/// 4-argument version of a helper function to #cons that accepts arguments that
/// have been already preprocessed with #filter to convert regular variables into
/// #var_ref and constants into #value.
/// \note This version will be called from #cons with a non-#view target type
template <typename T, size_t layout, typename P1, typename P2, typename P3, typename P4>
inline constr4<T,layout,P1,P2,P3,P4> cons_ex(const view<T,layout>&, P1&& p1, P2&& p2, P3&& p3, P4&& p4) noexcept
{
    return constr4<T,layout,P1,P2,P3,P4>(
            std::forward<P1>(p1),
            std::forward<P2>(p2),
            std::forward<P3>(p3),
            std::forward<P4>(p4)
           );
}

/// 4-argument version of a helper function to #cons that accepts arguments that
/// have been already preprocessed with #filter to convert regular variables into
/// #var_ref and constants into #value.
/// \note This version will be called from #cons that had its target type a #view
template <typename T, size_t layout, typename P1, typename P2, typename P3, typename P4>
inline constr4<T,layout,P1,P2,P3,P4> cons_ex(const view<view<T,layout>>&, P1&& p1, P2&& p2, P3&& p3, P4&& p4) noexcept
{
    return constr4<T,layout,P1,P2,P3,P4>(
            std::forward<P1>(p1),
            std::forward<P2>(p2),
            std::forward<P3>(p3),
            std::forward<P4>(p4)
           );
}

/// A 4-argument version of a tree-pattern constructor. Target type is allowed
/// to be a #view here.
template <typename T, typename P1, typename P2, typename P3, typename P4>
inline auto cons(P1&& p1, P2&& p2, P3&& p3, P4&& p4) noexcept -> XTL_RETURN
(
    cons_ex(
        view<T>(),
        filter(std::forward<P1>(p1)),
        filter(std::forward<P2>(p2)),
        filter(std::forward<P3>(p3)),
        filter(std::forward<P4>(p4))
    )
)

/// A 4-argument version of a tree-pattern constructor that takes layout in
/// addition to the target type.
/// \note #view is not supposed to be passed as a target type to this version
///       of the function because we will then have two potentially conflicting
///       layouts. Any layout different from #default_layout passed here will
///       result in a compile time error.
template <typename T, size_t layout, typename P1, typename P2, typename P3, typename P4>
inline auto cons(P1&& p1, P2&& p2, P3&& p3, P4&& p4) noexcept -> XTL_RETURN
(
    cons_ex(
        view<T,layout>(),
        filter(std::forward<P1>(p1)),
        filter(std::forward<P2>(p2)),
        filter(std::forward<P3>(p3)),
        filter(std::forward<P4>(p4))
    )
)

//------------------------------------------------------------------------------

/// #is_pattern_ is a helper meta-predicate capable of distinguishing all our patterns
template <typename T, size_t L>                                                     struct is_pattern_<constr0<T,L>>              { enum { value = true }; };
template <typename T, size_t L, typename P1>                                        struct is_pattern_<constr1<T,L,P1>>           { enum { value = true }; };
template <typename T, size_t L, typename P1, typename P2>                           struct is_pattern_<constr2<T,L,P1,P2>>        { enum { value = true }; };
template <typename T, size_t L, typename P1, typename P2, typename P3>              struct is_pattern_<constr3<T,L,P1,P2,P3>>     { enum { value = true }; };
template <typename T, size_t L, typename P1, typename P2, typename P3, typename P4> struct is_pattern_<constr4<T,L,P1,P2,P3,P4>>  { enum { value = true }; };

//------------------------------------------------------------------------------
