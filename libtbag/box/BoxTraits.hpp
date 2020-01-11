/**
 * @file   BoxTraits.hpp
 * @brief  BoxTraits class prototype.
 * @author zer0
 * @date   2019-12-30
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_BOX_BOXTRAITS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_BOX_BOXTRAITS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Type.hpp>
#include <libtbag/box/details/box_api.hpp>

#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace box {

// Forward declarations.
class Box;

using si8   = libtbag::box::details::si8;
using si16  = libtbag::box::details::si16;
using si32  = libtbag::box::details::si32;
using si64  = libtbag::box::details::si64;

using ui8   = libtbag::box::details::ui8;
using ui16  = libtbag::box::details::ui16;
using ui32  = libtbag::box::details::ui32;
using ui64  = libtbag::box::details::ui64;

using fp32  = libtbag::box::details::fp32;
using fp64  = libtbag::box::details::fp64;

using c64  = libtbag::box::details::c64;
using c128 = libtbag::box::details::c128;

using btype = libtbag::box::details::btype;
using bdev  = libtbag::box::details::bdev;

using box_data   = libtbag::box::details::box_data;
using box_slice  = libtbag::box::details::box_slice;
using box_cursor = libtbag::box::details::box_cursor;
using box_any    = libtbag::box::details::box_any;

using box_opaque_delete_cb = libtbag::box::details::box_opaque_delete_cb;

/**
 * Type table of Box class.
 *
 * @author zer0
 * @date   2018-11-07
 */
enum class BoxTypeTable : libtbag::box::details::btype
{
    // clang-format off
    BTT_NONE       = libtbag::box::details::BT_NONE      ,
    BTT_BOOL       = libtbag::box::details::BT_BOOL      ,
    BTT_INT8       = libtbag::box::details::BT_INT8      ,
    BTT_INT16      = libtbag::box::details::BT_INT16     ,
    BTT_INT32      = libtbag::box::details::BT_INT32     ,
    BTT_INT64      = libtbag::box::details::BT_INT64     ,
    BTT_UINT8      = libtbag::box::details::BT_UINT8     ,
    BTT_UINT16     = libtbag::box::details::BT_UINT16    ,
    BTT_UINT32     = libtbag::box::details::BT_UINT32    ,
    BTT_UINT64     = libtbag::box::details::BT_UINT64    ,
    BTT_FLOAT32    = libtbag::box::details::BT_FLOAT32   ,
    BTT_FLOAT64    = libtbag::box::details::BT_FLOAT64   ,
    BTT_COMPLEX64  = libtbag::box::details::BT_COMPLEX64 ,
    BTT_COMPLEX128 = libtbag::box::details::BT_COMPLEX128,
    // clang-format on
};

template <typename T, BoxTypeTable box_type>
struct BoxInfo
{
    using type = T;
    TBAG_CONSTEXPR static BoxTypeTable const value = box_type;
    TBAG_CONSTEXPR static std::size_t const size = sizeof(T);
    TBAG_CONSTEXPR static bool const is_egg_type = true;
};

template <typename T>
struct BoxInfo<T, BoxTypeTable::BTT_NONE>
{
    using type = T;
    TBAG_CONSTEXPR static BoxTypeTable const value = BoxTypeTable::BTT_NONE;
    TBAG_CONSTEXPR static std::size_t const size = sizeof(T);
    TBAG_CONSTEXPR static bool const is_egg_type = false;
};

template <>
struct BoxInfo<void, BoxTypeTable::BTT_NONE>
{
    using type = void;
    TBAG_CONSTEXPR static BoxTypeTable const value = BoxTypeTable::BTT_NONE;
    TBAG_CONSTEXPR static std::size_t const size = 0;
    TBAG_CONSTEXPR static bool const is_egg_type = false;
};

// clang-format off
template <typename T> struct BoxTypeInfo : public BoxInfo<T, BoxTypeTable::BTT_NONE     > { /* EMPTY. */ };
template <> struct BoxTypeInfo<bool> : public BoxInfo<bool, BoxTypeTable::BTT_BOOL      > { /* EMPTY. */ };
template <> struct BoxTypeInfo<si8 > : public BoxInfo<si8 , BoxTypeTable::BTT_INT8      > { /* EMPTY. */ };
template <> struct BoxTypeInfo<si16> : public BoxInfo<si16, BoxTypeTable::BTT_INT16     > { /* EMPTY. */ };
template <> struct BoxTypeInfo<si32> : public BoxInfo<si32, BoxTypeTable::BTT_INT32     > { /* EMPTY. */ };
template <> struct BoxTypeInfo<si64> : public BoxInfo<si64, BoxTypeTable::BTT_INT64     > { /* EMPTY. */ };
template <> struct BoxTypeInfo<ui8 > : public BoxInfo<ui8 , BoxTypeTable::BTT_UINT8     > { /* EMPTY. */ };
template <> struct BoxTypeInfo<ui16> : public BoxInfo<ui16, BoxTypeTable::BTT_UINT16    > { /* EMPTY. */ };
template <> struct BoxTypeInfo<ui32> : public BoxInfo<ui32, BoxTypeTable::BTT_UINT32    > { /* EMPTY. */ };
template <> struct BoxTypeInfo<ui64> : public BoxInfo<ui64, BoxTypeTable::BTT_UINT64    > { /* EMPTY. */ };
template <> struct BoxTypeInfo<fp32> : public BoxInfo<fp32, BoxTypeTable::BTT_FLOAT32   > { /* EMPTY. */ };
template <> struct BoxTypeInfo<fp64> : public BoxInfo<fp64, BoxTypeTable::BTT_FLOAT64   > { /* EMPTY. */ };
template <> struct BoxTypeInfo<c64 > : public BoxInfo<c64 , BoxTypeTable::BTT_COMPLEX64 > { /* EMPTY. */ };
template <> struct BoxTypeInfo<c128> : public BoxInfo<c128, BoxTypeTable::BTT_COMPLEX128> { /* EMPTY. */ };
// clang-format on

// clang-format off
template <BoxTypeTable egg_type> struct BoxEnumInfo          : public BoxInfo<void, BoxTypeTable::BTT_NONE      > { /* EMPTY. */ };
template <> struct BoxEnumInfo<BoxTypeTable::BTT_BOOL      > : public BoxInfo<bool, BoxTypeTable::BTT_BOOL      > { /* EMPTY. */ };
template <> struct BoxEnumInfo<BoxTypeTable::BTT_INT8      > : public BoxInfo<si8 , BoxTypeTable::BTT_INT8      > { /* EMPTY. */ };
template <> struct BoxEnumInfo<BoxTypeTable::BTT_INT16     > : public BoxInfo<si16, BoxTypeTable::BTT_INT16     > { /* EMPTY. */ };
template <> struct BoxEnumInfo<BoxTypeTable::BTT_INT32     > : public BoxInfo<si32, BoxTypeTable::BTT_INT32     > { /* EMPTY. */ };
template <> struct BoxEnumInfo<BoxTypeTable::BTT_INT64     > : public BoxInfo<si64, BoxTypeTable::BTT_INT64     > { /* EMPTY. */ };
template <> struct BoxEnumInfo<BoxTypeTable::BTT_UINT8     > : public BoxInfo<ui8 , BoxTypeTable::BTT_UINT8     > { /* EMPTY. */ };
template <> struct BoxEnumInfo<BoxTypeTable::BTT_UINT16    > : public BoxInfo<ui16, BoxTypeTable::BTT_UINT16    > { /* EMPTY. */ };
template <> struct BoxEnumInfo<BoxTypeTable::BTT_UINT32    > : public BoxInfo<ui32, BoxTypeTable::BTT_UINT32    > { /* EMPTY. */ };
template <> struct BoxEnumInfo<BoxTypeTable::BTT_UINT64    > : public BoxInfo<ui64, BoxTypeTable::BTT_UINT64    > { /* EMPTY. */ };
template <> struct BoxEnumInfo<BoxTypeTable::BTT_FLOAT32   > : public BoxInfo<fp32, BoxTypeTable::BTT_FLOAT32   > { /* EMPTY. */ };
template <> struct BoxEnumInfo<BoxTypeTable::BTT_FLOAT64   > : public BoxInfo<fp64, BoxTypeTable::BTT_FLOAT64   > { /* EMPTY. */ };
template <> struct BoxEnumInfo<BoxTypeTable::BTT_COMPLEX64 > : public BoxInfo<c64 , BoxTypeTable::BTT_COMPLEX64 > { /* EMPTY. */ };
template <> struct BoxEnumInfo<BoxTypeTable::BTT_COMPLEX128> : public BoxInfo<c128, BoxTypeTable::BTT_COMPLEX128> { /* EMPTY. */ };
// clang-format on

// clang-format off
static_assert(BoxTypeInfo<void>::size ==  0, "void size is not correct.");
static_assert(BoxTypeInfo<si8 >::size ==  1,  "si8 size is not correct.");
static_assert(BoxTypeInfo<si16>::size ==  2, "si16 size is not correct.");
static_assert(BoxTypeInfo<si32>::size ==  4, "si32 size is not correct.");
static_assert(BoxTypeInfo<si64>::size ==  8, "si64 size is not correct.");
static_assert(BoxTypeInfo<ui8 >::size ==  1,  "ui8 size is not correct.");
static_assert(BoxTypeInfo<ui16>::size ==  2, "ui16 size is not correct.");
static_assert(BoxTypeInfo<ui32>::size ==  4, "ui32 size is not correct.");
static_assert(BoxTypeInfo<ui64>::size ==  8, "ui64 size is not correct.");
static_assert(BoxTypeInfo<fp32>::size ==  4, "fp32 size is not correct.");
static_assert(BoxTypeInfo<fp64>::size ==  8, "fp64 size is not correct.");
static_assert(BoxTypeInfo<c64 >::size ==  8,  "c64 size is not correct.");
static_assert(BoxTypeInfo<c128>::size == 16, "c128 size is not correct.");
// clang-format on

template <typename T>
struct is_supported_egg : public BoxTypeInfo<T>
{ /* EMPTY. */ };

template <typename T, bool is_unsigned, std::size_t size, bool is_floating, bool is_complex>
struct __make_egg_type : public BoxTypeInfo<void>
{ /* EMPTY. */ };

// clang-format off
template <typename T, bool is_unsigned> struct __make_egg_type<T, is_unsigned, 4, true, false> : public BoxTypeInfo<float>  { /* EMPTY. */ };
template <typename T, bool is_unsigned> struct __make_egg_type<T, is_unsigned, 8, true, false> : public BoxTypeInfo<double> { /* EMPTY. */ };
// clang-format on

// clang-format off
template <typename T> struct __make_egg_type<T, false, 1, false, false> : public BoxTypeInfo<  int8_t> { /* EMPTY. */ };
template <typename T> struct __make_egg_type<T,  true, 1, false, false> : public BoxTypeInfo< uint8_t> { /* EMPTY. */ };
template <typename T> struct __make_egg_type<T, false, 2, false, false> : public BoxTypeInfo< int16_t> { /* EMPTY. */ };
template <typename T> struct __make_egg_type<T,  true, 2, false, false> : public BoxTypeInfo<uint16_t> { /* EMPTY. */ };
template <typename T> struct __make_egg_type<T, false, 4, false, false> : public BoxTypeInfo< int32_t> { /* EMPTY. */ };
template <typename T> struct __make_egg_type<T,  true, 4, false, false> : public BoxTypeInfo<uint32_t> { /* EMPTY. */ };
template <typename T> struct __make_egg_type<T, false, 8, false, false> : public BoxTypeInfo< int64_t> { /* EMPTY. */ };
template <typename T> struct __make_egg_type<T,  true, 8, false, false> : public BoxTypeInfo<uint64_t> { /* EMPTY. */ };
// clang-format on

// clang-format off
template <typename T> struct __make_egg_type<T, false,  8, false, true> : public BoxTypeInfo<c64> { /* EMPTY. */ };
template <typename T> struct __make_egg_type<T, false, 16, false, true> : public BoxTypeInfo<c128> { /* EMPTY. */ };
// clang-format on

template <typename T>
struct make_egg_type : public __make_egg_type<
        T,
        std::is_unsigned<T>::value,
        sizeof(T),
        std::is_floating_point<T>::value,
        is_complex<T>::value>
{
    using original_type = T;
};

/**
 * The following applies:
 *  - Remove <code>const</code> decoration.
 *  - Remove <code>& (reference)</code> decoration.
 *  - keep <code>signed char</code> and <code>char</code> type results the same.
 */
template <typename T>
struct btype_regularization
{
    using __data_t = typename libtbag::remove_cr<T>::type;
    using type = typename std::conditional<std::is_same<__data_t, char>::value, si8, __data_t>::type;
};

template <typename T>
TBAG_CONSTEXPR inline BoxTypeTable getBoxType() TBAG_NOEXCEPT
{
    return BoxTypeInfo<typename btype_regularization<T>::type>::value;
}

template <typename T>
inline bool isTypeEquals(BoxTypeTable type) TBAG_NOEXCEPT
{
    return type == getBoxType<typename btype_regularization<T>::type>();
}

template <typename T>
TBAG_CONSTEXPR inline libtbag::box::details::btype get_btype() TBAG_NOEXCEPT
{
    return static_cast<libtbag::box::details::btype>(BoxTypeInfo<typename btype_regularization<T>::type>::value);
}

template <typename T>
inline bool is_btype_equals(libtbag::box::details::btype type) TBAG_NOEXCEPT
{
    return type == get_btype<typename btype_regularization<T>::type>();
}

TBAG_CONSTEXPR int const nop = libtbag::box::details::box_nop;

TBAG_CONSTEXPR btype const type_none() TBAG_NOEXCEPT { return get_btype<void>(); }
TBAG_CONSTEXPR btype const type_bool() TBAG_NOEXCEPT { return get_btype<bool>(); }
TBAG_CONSTEXPR btype const type_si8 () TBAG_NOEXCEPT { return get_btype<si8 >(); }
TBAG_CONSTEXPR btype const type_si16() TBAG_NOEXCEPT { return get_btype<si16>(); }
TBAG_CONSTEXPR btype const type_si32() TBAG_NOEXCEPT { return get_btype<si32>(); }
TBAG_CONSTEXPR btype const type_si64() TBAG_NOEXCEPT { return get_btype<si64>(); }
TBAG_CONSTEXPR btype const type_ui8 () TBAG_NOEXCEPT { return get_btype<ui8 >(); }
TBAG_CONSTEXPR btype const type_ui16() TBAG_NOEXCEPT { return get_btype<ui16>(); }
TBAG_CONSTEXPR btype const type_ui32() TBAG_NOEXCEPT { return get_btype<ui32>(); }
TBAG_CONSTEXPR btype const type_ui64() TBAG_NOEXCEPT { return get_btype<ui64>(); }
TBAG_CONSTEXPR btype const type_fp32() TBAG_NOEXCEPT { return get_btype<fp32>(); }
TBAG_CONSTEXPR btype const type_fp64() TBAG_NOEXCEPT { return get_btype<fp64>(); }
TBAG_CONSTEXPR btype const type_c64 () TBAG_NOEXCEPT { return get_btype<c64 >(); }
TBAG_CONSTEXPR btype const type_c128() TBAG_NOEXCEPT { return get_btype<c128>(); }

TBAG_CONSTEXPR bdev const device_cpu () TBAG_NOEXCEPT { return libtbag::box::details::BD_CPU ; }
TBAG_CONSTEXPR bdev const device_cuda() TBAG_NOEXCEPT { return libtbag::box::details::BD_CUDA; }
TBAG_CONSTEXPR bdev const device_cl  () TBAG_NOEXCEPT { return libtbag::box::details::BD_CL  ; }

template <typename ... T>
struct is_first_box_data;

template <typename Head, typename ... Tail>
struct is_first_box_data<Head, Tail...>
{
    TBAG_CONSTEXPR static bool const value = is_first_box_data<Head>::value;
};

template <typename T>
struct is_first_box_data<T>
{
    using __type = typename libtbag::remove_cpr<T>::type;
    TBAG_CONSTEXPR static bool const value = std::is_same<__type, libtbag::box::details::box_data>::value;
};

template <typename ... T>
struct is_first_Box;

template <typename Head, typename ... Tail>
struct is_first_Box<Head, Tail...>
{
    TBAG_CONSTEXPR static bool const value = is_first_Box<Head>::value;
};

template <typename T>
struct is_first_Box<T>
{
    using __type = typename libtbag::remove_cpr<T>::type;
    TBAG_CONSTEXPR static bool const value = std::is_same<__type, libtbag::box::Box>::value;
};

template <typename ... T>
struct is_last_va_list;

template <typename Head, typename ... Tail>
struct is_last_va_list<Head, Tail...>
{
    TBAG_CONSTEXPR static bool const value = is_last_va_list<Tail...>::value;
};

template <typename T>
struct is_last_va_list<T>
{
    // [WARNING]
    // Do not use 'std::remove_pointer' type traits.
    // In MSVC, va_list is declared as a pointer.
    using __type = typename libtbag::remove_cr<T>::type;
    TBAG_CONSTEXPR static bool const value = std::is_same<__type, va_list>::value;
};

template <typename ... T>
struct is_last_ui32_ptr;

template <typename Head, typename ... Tail>
struct is_last_ui32_ptr<Head, Tail...>
{
    TBAG_CONSTEXPR static bool const value = is_last_ui32_ptr<Tail...>::value;
};

template <typename T>
struct is_last_ui32_ptr<T>
{
    using __type = typename libtbag::remove_cpr<T>::type;
    TBAG_CONSTEXPR static bool const value = std::is_same<__type, ui32>::value && std::is_pointer<T>::value;
};

template <typename ... T>
struct is_first_bdev_and_second_ui64_ptr;

template <typename First, typename Second, typename ... Tail>
struct is_first_bdev_and_second_ui64_ptr<First, Second, Tail...>
{
    TBAG_CONSTEXPR static bool const value = is_first_bdev_and_second_ui64_ptr<First, Second>::value;
};

template <typename First, typename Second>
struct is_first_bdev_and_second_ui64_ptr<First, Second>
{
    using __first = typename libtbag::remove_cpr<First>::type;
    using __second = typename libtbag::remove_cpr<Second>::type;

    TBAG_CONSTEXPR static bool const value =
            std::is_same<__first, libtbag::box::details::bdev>::value &&
            std::is_same<__second, ui64>::value &&
            std::is_pointer<Second>::value;
};

template <typename T>
struct is_first_bdev_and_second_ui64_ptr<T>
{
    TBAG_CONSTEXPR static bool const value = false;
};

struct shape_error_t { /* EMPTY. */ };
struct shape_args1_t { /* EMPTY. */ };
struct shape_args2_t { /* EMPTY. */ };
struct shape_dims1_t { /* EMPTY. */ };
struct shape_dims2_t { /* EMPTY. */ };
struct shape_like1_t { /* EMPTY. */ };
struct shape_like2_t { /* EMPTY. */ };

template <typename ... Args>
struct shape_selector
{
    TBAG_CONSTEXPR static bool const is_shape_ref_box1 =
            sizeof...(Args) == 1 &&
            is_first_box_data<Args...>::value;
    TBAG_CONSTEXPR static bool const is_shape_ref_box2 =
            sizeof...(Args) == 1 &&
            is_first_Box<Args...>::value;

    TBAG_CONSTEXPR static bool const is_shape_dims1 =
            sizeof...(Args) == 4 &&
            is_first_bdev_and_second_ui64_ptr<Args...>::value &&
            is_last_ui32_ptr<Args...>::value;
    TBAG_CONSTEXPR static bool const is_shape_dims2 =
            sizeof...(Args) == 2 &&
            is_last_ui32_ptr<Args...>::value;

    TBAG_CONSTEXPR static bool const is_shape_args1 =
            sizeof...(Args) >= 3 &&
            is_first_bdev_and_second_ui64_ptr<Args...>::value;
    TBAG_CONSTEXPR static bool const is_shape_args2 =
            sizeof...(Args) >= 1 &&
            is_all_integral<Args...>::value;

    // clang-format off
    using type = typename std::conditional<is_shape_ref_box1, shape_like1_t,
                 typename std::conditional<is_shape_ref_box2, shape_like2_t,
                 typename std::conditional<is_shape_dims1, shape_dims1_t,
                 typename std::conditional<is_shape_dims2, shape_dims2_t,
                 typename std::conditional<is_shape_args1, shape_args1_t,
                 typename std::conditional<is_shape_args2, shape_args2_t,
                 shape_error_t
                 >::type // shape_args2_t
                 >::type // shape_args1_t
                 >::type // shape_dims2_t
                 >::type // shape_dims1_t
                 >::type // shape_ref_box2_t
                 >::type;// shape_ref_box1_t
    // clang-format on

    TBAG_CONSTEXPR static type const value = { /* EMPTY. */ };
};

template <typename ... Args>
struct index_selector
{
    TBAG_CONSTEXPR static bool const is_shape_dims1 =
            sizeof...(Args) == 4 &&
            is_first_bdev_and_second_ui64_ptr<Args...>::value &&
            is_last_ui32_ptr<Args...>::value;
    TBAG_CONSTEXPR static bool const is_shape_dims2 =
            sizeof...(Args) == 2 &&
            is_last_ui32_ptr<Args...>::value;

    TBAG_CONSTEXPR static bool const is_shape_args1 =
            sizeof...(Args) >= 3 &&
            is_first_bdev_and_second_ui64_ptr<Args...>::value;
    TBAG_CONSTEXPR static bool const is_shape_args2 =
            sizeof...(Args) >= 1 &&
            is_all_integral<Args...>::value;

    // clang-format off
    using type = typename std::conditional<is_shape_dims1, shape_dims1_t,
                 typename std::conditional<is_shape_dims2, shape_dims2_t,
                 typename std::conditional<is_shape_args1, shape_args1_t,
                 typename std::conditional<is_shape_args2, shape_args2_t,
                 shape_error_t
                 >::type // shape_args2_t
                 >::type // shape_args1_t
                 >::type // shape_dims2_t
                 >::type;// shape_dims1_t
    // clang-format on

    TBAG_CONSTEXPR static type const value = { /* EMPTY. */ };
};

struct access_unknown_t   { /* EMPTY. */ };
struct access_const_ptr_t { /* EMPTY. */ };
struct access_const_ref_t { /* EMPTY. */ };
struct access_ptr_t       { /* EMPTY. */ };
struct access_ref_t       { /* EMPTY. */ };

template <typename T>
struct access_selector
{
    TBAG_CONSTEXPR static bool const is_reference = std::is_reference<T>::value;
    using __t1 = typename std::remove_reference<T>::type;
    TBAG_CONSTEXPR static bool const is_pointer = std::is_pointer<__t1>::value;
    using __t2 = typename std::remove_pointer<__t1>::type;
    TBAG_CONSTEXPR static bool const is_const = std::is_const<__t2>::value;

    // clang-format off
    using type = typename std::conditional< is_const &&  is_pointer && !is_reference, access_const_ptr_t,
                 typename std::conditional< is_const && !is_pointer &&  is_reference, access_const_ref_t,
                 typename std::conditional<!is_const &&  is_pointer && !is_reference, access_ptr_t,
                 typename std::conditional<!is_const && !is_pointer &&  is_reference, access_ref_t,
                 access_unknown_t
                 >::type // access_ref_t
                 >::type // access_ptr_t
                 >::type // access_const_ref_t
                 >::type;// access_const_ptr_t
    // clang-format on

    TBAG_CONSTEXPR static type const value = { /* EMPTY. */ };
};

} // namespace box

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_BOX_BOXTRAITS_HPP__

