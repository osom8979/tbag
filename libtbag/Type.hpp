/**
 * @file   Type.hpp
 * @brief  Type utilities.
 * @author zer0
 * @date   2016-04-20
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TYPE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TYPE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <cstddef>

#include <utility>
#include <type_traits>

#define STATIC_ASSERT_CHECK_TRAIT1(traits, type) \
    static_assert(traits<type>::value, #traits "<" #type ">::value");

#define STATIC_ASSERT_CHECK_TRAIT2(traits, type1, type2) \
    static_assert(traits<type1, type2>::value, #traits "<" #type1 "," #type2 ">::value");

#define STATIC_ASSERT_FULL_SUPPORTED_OPERATIONS(type)                     \
    STATIC_ASSERT_CHECK_TRAIT1(std::is_constructible        , type)       \
    STATIC_ASSERT_CHECK_TRAIT1(std::is_default_constructible, type)       \
    STATIC_ASSERT_CHECK_TRAIT1(std::is_copy_assignable      , type)       \
    STATIC_ASSERT_CHECK_TRAIT1(std::is_move_constructible   , type)       \
    STATIC_ASSERT_CHECK_TRAIT2(std::is_assignable           , type, type) \
    STATIC_ASSERT_CHECK_TRAIT1(std::is_copy_assignable      , type)       \
    STATIC_ASSERT_CHECK_TRAIT1(std::is_move_assignable      , type)       \
    STATIC_ASSERT_CHECK_TRAIT1(std::is_destructible         , type)       \
    STATIC_ASSERT_CHECK_TRAIT1(std::has_virtual_destructor  , type)

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

/** Common ID type. */
typedef std::size_t Id;

/** Unknown ID. */
Id const UNKNOWN_ID = 0;

// ----------------
// Transformations.
// ----------------

/**
 * Remove const & reference.
 *
 * @remarks
 *  - First, remove reference,
 *  - Second, remove const.
 *
 * @author zer0
 * @date   2015-10-17
 * @date   2015-10-21 (Bug fix: Change transformation order)
 */
template <typename T>
struct remove_cr
{
    typedef typename std::remove_reference<T>::type remove_reference;
    typedef typename std::remove_const<remove_reference>::type remove_reference_const;

    typedef remove_reference_const type;
};

// Test assertion.
static_assert(!std::is_const<typename remove_cr<int const &>::type>::value
        , "It isn't non-const type.");
static_assert(!std::is_reference<typename remove_cr<int const &>::type>::value
        , "It isn't non-reference type.");

/**
 * Create a 2-dimensional trivial structure.
 *
 * @author zer0
 * @date   2015-10-21
 */
template <template <typename Tp> class TrivialType
        , typename T1
        , typename T2 = T1>
inline TBAG_CONSTEXPR TrivialType<typename remove_cr<T1>::type>
makeTrivial2(T1 && v1, T2 && v2) TBAG_NOEXCEPT
{
    typedef typename remove_cr<T1>::type __remove_cr;
    typedef TrivialType<__remove_cr> __trivial_type;
    return (__trivial_type{std::forward<T1>(v1), std::forward<T2>(v2)});
}

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TYPE_HPP__

