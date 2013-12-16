///
/// \file unisyn.hpp
///
/// This file defines the helper data structures used to unified syntax of the
/// Match statement.
///
/// \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
///
/// This file is a part of Mach7 library (http://parasol.tamu.edu/mach7/).
/// Copyright (C) 2011-2013 Texas A&M University.
/// All rights reserved.
///

#pragma once

#include "has_member.hpp"    // Meta-functions to check use of certain #bindings facilities
#include "patterns/bindings.hpp"
#include "vtblmap.hpp"
#include <unordered_map>
#include <vector>

namespace mch ///< Mach7 library namespace
{

//------------------------------------------------------------------------------

/// The type capable of holding any values of original tags in user's class 
/// hierarchy. We make it different from size_t to ensure we don't mix original 
/// and remapped tags.
enum tag_type { min_tag = size_t(0), max_tag = ~size_t(0) };

/// The type holding remapped original tags into the actual case labels.
/// We make this type distinct from tag_type to make sure we don't pass
/// original tags where remapped are expected and vice versa.
/// We need remapping in order to accomodate a distinct value, indicating 
/// the end of tag precedence list, as well as to make all the case labels
/// close to 0. Remapping mostly happens at compile time, so in very few
/// places we will have to do a run-time conversion.
enum lbl_type { min_lbl = size_t(0), max_lbl = ~size_t(0) };

/// Amount of extra values in the range of labels (lbl_type) we need for our purposes
const size_t reserved_extra_kinds = 1;

#if !defined(SKV)
    /// Macro to be used in global scope to set the smallest kind value N for the 
    /// class hierarchy rooted at C. When it is not used, 0 is assumed.
    #define SKV(C,N) mch::smallest_kind_is<N> smallest_kind_value_helper(const C&)
#else
    #error Macro SKV used by the pattern-matching library has already been defined
#endif

/// Predefined structure that will be used when user did specify smallest kind with #SKV macro
template <size_t N> struct smallest_kind_is { static const size_t value = N; };
/// When the user did not specify the smallest kind with #SKV macro, assume it is 0
smallest_kind_is<0> smallest_kind_value_helper(...);

/// Accessort to get the smallest kind for the hierarchy type T belongs to
template <typename T>
struct smallest_kind
{
    typedef decltype(smallest_kind_value_helper(std::declval<T>())) type;
    static const size_t value = type::value;
};

/// Convenience meta-function to get the original kind associated with the class T
template <typename T, size_t L> struct original                   { static const tag_type tag = tag_type(L); }; // FIX: This temporarily for test purposes assumes only union case. Split in two with enable_if
template <typename T>           struct original<T,default_layout> { static const tag_type tag = tag_type(bindings<T>::kind_value); };
/// Convenience meta-function to get the remapped kind associated with the class T
template <typename T, size_t L = default_layout> struct remapped  { static const lbl_type lbl = lbl_type(original<T,L>::tag - smallest_kind<T>::value + reserved_extra_kinds); };
/// Convenience function to convert remapped kind to original at run-time
template <typename T> inline tag_type remapped2original(lbl_type l) { return tag_type(l + smallest_kind<T>::value - reserved_extra_kinds); }
/// Convenience function to convert original kind to remapped at run-time
template <typename T> inline lbl_type original2remapped(tag_type t) { return lbl_type(t - smallest_kind<T>::value + reserved_extra_kinds); }

//------------------------------------------------------------------------------

typedef std::unordered_map<lbl_type, const lbl_type*> kind_to_kinds_map;

template <typename T>
inline kind_to_kinds_map& get_kind_to_kinds_map() noexcept 
{
    static kind_to_kinds_map k2k;
    return k2k;
}
/*
template <typename T>
struct kind_to_kinds_map_of
{
    static kind_to_kinds_map k2k;
};

template <typename T>
kind_to_kinds_map kind_to_kinds_map_of<T>::k2k;
*/

/// Gets all the kinds of a class with static type T and dynamic type represented 
/// by kind. The first element of the returned list will always be equal to kind,
/// the last to a dedicated value and those in between to the kinds of base classes.
template <typename T>
inline const lbl_type* get_kinds(lbl_type kind) noexcept
{
    return get_kind_to_kinds_map<T>()[kind];
}

template <typename T>
inline const lbl_type* set_kinds(lbl_type kind, const lbl_type* kinds) noexcept
{
    return get_kind_to_kinds_map<T>()[kind] = kinds;
}

template <typename D, typename B>
struct associate_kinds
{
    static_assert(std::is_base_of<B,D>::value, "Not a base class");
    static const lbl_type* kinds;
};

template <typename D, typename B>
const lbl_type* associate_kinds<D,B>::kinds = set_kinds<B>(remapped<D>::lbl, bindings<D>::get_kinds());

/// Checks whether a given base_kind belongs to the tag precedence list of derived_kind
template <typename T>
inline bool is_base_and_derived_kinds(lbl_type base_kind, lbl_type derived_kind) noexcept
{
    const lbl_type* all_kinds = get_kinds<T>(derived_kind);

    if (!all_kinds)
        return base_kind == derived_kind;

    while (*all_kinds)
        if (*all_kinds++ == base_kind)
            return true;

    return false;
}

//------------------------------------------------------------------------------

/// A traits-like class used by pattern matching library to unify the syntax of
/// open and close cases. This is different from defining the XTL_DEFAULT_SYNTAX, 
/// which will make the choice global for every class hierarchy and Match 
/// statement used by the program. With the help of this class, the library will
/// be able to figure out on its own whether we are dealing with open, closed or
/// discriminated union case. The price of such generality is a slight performance
/// overhead that appears because of the necessity of merging the syntactic 
/// structures used by open and closed cases.
/// \note This is the general case that assumes no knowledge and generates simple 
///       cascading-if statement sequentially testing each clause.
template <typename SubjectType, class condition = void>
class unified_switch
{
public:

    /// Type of the argument on which extended switch is done
    typedef typename underlying<SubjectType>::type source_type;

    /// Type of data that has to be statically allocated inside the block 
    /// containg extended switch
    struct static_data_type {};

    /// Type of data that has to be automatically allocated inside the block 
    /// containg extended switch
    struct local_data_type {};

    /// Meta function that defines some case labels required to support extended switch.
    /// The main difference of this function from the one used on case clauses is that 
    /// this one is used on the level of match statement to define the values of common
    /// entry and exit cases.
    template <size_t Counter>
    struct CaseLabel
    {
        enum
        {
            //entry = 0,      ///< Case label that will be used to enter beginning of the switch
            exit  = Counter ///< Case label that will be used to jump to the end of the switch
        };
    };

    /// Function used to get the value we'll be switching on
    static inline size_t choose(const source_type*, static_data_type&, local_data_type&) noexcept
    {
        return 0;
    }

    /// Function that will be called upon first entry to the case through the fall-through behavior
    static inline void on_first_pass(const source_type*, local_data_type&, size_t) noexcept {}
    
    /// Function that will be called when the fall-through behavior reached end of the switch
    static inline void on_end(const source_type*, local_data_type&, size_t) noexcept {}

    /// Function that will be called on default clause. It should return true 
    /// when unconditional jump to ReMatch label should be performed.
    static inline bool on_default(size_t&, local_data_type&, static_data_type&) noexcept { return false; }

    /// Structure used to disambiguate whether first argument is a type or a value
    /// \note Not used for the general case, so we don't specialize it based on argument,
    ///       which will always be type.
    template <bool FirstParamIsValue>
    struct disambiguate
    {
        /// Essentially a catcher of the first argument of the case clause
        /// a type in this case.
        template <typename T>
        struct parameter
        {
            /// The type passed as a first argument of the case clause is the target type.
            typedef typename target_of<T>::type target_type;

            /// Layout that has to be used for the given target type.
            enum { layout = target_of<T>::layout };

            /// Depending on whether we handle open or closed case, different case labels
            /// are used for the generated match statement. This metafunction takes
            /// a unique (per match statement) counter and returns the actual label that
            /// will be used for the case clause.
            template <size_t Counter>
            struct CaseLabel
            {
                enum 
                {
                    value = Counter ///< Case label that will be used for case at line offset Counter
                };
            };

            /// Condition that guards applicability of the given case clause
            /// during the fall-through behavior.
            static inline bool main_condition(const source_type*, local_data_type&) noexcept
            {
                return true;
            }

            /// Performs the necessary conversion of the original subject into the proper
            /// object of target type.
            /// \note The subject is const-qualified, thus the target is also const-qualified
            static inline const target_type* get_matched(const source_type* subject_ptr, local_data_type&) noexcept
            {
                return static_cast<const target_type*>(subject_ptr);
            }

            /// Performs the necessary conversion of the original subject into the proper
            /// object of target type.
            /// \note The subject is non-const, thus the target is also non-const
            static inline       target_type* get_matched(      source_type* subject_ptr, local_data_type& local_data) noexcept
            {
                return static_cast<      target_type*>(subject_ptr);
            }
        };
    };
};

/// A traits-like class used by pattern matching library to unify the syntax of
/// open and close cases. This is different from defining the XTL_DEFAULT_SYNTAX, 
/// which will make the choice global for every class hierarchy and Match 
/// statement used by the program. With the help of this class, the library will
/// be able to figure out on its own whether we are dealing with open, closed or
/// discriminated union case. The price of such generality is a slight performance
/// overhead that appears because of the necessity of merging the syntactic 
/// structures used by open and closed cases.
/// \note This is a specialization for polymorphic case
template <typename SubjectType>
class unified_switch<
    SubjectType, 
    typename std::enable_if<
                std::is_polymorphic<typename underlying<SubjectType>::type>::value &&
               !has_member_kind_selector<bindings<typename underlying<SubjectType>::type>>::value,
                void
             >::type
>
{
public:

    /// Type of the argument on which extended switch is done
    typedef typename underlying<SubjectType>::type source_type;

    /// Type of data that has to be statically allocated inside the block 
    /// containg extended switch
    typedef vtblmap<type_switch_info> static_data_type;

    /// Type of data that has to be automatically allocated inside the block 
    /// containg extended switch
    struct local_data_type
    {
        const void*       casted_ptr;
        type_switch_info* switch_info_ptr;
    };

    /// Meta function that defines some case labels required to support extended switch.
    /// The main difference of this function from the one used on case clauses is that 
    /// this one is used on the level of match statement to define the values of common
    /// entry and exit cases.
    template <size_t Counter>
    struct CaseLabel
    {
        enum
        {
            //entry = 0,      ///< Case label that will be used to enter beginning of the switch
            exit  = Counter ///< Case label that will be used to jump to the end of the switch
        };
    };

    /// Function used to get the value we'll be switching on
    static inline size_t choose(const source_type* subject_ptr, static_data_type& static_data, local_data_type& local_data) noexcept
    {
        local_data.switch_info_ptr = &static_data.get(subject_ptr);
        return local_data.switch_info_ptr->target;
    }

    /// Function that will be called upon first entry to the case through the fall-through behavior
    static inline void on_first_pass(const source_type* subject_ptr, local_data_type& local_data, size_t line) noexcept
    {
        if (XTL_LIKELY(local_data.switch_info_ptr->target == 0)) 
        {
            local_data.switch_info_ptr->target = line; 
            local_data.switch_info_ptr->offset = intptr_t(local_data.casted_ptr)-intptr_t(subject_ptr);
        } 
    }
    
    /// Function that will be called when the fall-through behavior reached end of the switch
    static inline void on_end(const source_type*, local_data_type& local_data, size_t line) noexcept
    {
        XTL_ASSERT(local_data.switch_info_ptr->target == 0);
        //if (XTL_LIKELY(local_data.switch_info_ptr->target == 0)) 
        { 
            local_data.switch_info_ptr->target = line;
        }
    }

    /// Function that will be called on default clause. It should return true 
    /// when unconditional jump to ReMatch label should be performed.
    static inline bool on_default(size_t&, local_data_type&, static_data_type&) noexcept { return false; }

    /// Structure used to disambiguate whether first argument is a type or a value
    /// \note Not used for the open case, so we don't specialize it based on argument,
    ///       which will always be type.
    template <bool FirstParamIsValue>
    struct disambiguate
    {
        static_assert(!FirstParamIsValue, "Open case does not allow values as first argument. Did you forget to use KS macro?");

        /// Essentially a catcher of the first argument of the case clause
        /// a type in this case.
        template <typename T>
        struct parameter
        {
            /// The type passed as a first argument of the case clause is the target type.
            typedef typename target_of<T>::type target_type;

            /// Layout that has to be used for the given target type.
            enum { layout = target_of<T>::layout };

            /// Depending on whether we handle open or closed case, different case labels
            /// are used for the generated match statement. This metafunction takes
            /// a unique (per match statement) counter and returns the actual label that
            /// will be used for the case clause.
            template <size_t Counter>
            struct CaseLabel
            {
                enum 
                {
                    value = Counter ///< Case label that will be used for case at line offset Counter
                };
            };

            /// Condition that guards applicability of the given case clause
            /// during the fall-through behavior.
            static inline bool main_condition(const source_type* subject_ptr, local_data_type& local_data) noexcept
            {
                return (local_data.casted_ptr = dynamic_cast<const target_type*>(subject_ptr)) != 0;
            }

            /// Performs the necessary conversion of the original subject into the proper
            /// object of target type.
            /// \note The subject is const-qualified, thus the target is also const-qualified
            static inline const target_type* get_matched(const source_type* subject_ptr, local_data_type& local_data) noexcept
            {
                return adjust_ptr<target_type>(subject_ptr,local_data.switch_info_ptr->offset);
            }

            /// Performs the necessary conversion of the original subject into the proper
            /// object of target type.
            /// \note The subject is non-const, thus the target is also non-const
            static inline       target_type* get_matched(      source_type* subject_ptr, local_data_type& local_data) noexcept
            {
                return adjust_ptr<target_type>(subject_ptr,local_data.switch_info_ptr->offset);
            }
        };
    };
};

//------------------------------------------------------------------------------

/// A traits-like class used by pattern matching library to unify the syntax of
/// open and close cases. This is different from defining the XTL_DEFAULT_SYNTAX, 
/// which will make the choice global for every class hierarchy and Match 
/// statement used by the program. With the help of this class, the library will
/// be able to figure out on its own whether we are dealing with open, closed or
/// discriminated union case. The price of such generality is a slight performance
/// overhead that appears because of the necessity of merging the syntactic 
/// structures used by open and closed cases.
/// \note This is a specialization for the closed case. It becomes enabled when
///       user used KS macro inside #bindings to define which member will
///       be used as kind selector.
template <typename SubjectType>
class unified_switch<
    SubjectType, 
    typename std::enable_if<
                has_member_kind_selector<bindings<typename underlying<SubjectType>::type>>::value,
                void
             >::type
>
{
public:

    /// Type of the argument on which extended switch is done
    typedef typename underlying<SubjectType>::type source_type;

    /// Type of data that has to be statically allocated inside the block 
    /// containg extended switch
    typedef std::vector<const lbl_type*> static_data_type;

    /// Type of data that has to be automatically allocated inside the block 
    /// containg extended switch
    struct local_data_type
    {
        local_data_type() : kinds(0)/*, attempt(0)*/ {} // NOTE: attempt is not intialized on purpose for performance reasons. It will be initialized before use in on_default.
        const lbl_type* kinds;
        //size_t          attempt;
    };

    enum { match_exit_label = 0 }; // A label value reserved for exiting base class tag iteration loop

    /// Meta function that defines some case labels required to support extended switch.
    /// The main difference of this function from the one used on case clauses is that 
    /// this one is used on the level of match statement to define the values of common
    /// entry and exit cases.
    template <size_t Counter>
    struct CaseLabel
    {
        // We effectively prepend two new case labels to the user's range of 
        // labels to assure that all the labels are close to each other.
        // From our experiments we saw that putting just these two labels 
        // elsewhere was often triggering both gcc and msvc to generate a
        // binary search and then jump table based on subranges, which was
        // killing the performance.
        enum
        {
            exit = match_exit_label ///< Case label that will be used to jump to the end of the switch
        };
    };

    /// Function used to get the value we'll be switching on
    static inline size_t choose(const source_type* subject_ptr, static_data_type&, local_data_type&)
    {
        return original2remapped<source_type>(tag_type(kind_selector(subject_ptr)));
    }

    /// Function that will be called upon first entry to the case through the fall-through behavior
    static inline void on_first_pass(const source_type*, local_data_type&, size_t) noexcept {}

    /// Function that will be called when the fall-through behavior reached end of the switch
    static inline void on_end(const source_type*, local_data_type&, size_t) noexcept {}

    /// Function that will be called on default clause. It should return true 
    /// when unconditional jump to ReMatch label should be performed.
    static inline bool on_default(size_t& jump_target, local_data_type& local_data, static_data_type& static_data) noexcept
    {
        if (XTL_LIKELY(!local_data.kinds))
        {
            /// FIX: The use of vector and resize in it assumes at the moment small tags in 
            ///      region 0..k. Tag randomization will overuse memory!

            //local_data.attempt = 0;
            if (XTL_UNLIKELY(jump_target >= static_data.size()))
                static_data.resize(jump_target+1);
            local_data.kinds = static_data[jump_target];
            if (!local_data.kinds)
                local_data.kinds = static_data[jump_target] = get_kinds<source_type>(lbl_type(jump_target));
        }
        XTL_ASSERT(("Base classes for this kind were not specified",local_data.kinds));
        //XTL_ASSERT(("Invalid list of kinds",local_data.kinds[local_data.attempt]==jump_target));
        //jump_target = local_data.kinds ? local_data.kinds[++local_data.attempt] : lbl_type(match_exit_label);
        XTL_ASSERT(("Invalid list of kinds",*local_data.kinds==jump_target));
        jump_target = local_data.kinds ? *++local_data.kinds : lbl_type(match_exit_label);
        return true;
    }
    /// Structure used to disambiguate whether first argument is a type or a value.
    /// \note This generic one handles a value, which can be seen from the type of 
    ///       parameter struct.
    // C++ standard (14.7.3.2) would not allow us to explicitly specialize 
    // disambiguate later here, but will accept a partial specialization so we
    // add a bogus template parameter to turn explicit specialization into 
    // partial.
    template <bool FirstParamIsValue, typename bogus_parameter = void>
    struct disambiguate
    {
        /// Essentially a catcher of the first argument of the case clause
        /// a value in this case.
        template <size_t N>
        struct parameter
        {
            /// Since value as a first argument of the case clause is only
            /// allowed by us on discriminated unions, the target type is equal
            /// to the source type.
            typedef typename target_of<source_type>::type target_type;

            /// Layout that has to be used for the given target type.
            enum { layout = N };

            /// Depending on whether we handle open or closed case, different case labels
            /// are used for the generated match statement. This metafunction takes
            /// a unique (per match statement) counter and returns the actual label that
            /// will be used for the case clause.
            template <size_t Counter>
            struct CaseLabel
            {
                enum 
                {
                    value = remapped<target_type,N>::lbl ///< Case label that will be used for case at line offset Counter
                };
            };
            
            /// Condition that guards applicability of the given case clause
            /// during the fall-through behavior.
            static inline bool main_condition(const source_type* subject_ptr, local_data_type&) 
            {
                lbl_type derived_kind = original2remapped<source_type>(tag_type(kind_selector(subject_ptr)));
                return is_base_and_derived_kinds<source_type>(remapped<target_type,layout>::lbl, derived_kind); 
            }

            /// Performs the necessary conversion of the original subject into the proper
            /// object of target type.
            /// \note The subject is const-qualified, thus the target is also const-qualified
            static inline const target_type* get_matched(const source_type* subject_ptr, local_data_type&) noexcept
            {
                return subject_ptr;
            }

            /// Performs the necessary conversion of the original subject into the proper
            /// object of target type.
            /// \note The subject is non-const, thus the target is also non-const
            static inline       target_type* get_matched(      source_type* subject_ptr, local_data_type&) noexcept
            {
                return subject_ptr;
            }
        };
    };

    /// Structure used to disambiguate whether first argument is a type or a value.
    /// \note This specialization handles a type, which can be seen from the type of 
    ///       parameter struct.
    template <typename bogus_parameter>
    struct disambiguate<false,bogus_parameter>
    {
        /// Essentially a catcher of the first argument of the case clause
        /// a type in this case.
        template <typename T>
        struct parameter
        {
            /// The type passed as a first argument of the case clause is the target type.
            typedef typename target_of<T>::type target_type;

            /// Layout that has to be used for the given target type.
            enum { layout = target_of<T>::layout };

            /// Depending on whether we handle open or closed case, different case labels
            /// are used for the generated match statement. This metafunction takes
            /// a unique (per match statement) counter and returns the actual label that
            /// will be used for the case clause.
            template <size_t Counter>
            struct CaseLabel
            {
                enum 
                {
                    value = remapped<target_type>::lbl ///< Case label that will be used for case at line offset Counter
                };
            };

            /// Condition that guards applicability of the given case clause
            /// during the fall-through behavior.
            static inline bool main_condition(const source_type* subject_ptr, local_data_type&) 
            { 
                lbl_type derived_kind = original2remapped<source_type>(tag_type(kind_selector(subject_ptr)));
                return is_base_and_derived_kinds<source_type>(remapped<target_type,layout>::lbl, derived_kind); 
            }

            /// Performs the necessary conversion of the original subject into the proper
            /// object of target type.
            /// \note The subject is const-qualified, thus the target is also const-qualified
            static inline const target_type* get_matched(const source_type* subject_ptr, local_data_type&) noexcept
            {
                return stat_cast<target_type>(subject_ptr);
            }

            /// Performs the necessary conversion of the original subject into the proper
            /// object of target type.
            /// \note The subject is non-const, thus the target is also non-const
            static inline       target_type* get_matched(      source_type* subject_ptr, local_data_type&) noexcept
            {
                return stat_cast<target_type>(subject_ptr);
            }
        };
    };
};

//------------------------------------------------------------------------------

} // of namespace mch

namespace std 
{
    template <> struct hash<mch::lbl_type> { size_t operator()(const mch::lbl_type& l) const noexcept { return l; } };
} // of namespace std
