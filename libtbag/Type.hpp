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
#include <memory>
#include <string>
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

#ifndef STATIC_ASSERT_CHECK_IS_BASE_OF
#define STATIC_ASSERT_CHECK_IS_BASE_OF(parent, derived) \
    static_assert(std::is_base_of<parent, derived>::value, "Not Derived " #parent " to " #derived)
#endif

#ifndef STATIC_ASSERT_CHECK_IS_SAME
#define STATIC_ASSERT_CHECK_IS_SAME(type1, type2) \
    static_assert(std::is_same<type1, type2>::value, #type1 " must be the same type as " #type2)
#endif

#ifndef STATIC_ASSERT_CHECK_IS_EQUALS
#define STATIC_ASSERT_CHECK_IS_EQUALS(value1, value2) \
    static_assert(value1 == value2, #value1 " and " #value2 " should be the same.")
#endif

#ifndef STATIC_ASSERT_INTEGER_EQUAL
#define STATIC_ASSERT_INTEGER_EQUAL(x, y) \
    STATIC_ASSERT_CHECK_IS_EQUALS((int)(x), (int)(y))
#endif

#ifndef STATIC_ASSERT_CHECK_SIZEOF
#define STATIC_ASSERT_CHECK_SIZEOF(type, value) \
    static_assert(sizeof(type) == value, "The size of the " #type " must be " #value ".")
#endif

#ifndef STATIC_ASSERT_CHECK_IS_DEFAULT_CONSTRUCTIBLE
#define STATIC_ASSERT_CHECK_IS_DEFAULT_CONSTRUCTIBLE(class_name)    \
    static_assert(std::is_default_constructible<class_name>::value, \
            "The " #class_name " class must support the default constructor.")
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

template <typename T, typename SharedType = T>
struct is_shared_ptr : public std::false_type
{ /* EMPTY. */ };

template <typename T>
struct is_shared_ptr<std::shared_ptr<T> > : public std::true_type
{ /* EMPTY. */ };

static_assert(is_shared_ptr<int>::value == false, "T is not std::shared_ptr type.");
static_assert(is_shared_ptr<std::shared_ptr<int> >::value, "T is std::shared_ptr type.");

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

/**
 * Tests for a string literal type.
 *
 * @remarks
 *  <code>decltype("some string")</code> of a string literal returns <code>const char (&)[n]</code> type.
 *
 * @author zer0
 * @date   2019-10-17
 */
template <typename T>
struct is_string_literal
{
    typedef typename remove_cr<T>::type __base_t;
    typedef typename std::add_lvalue_reference<const char [std::extent<__base_t>::value]>::type __test_t;

    TBAG_CONSTEXPR static bool value = std::is_same<T, __test_t>::value;
};

/**
 * Test that the first template argument is a string type.
 *
 * @author zer0
 * @date   2019-12-24
 */
template <typename ... T>
struct is_first_string;

template <typename Head, typename ... Tail>
struct is_first_string<Head, Tail...>
{
    TBAG_CONSTEXPR static bool const value = is_first_string<Head>::value;
};

template <typename T>
struct is_first_string<T>
{
    TBAG_CONSTEXPR static bool const value =
            std::is_same<typename remove_cr<T>::type, std::string>::value ||
            is_string_literal<T>::value;
};

/**
 * Test that all template arguments are of type integer.
 *
 * @author zer0
 * @date   2019-12-28
 */
template <typename ... T>
struct is_all_integral;

template <typename Head, typename ... Tail>
struct is_all_integral<Head, Tail...>
{
    TBAG_CONSTEXPR static bool const value = is_all_integral<Head>::value && is_all_integral<Tail ...>::value;
};

template <typename T>
struct is_all_integral<T>
{
    TBAG_CONSTEXPR static bool const value = std::is_integral<typename remove_cr<T>::type>::value;
};

struct constexpr_init_t { /* EMPTY. */ };

TBAG_CONSTEXPR static constexpr_init_t const constexpr_init = {};

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

