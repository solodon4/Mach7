///
/// \file constructor.hpp
///
/// This file defines constructor/type pattern support by our library.
///
/// \autor Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of the XTL framework (http://parasol.tamu.edu/xtl/).
/// Copyright (C) 2005-2012 Texas A&M University.
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
template <typename T, size_t layout, typename E1, typename Condition = void>
struct constr1
{
    static_assert(is_pattern<E1>::value, "Argument E1 of constructor-pattern must be a pattern");

    explicit constr1(const E1& e1) : m_e1(e1) {}
    explicit constr1(E1&& e1) noexcept : m_e1(std::move(e1)) {}
    constr1(constr1&& src) noexcept : m_e1(std::move(src.m_e1)) {}

    /// Helper function that does the actual structural matching once we have
    /// uncovered a value of the target type. Applies to a const argument!
    const T* match_structure(const T* t) const
    {
        XTL_ASSERT(t); // This helper function assumes t cannot be a nullptr
        return apply_expression(m_e1, t, bindings<T,layout>::member0()) // error C2027: use of undefined type 'bindings<type_being_matched,layout>'
             ? t                                                             // here means you did not provide bindings for type_being_matched and layout
             : 0;                                                            // described in the details of error message. See @bindings and @CM
                                                                             // error: incomplete type 'bindings<type_being_matched, layout>' used in nested name specifier (see above description for Visual C++)
    }
    /// Helper function that does the actual structural matching once we have
    /// uncovered a value of the target type. Applies to a non-const argument!
          T* match_structure(      T* t) const
    {
        XTL_ASSERT(t); // This helper function assumes t cannot be a nullptr
        return apply_expression(m_e1, t, bindings<T,layout>::member0()) // error C2027: use of undefined type 'bindings<type_being_matched,layout>'
             ? t                                                             // here means you did not provide bindings for type_being_matched and layout
             : 0;                                                            // described in the details of error message. See @bindings and @CM
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

    E1 m_e1; ///< Expression representing 1st operand
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
template <typename T, size_t layout, typename E1>
struct constr1<T,layout,E1,typename std::enable_if<std::is_same<T,typename E1::result_type>::value>::type>
{
    static_assert(is_pattern<E1>::value, "Argument E1 of constructor-pattern must be a pattern");

    explicit constr1(const E1& e1) : m_e1(e1) {}
    explicit constr1(E1&& e1) noexcept : m_e1(std::move(e1)) {}
    constr1(constr1&& src) noexcept : m_e1(std::move(src.m_e1)) {}

    /// Helper function that does the actual structural matching once we have
    /// uncovered a value of the target type. Applies to a const argument!
    const T* match_structure(const T* t) const
    {
        XTL_ASSERT(t); // This helper function assumes t cannot be a nullptr
        return m_e1(*t) ? t : 0;
    }
    /// Helper function that does the actual structural matching once we have
    /// uncovered a value of the target type. Applies to a non-const argument!
          T* match_structure(      T* t) const
    {
        XTL_ASSERT(t); // This helper function assumes t cannot be a nullptr
        return m_e1(*t) ? t : 0;
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

    E1 m_e1; ///< Expression representing 1st operand
};

//------------------------------------------------------------------------------

/// Constructor/Type pattern of 2 arguments
template <typename T, size_t layout, typename E1, typename E2>
struct constr2
{
    static_assert(is_pattern<E1>::value, "Argument E1 of constructor-pattern must be a pattern");
    static_assert(is_pattern<E2>::value, "Argument E2 of constructor-pattern must be a pattern");

    constr2(const E1& e1, const E2& e2) : m_e1(e1), m_e2(e2) {}
    constr2(E1&& e1, E2&& e2) noexcept : m_e1(std::move(e1)), m_e2(std::move(e2)) {}
    constr2(constr2&& src) noexcept : m_e1(std::move(src.m_e1)), m_e2(std::move(src.m_e2)) {}

    /// Helper function that does the actual structural matching once we have
    /// uncovered a value of the target type. Applies to a const argument!
    const T* match_structure(const T* t) const
    {
        XTL_ASSERT(t); // This helper function assumes t cannot be a nullptr
        return apply_expression(m_e1, t, bindings<T,layout>::member0()) // error C2027: use of undefined type 'bindings<type_being_matched,layout>'
            && apply_expression(m_e2, t, bindings<T,layout>::member1()) // here means you did not provide bindings for type_being_matched and layout
             ? t                                                             // described in the details of error message. See @bindings and @CM
             : 0;                                                            // error: incomplete type 'bindings<type_being_matched, layout>' used in nested name specifier (see above description for Visual C++)
    }
    /// Helper function that does the actual structural matching once we have
    /// uncovered a value of the target type. Applies to a non-const argument!
          T* match_structure(      T* t) const
    {
        XTL_ASSERT(t); // This helper function assumes t cannot be a nullptr
        return apply_expression(m_e1, t, bindings<T,layout>::member0()) // error C2027: use of undefined type 'bindings<type_being_matched,layout>'
            && apply_expression(m_e2, t, bindings<T,layout>::member1()) // here means you did not provide bindings for type_being_matched and layout
             ? t                                                             // described in the details of error message. See @bindings and @CM
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

    E1 m_e1; ///< Expression representing 1st operand
    E2 m_e2; ///< Expression representing 2nd operand
};

//------------------------------------------------------------------------------

/// Constructor/Type pattern of 3 arguments
template <typename T, size_t layout, typename E1, typename E2, typename E3>
struct constr3
{
    static_assert(is_pattern<E1>::value, "Argument E1 of constructor-pattern must be a pattern");
    static_assert(is_pattern<E2>::value, "Argument E2 of constructor-pattern must be a pattern");
    static_assert(is_pattern<E3>::value, "Argument E3 of constructor-pattern must be a pattern");

    constr3(const E1& e1, const E2& e2, const E3& e3) : m_e1(e1), m_e2(e2), m_e3(e3) {}
    constr3(E1&& e1, E2&& e2, E3&& e3) noexcept : m_e1(std::move(e1)), m_e2(std::move(e2)), m_e3(std::move(e3)) {}
    constr3(constr3&& src) noexcept : m_e1(std::move(src.m_e1)), m_e2(std::move(src.m_e2)), m_e3(std::move(src.m_e3)) {}

    /// Helper function that does the actual structural matching once we have
    /// uncovered a value of the target type. Applies to a const argument!
    const T* match_structure(const T* t) const
    {
        XTL_ASSERT(t); // This helper function assumes t cannot be a nullptr
        return apply_expression(m_e1, t, bindings<T,layout>::member0()) // error C2027: use of undefined type 'bindings<type_being_matched,layout>'
            && apply_expression(m_e2, t, bindings<T,layout>::member1()) // here means you did not provide bindings for type_being_matched and layout
            && apply_expression(m_e3, t, bindings<T,layout>::member2()) // described in the details of error message. See @bindings and @CM
             ? t                                                             // error: incomplete type 'bindings<type_being_matched, layout>' used in nested name specifier (see above description for Visual C++)
             : 0;
    }
    /// Helper function that does the actual structural matching once we have
    /// uncovered a value of the target type. Applies to a non-const argument!
          T* match_structure(      T* t) const
    {
        XTL_ASSERT(t); // This helper function assumes t cannot be a nullptr
        return apply_expression(m_e1, t, bindings<T,layout>::member0()) // error C2027: use of undefined type 'bindings<type_being_matched,layout>'
            && apply_expression(m_e2, t, bindings<T,layout>::member1()) // here means you did not provide bindings for type_being_matched and layout
            && apply_expression(m_e3, t, bindings<T,layout>::member2()) // described in the details of error message. See @bindings and @CM
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

    E1 m_e1; ///< Expression representing 1st operand
    E2 m_e2; ///< Expression representing 2nd operand
    E3 m_e3; ///< Expression representing 3rd operand
};

//------------------------------------------------------------------------------

/// Constructor/Type pattern of 4 arguments
template <typename T, size_t layout, typename E1, typename E2, typename E3, typename E4>
struct constr4
{
    static_assert(is_pattern<E1>::value, "Argument E1 of constructor-pattern must be a pattern");
    static_assert(is_pattern<E2>::value, "Argument E2 of constructor-pattern must be a pattern");
    static_assert(is_pattern<E3>::value, "Argument E3 of constructor-pattern must be a pattern");
    static_assert(is_pattern<E4>::value, "Argument E4 of constructor-pattern must be a pattern");

    constr4(const E1& e1, const E2& e2, const E3& e3, const E4& e4) : m_e1(e1), m_e2(e2), m_e3(e3), m_e4(e4) {}
    constr4(E1&& e1, E2&& e2, E3&& e3, E4&& e4) noexcept : m_e1(std::move(e1)), m_e2(std::move(e2)), m_e3(std::move(e3)), m_e4(std::move(e4)) {}
    constr4(constr4&& src) noexcept : m_e1(std::move(src.m_e1)), m_e2(std::move(src.m_e2)), m_e3(std::move(src.m_e3)), m_e4(std::move(src.m_e4)) {}

    /// Helper function that does the actual structural matching once we have
    /// uncovered a value of the target type. Applies to a const argument!
    const T* match_structure(const T* t) const
    {
        XTL_ASSERT(t); // This helper function assumes t cannot be a nullptr
        return apply_expression(m_e1, t, bindings<T,layout>::member0()) // error C2027: use of undefined type 'bindings<type_being_matched,layout>'
            && apply_expression(m_e2, t, bindings<T,layout>::member1()) // here means you did not provide bindings for type_being_matched and layout
            && apply_expression(m_e3, t, bindings<T,layout>::member2()) // described in the details of error message. See @bindings and @CM
            && apply_expression(m_e4, t, bindings<T,layout>::member3()) // error: incomplete type 'bindings<type_being_matched, layout>' used in nested name specifier (see above description for Visual C++)
             ? t
             : 0;
    }
    /// Helper function that does the actual structural matching once we have
    /// uncovered a value of the target type. Applies to a non-const argument!
          T* match_structure(      T* t) const
    {
        XTL_ASSERT(t); // This helper function assumes t cannot be a nullptr
        return apply_expression(m_e1, t, bindings<T,layout>::member0()) // error C2027: use of undefined type 'bindings<type_being_matched,layout>'
            && apply_expression(m_e2, t, bindings<T,layout>::member1()) // here means you did not provide bindings for type_being_matched and layout
            && apply_expression(m_e3, t, bindings<T,layout>::member2()) // described in the details of error message. See @bindings and @CM
            && apply_expression(m_e4, t, bindings<T,layout>::member3()) // error: incomplete type 'bindings<type_being_matched, layout>' used in nested name specifier (see above description for Visual C++)
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

    E1 m_e1; ///< Expression representing 1st operand
    E2 m_e2; ///< Expression representing 2nd operand
    E3 m_e3; ///< Expression representing 3rd operand
    E4 m_e4; ///< Expression representing 4th operand
};

//------------------------------------------------------------------------------

/// 0-argument version of a helper function to @match that accepts arguments that
/// have been already preprocessed with @filter to convert regular variables into
/// @var_ref and constants into @value.
/// \note This version will be called from @match with a non-@view target type
template <typename T, size_t layout>
inline constr0<T,layout> cons_ex(const view<T,layout>&) noexcept
{
    return constr0<T,layout>();
}

/// 0-argument version of a helper function to @match that accepts arguments that
/// have been already preprocessed with @filter to convert regular variables into
/// @var_ref and constants into @value.
/// \note This version will be called from @match that had its target type a @view
template <typename T, size_t layout>
inline constr0<T,layout> cons_ex(const view<view<T,layout>>&) noexcept
{
    return constr0<T,layout>();
}

/// A 0-argument version of a tree-pattern constructor. Target type is allowed
/// to be a @view here.
template <typename T>
inline auto cons() noexcept -> XTL_RETURN(cons_ex(view<T>()))

/// A 0-argument version of a tree-pattern constructor that takes layout in
/// addition to the target type.
/// \note @view is not supposed to be passed as a target type to this version
///       of the function because we will then have two potentially conflicting
///       layouts. Any layout different from @default_layout passed here will
///       result in a compile time error.
template <typename T, size_t layout>
inline auto cons() noexcept -> XTL_RETURN(cons_ex(view<T,layout>()))

//------------------------------------------------------------------------------

/// 1-argument version of a helper function to @match that accepts arguments that
/// have been already preprocessed with @filter to convert regular variables into
/// @var_ref and constants into @value.
/// \note This version will be called from @match with a non-@view target type
template <typename T, size_t layout, typename E1>
inline constr1<T,layout,E1> cons_ex(const view<T,layout>&, E1&& e1) noexcept
{
    return constr1<T,layout,E1>(std::forward<E1>(e1));
}

/// 1-argument version of a helper function to @match that accepts arguments that
/// have been already preprocessed with @filter to convert regular variables into
/// @var_ref and constants into @value.
/// \note This version will be called from @match that had its target type a @view
template <typename T, size_t layout, typename E1>
inline constr1<T,layout,E1> cons_ex(const view<view<T,layout>>&, E1&& e1) noexcept
{
    return constr1<T,layout,E1>(std::forward<E1>(e1));
}

/// A 1-argument version of a tree-pattern constructor. Target type is allowed
/// to be a @view here.
template <typename T, typename E1>
inline auto cons(E1&& e1) noexcept -> XTL_RETURN
(
    cons_ex(
        view<T>(),
        filter(std::forward<E1>(e1))
    )
)

/// A 1-argument version of a tree-pattern constructor that takes layout in
/// addition to the target type.
/// \note @view is not supposed to be passed as a target type to this version
///       of the function because we will then have two potentially conflicting
///       layouts. Any layout different from @default_layout passed here will
///       result in a compile time error.
template <typename T, size_t layout, typename E1>
inline auto cons(E1&& e1) noexcept -> XTL_RETURN
(
    cons_ex(
        view<T,layout>(),
        filter(std::forward<E1>(e1))
    )
)

//------------------------------------------------------------------------------

/// 2-argument version of a helper function to @match that accepts arguments that
/// have been already preprocessed with @filter to convert regular variables into
/// @var_ref and constants into @value.
/// \note This version will be called from @match with a non-@view target type
template <typename T, size_t layout, typename E1, typename E2>
inline constr2<T,layout,E1,E2> cons_ex(const view<T,layout>&, E1&& e1, E2&& e2) noexcept
{
    return constr2<T,layout,E1,E2>(
            std::forward<E1>(e1),
            std::forward<E2>(e2)
           );
}

/// 2-argument version of a helper function to @match that accepts arguments that
/// have been already preprocessed with @filter to convert regular variables into
/// @var_ref and constants into @value.
/// \note This version will be called from @match that had its target type a @view
template <typename T, size_t layout, typename E1, typename E2>
inline constr2<T,layout,E1,E2> cons_ex(const view<view<T,layout>>&, E1&& e1, E2&& e2) noexcept
{
    return constr2<T,layout,E1,E2>(
            std::forward<E1>(e1),
            std::forward<E2>(e2)
           );
}

/// A 2-argument version of a tree-pattern constructor. Target type is allowed
/// to be a @view here.
template <typename T, typename E1, typename E2>
inline auto cons(E1&& e1, E2&& e2) noexcept -> XTL_RETURN
(
    cons_ex(
        view<T>(),
        filter(std::forward<E1>(e1)),
        filter(std::forward<E2>(e2))
    )
)

/// A 2-argument version of a tree-pattern constructor that takes layout in
/// addition to the target type.
/// \note @view is not supposed to be passed as a target type to this version
///       of the function because we will then have two potentially conflicting
///       layouts. Any layout different from @default_layout passed here will
///       result in a compile time error.
template <typename T, size_t layout, typename E1, typename E2>
inline auto cons(E1&& e1, E2&& e2) noexcept -> XTL_RETURN
(
    cons_ex(
        view<T,layout>(),
        filter(std::forward<E1>(e1)),
        filter(std::forward<E2>(e2))
    )
)

//------------------------------------------------------------------------------

/// 3-argument version of a helper function to @match that accepts arguments that
/// have been already preprocessed with @filter to convert regular variables into
/// @var_ref and constants into @value.
/// \note This version will be called from @match with a non-@view target type
template <typename T, size_t layout, typename E1, typename E2, typename E3>
inline constr3<T,layout,E1,E2,E3> cons_ex(const view<T,layout>&, E1&& e1, E2&& e2, E3&& e3) noexcept
{
    return constr3<T,layout,E1,E2,E3>(
            std::forward<E1>(e1),
            std::forward<E2>(e2),
            std::forward<E3>(e3)
           );
}

/// 3-argument version of a helper function to @match that accepts arguments that
/// have been already preprocessed with @filter to convert regular variables into
/// @var_ref and constants into @value.
/// \note This version will be called from @match that had its target type a @view
template <typename T, size_t layout, typename E1, typename E2, typename E3>
inline constr3<T,layout,E1,E2,E3> cons_ex(const view<view<T,layout>>&, E1&& e1, E2&& e2, E3&& e3) noexcept
{
    return constr3<T,layout,E1,E2,E3>(
            std::forward<E1>(e1),
            std::forward<E2>(e2),
            std::forward<E3>(e3)
           );
}

/// A 3-argument version of a tree-pattern constructor. Target type is allowed
/// to be a @view here.
template <typename T, typename E1, typename E2, typename E3>
inline auto cons(E1&& e1, E2&& e2, E3&& e3) noexcept -> XTL_RETURN
(
    cons_ex(
        view<T>(),
        filter(std::forward<E1>(e1)),
        filter(std::forward<E2>(e2)),
        filter(std::forward<E3>(e3))
    )
)

/// A 3-argument version of a tree-pattern constructor that takes layout in
/// addition to the target type.
/// \note @view is not supposed to be passed as a target type to this version
///       of the function because we will then have two potentially conflicting
///       layouts. Any layout different from @default_layout passed here will
///       result in a compile time error.
template <typename T, size_t layout, typename E1, typename E2, typename E3>
inline auto cons(E1&& e1, E2&& e2, E3&& e3) noexcept -> XTL_RETURN
(
    cons_ex(
        view<T,layout>(),
        filter(std::forward<E1>(e1)),
        filter(std::forward<E2>(e2)),
        filter(std::forward<E3>(e3))
    )
)

//------------------------------------------------------------------------------

/// 4-argument version of a helper function to @match that accepts arguments that
/// have been already preprocessed with @filter to convert regular variables into
/// @var_ref and constants into @value.
/// \note This version will be called from @match with a non-@view target type
template <typename T, size_t layout, typename E1, typename E2, typename E3, typename E4>
inline constr4<T,layout,E1,E2,E3,E4> cons_ex(const view<T,layout>&, E1&& e1, E2&& e2, E3&& e3, E4&& e4) noexcept
{
    return constr4<T,layout,E1,E2,E3,E4>(
            std::forward<E1>(e1),
            std::forward<E2>(e2),
            std::forward<E3>(e3),
            std::forward<E4>(e4)
           );
}

/// 4-argument version of a helper function to @match that accepts arguments that
/// have been already preprocessed with @filter to convert regular variables into
/// @var_ref and constants into @value.
/// \note This version will be called from @match that had its target type a @view
template <typename T, size_t layout, typename E1, typename E2, typename E3, typename E4>
inline constr4<T,layout,E1,E2,E3,E4> cons_ex(const view<view<T,layout>>&, E1&& e1, E2&& e2, E3&& e3, E4&& e4) noexcept
{
    return constr4<T,layout,E1,E2,E3,E4>(
            std::forward<E1>(e1),
            std::forward<E2>(e2),
            std::forward<E3>(e3),
            std::forward<E4>(e4)
           );
}

/// A 4-argument version of a tree-pattern constructor. Target type is allowed
/// to be a @view here.
template <typename T, typename E1, typename E2, typename E3, typename E4>
inline auto cons(E1&& e1, E2&& e2, E3&& e3, E4&& e4) noexcept -> XTL_RETURN
(
    cons_ex(
        view<T>(),
        filter(std::forward<E1>(e1)),
        filter(std::forward<E2>(e2)),
        filter(std::forward<E3>(e3)),
        filter(std::forward<E4>(e4))
    )
)

/// A 4-argument version of a tree-pattern constructor that takes layout in
/// addition to the target type.
/// \note @view is not supposed to be passed as a target type to this version
///       of the function because we will then have two potentially conflicting
///       layouts. Any layout different from @default_layout passed here will
///       result in a compile time error.
template <typename T, size_t layout, typename E1, typename E2, typename E3, typename E4>
inline auto cons(E1&& e1, E2&& e2, E3&& e3, E4&& e4) noexcept -> XTL_RETURN
(
    cons_ex(
        view<T,layout>(),
        filter(std::forward<E1>(e1)),
        filter(std::forward<E2>(e2)),
        filter(std::forward<E3>(e3)),
        filter(std::forward<E4>(e4))
    )
)

//------------------------------------------------------------------------------

/// @is_pattern_ is a helper meta-predicate capable of distinguishing all our patterns
template <typename T, size_t L>                                                     struct is_pattern_<constr0<T,L>>              { enum { value = true }; };
template <typename T, size_t L, typename E1>                                        struct is_pattern_<constr1<T,L,E1>>           { enum { value = true }; };
template <typename T, size_t L, typename E1, typename E2>                           struct is_pattern_<constr2<T,L,E1,E2>>        { enum { value = true }; };
template <typename T, size_t L, typename E1, typename E2, typename E3>              struct is_pattern_<constr3<T,L,E1,E2,E3>>     { enum { value = true }; };
template <typename T, size_t L, typename E1, typename E2, typename E3, typename E4> struct is_pattern_<constr4<T,L,E1,E2,E3,E4>>  { enum { value = true }; };

//------------------------------------------------------------------------------
