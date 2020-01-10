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
#include <complex>
#include <memory>
#include <string>
#include <tuple>
#include <functional>
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

// Test std::shared_ptr

template <typename T, typename SharedType = T>
struct is_shared_ptr : public std::false_type
{ /* EMPTY. */ };

template <typename T>
struct is_shared_ptr< std::shared_ptr<T> > : public std::true_type
{ /* EMPTY. */ };

static_assert(!is_shared_ptr<int>::value, "T is not std::shared_ptr type.");
static_assert(is_shared_ptr< std::shared_ptr<int> >::value, "T is std::shared_ptr type.");

// Test std::complex

template <typename T, typename SharedType = T>
struct is_complex : public std::false_type
{ /* EMPTY. */ };

template <typename T>
struct is_complex< std::complex<T> > : public std::true_type
{ /* EMPTY. */ };

static_assert(!is_complex<float>::value, "T is not std::complex type.");
static_assert(is_complex< std::complex<float> >::value, "T is std::complex type.");

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
    using __t0 = T;
    using __t1 = typename std::remove_reference<__t0>::type;
    using __t2 = typename std::remove_const<__t1>::type;
    using type = __t2;
};

/**
 * Remove const & reference & pointer.
 *
 * @remarks
 *  - First, remove reference,
 *  - Second, remove pointer,
 *  - Third, remove const.
 *
 * @author zer0
 * @date   2019-12-30
 */
template <typename T>
struct remove_cpr
{
    using __t0 = T;
    using __t1 = typename std::remove_reference<__t0>::type;
    using __t2 = typename std::remove_pointer<__t1>::type;
    using __t3 = typename std::remove_const<__t2>::type;
    using type = __t3;
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
 * Obtain the first template argument type.
 *
 * @author zer0
 * @date   2019-12-29
 */
template <typename ... T>
struct first_argument;

template <typename Head, typename ... Tail>
struct first_argument<Head, Tail...>
{
    using type = Head;
};

template <typename T>
struct first_argument<T>
{
    using type = T;
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

template <typename T>
struct function_traits;

template <typename RetT, typename ... ArgsT>
struct function_traits<RetT(ArgsT...)>
{
    TBAG_CONSTEXPR static std::size_t const argument_size = sizeof...(ArgsT);

    using return_type = RetT;
    using function_type = RetT(ArgsT...);

    template <std::size_t i>
    struct arguments
    {
        using type = typename std::tuple_element<i, std::tuple<ArgsT ...> >::type;
    };
};

template <typename RetT, typename ... ArgsT>
struct function_traits<RetT(*)(ArgsT...)> : public function_traits<RetT(ArgsT...)>
{ /* EMPTY. */ };

template <typename RetT, typename ClassT, typename ... ArgsT>
struct function_traits<RetT(ClassT::*)(ArgsT...)> : public function_traits<RetT(ArgsT...)>
{
    using class_type = ClassT;
};

template <typename RetT, typename ClassT, typename ... ArgsT>
struct function_traits<RetT(ClassT::*)(ArgsT...) const> : public function_traits<RetT(ArgsT...)>
{
    using class_type = ClassT;
};

template <typename RetT, typename ClassT, typename ... ArgsT>
struct function_traits<RetT(ClassT::*)(ArgsT...) volatile> : public function_traits<RetT(ArgsT...)>
{
    using class_type = ClassT;
};

template <typename RetT, typename ClassT, typename ... ArgsT>
struct function_traits<RetT(ClassT::*)(ArgsT...) volatile const> : public function_traits<RetT(ArgsT...)>
{
    using class_type = ClassT;
};

template <typename T>
struct function_traits : public function_traits<decltype(&T::operator())>
{ /* EMPTY. */ };

template <typename T>
struct function_traits< std::function<T> > : public function_traits<T>
{ /* EMPTY. */ };

#if defined(_GLIBCXX_FUNCTIONAL)
# define _TBAG_MEM_FUNCTIONAL_TYPE std::_Mem_fn
#elif defined(_LIBCPP_FUNCTIONAL)
# define _TBAG_MEM_FUNCTIONAL_TYPE std::__mem_fn
#endif

#if defined(_TBAG_MEM_FUNCTIONAL_TYPE)
template <typename RetT, typename ClassT>
struct function_traits<_TBAG_MEM_FUNCTIONAL_TYPE<RetT(ClassT::*)> >
        : public function_traits<RetT(ClassT*)>
{ /* EMPTY. */ };

template <typename RetT, typename ClassT, typename ... ArgsT>
struct function_traits<_TBAG_MEM_FUNCTIONAL_TYPE<RetT(ClassT::*)(ArgsT...)> >
        : public function_traits<RetT(ClassT*, ArgsT...)>
{ /* EMPTY. */ };

template <typename RetT, typename ClassT, typename ... ArgsT>
struct function_traits<_TBAG_MEM_FUNCTIONAL_TYPE<RetT(ClassT::*)(ArgsT...) const> >
        : public function_traits<RetT(const ClassT*, ArgsT...)>
{ /* EMPTY. */ };

template <typename RetT, typename ClassT, typename ... ArgsT>
struct function_traits<_TBAG_MEM_FUNCTIONAL_TYPE<RetT(ClassT::*)(ArgsT...) volatile> >
        : public function_traits<RetT(volatile ClassT*, ArgsT...)>
{ /* EMPTY. */ };

template <typename RetT, typename ClassT, typename ... ArgsT>
struct function_traits<_TBAG_MEM_FUNCTIONAL_TYPE<RetT(ClassT::*)(ArgsT...) volatile const> >
        : public function_traits<RetT(volatile const ClassT*, ArgsT...)>
{ /* EMPTY. */ };

#endif // _TBAG_MEM_FUNCTIONAL_TYPE

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

