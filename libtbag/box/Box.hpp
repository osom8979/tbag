/**
 * @file   Box.hpp
 * @brief  Box class prototype.
 * @author zer0
 * @date   2019-05-16
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_BOX_BOX_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_BOX_BOX_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Type.hpp>
#include <libtbag/box/details/box_api.hpp>
#include <libtbag/box/BoxCursor.hpp>
#include <libtbag/box/BoxIterator.hpp>
#include <libtbag/box/BoxIteratorGenerator.hpp>
#include <libtbag/box/BoxPacket.hpp>
#include <libtbag/box/BoxTraits.hpp>

#include <cstddef>
#include <cstdlib>
#include <cstdarg>
#include <cstring>
#include <cassert>

#include <type_traits>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <string>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace box {

// Forward declarations.
class Box;

/**
 * Type table of Box class.
 *
 * @author zer0
 * @date   2018-11-07
 */
enum class BoxTypeTable : libtbag::box::details::btype
{
    // clang-format off
    BTT_NONE    = libtbag::box::details::BT_NONE   ,
    BTT_INT8    = libtbag::box::details::BT_INT8   ,
    BTT_INT16   = libtbag::box::details::BT_INT16  ,
    BTT_INT32   = libtbag::box::details::BT_INT32  ,
    BTT_INT64   = libtbag::box::details::BT_INT64  ,
    BTT_UINT8   = libtbag::box::details::BT_UINT8  ,
    BTT_UINT16  = libtbag::box::details::BT_UINT16 ,
    BTT_UINT32  = libtbag::box::details::BT_UINT32 ,
    BTT_UINT64  = libtbag::box::details::BT_UINT64 ,
    BTT_FLOAT32 = libtbag::box::details::BT_FLOAT32,
    BTT_FLOAT64 = libtbag::box::details::BT_FLOAT64,
    // clang-format on
};

// clang-format off
using si8  = libtbag::box::details::si8;
using si16 = libtbag::box::details::si16;
using si32 = libtbag::box::details::si32;
using si64 = libtbag::box::details::si64;
using ui8  = libtbag::box::details::ui8;
using ui16 = libtbag::box::details::ui16;
using ui32 = libtbag::box::details::ui32;
using ui64 = libtbag::box::details::ui64;
using fp32 = libtbag::box::details::fp32;
using fp64 = libtbag::box::details::fp64;
// clang-format on

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
template <typename T> struct BoxTypeInfo : public BoxInfo<T, BoxTypeTable::BTT_NONE   > { /* EMPTY. */ };
template <> struct BoxTypeInfo<si8 > : public BoxInfo<si8  , BoxTypeTable::BTT_INT8   > { /* EMPTY. */ };
template <> struct BoxTypeInfo<si16> : public BoxInfo<si16 , BoxTypeTable::BTT_INT16  > { /* EMPTY. */ };
template <> struct BoxTypeInfo<si32> : public BoxInfo<si32 , BoxTypeTable::BTT_INT32  > { /* EMPTY. */ };
template <> struct BoxTypeInfo<si64> : public BoxInfo<si64 , BoxTypeTable::BTT_INT64  > { /* EMPTY. */ };
template <> struct BoxTypeInfo<ui8 > : public BoxInfo<ui8  , BoxTypeTable::BTT_UINT8  > { /* EMPTY. */ };
template <> struct BoxTypeInfo<ui16> : public BoxInfo<ui16 , BoxTypeTable::BTT_UINT16 > { /* EMPTY. */ };
template <> struct BoxTypeInfo<ui32> : public BoxInfo<ui32 , BoxTypeTable::BTT_UINT32 > { /* EMPTY. */ };
template <> struct BoxTypeInfo<ui64> : public BoxInfo<ui64 , BoxTypeTable::BTT_UINT64 > { /* EMPTY. */ };
template <> struct BoxTypeInfo<fp32> : public BoxInfo<fp32 , BoxTypeTable::BTT_FLOAT32> { /* EMPTY. */ };
template <> struct BoxTypeInfo<fp64> : public BoxInfo<fp64 , BoxTypeTable::BTT_FLOAT64> { /* EMPTY. */ };
// clang-format on

// clang-format off
template <BoxTypeTable egg_type> struct BoxEnumInfo       : public BoxInfo<void, BoxTypeTable::BTT_NONE   > { /* EMPTY. */ };
template <> struct BoxEnumInfo<BoxTypeTable::BTT_INT8   > : public BoxInfo<si8 , BoxTypeTable::BTT_INT8   > { /* EMPTY. */ };
template <> struct BoxEnumInfo<BoxTypeTable::BTT_INT16  > : public BoxInfo<si16, BoxTypeTable::BTT_INT16  > { /* EMPTY. */ };
template <> struct BoxEnumInfo<BoxTypeTable::BTT_INT32  > : public BoxInfo<si32, BoxTypeTable::BTT_INT32  > { /* EMPTY. */ };
template <> struct BoxEnumInfo<BoxTypeTable::BTT_INT64  > : public BoxInfo<si64, BoxTypeTable::BTT_INT64  > { /* EMPTY. */ };
template <> struct BoxEnumInfo<BoxTypeTable::BTT_UINT8  > : public BoxInfo<ui8 , BoxTypeTable::BTT_UINT8  > { /* EMPTY. */ };
template <> struct BoxEnumInfo<BoxTypeTable::BTT_UINT16 > : public BoxInfo<ui16, BoxTypeTable::BTT_UINT16 > { /* EMPTY. */ };
template <> struct BoxEnumInfo<BoxTypeTable::BTT_UINT32 > : public BoxInfo<ui32, BoxTypeTable::BTT_UINT32 > { /* EMPTY. */ };
template <> struct BoxEnumInfo<BoxTypeTable::BTT_UINT64 > : public BoxInfo<ui64, BoxTypeTable::BTT_UINT64 > { /* EMPTY. */ };
template <> struct BoxEnumInfo<BoxTypeTable::BTT_FLOAT32> : public BoxInfo<fp32, BoxTypeTable::BTT_FLOAT32> { /* EMPTY. */ };
template <> struct BoxEnumInfo<BoxTypeTable::BTT_FLOAT64> : public BoxInfo<fp64, BoxTypeTable::BTT_FLOAT64> { /* EMPTY. */ };
// clang-format on

// clang-format off
static_assert(BoxTypeInfo<void>::size == 0, "void size is not correct.");
static_assert(BoxTypeInfo<si8 >::size == 1,  "si8 size is not correct.");
static_assert(BoxTypeInfo<si16>::size == 2, "si16 size is not correct.");
static_assert(BoxTypeInfo<si32>::size == 4, "si32 size is not correct.");
static_assert(BoxTypeInfo<si64>::size == 8, "si64 size is not correct.");
static_assert(BoxTypeInfo<ui8 >::size == 1,  "ui8 size is not correct.");
static_assert(BoxTypeInfo<ui16>::size == 2, "ui16 size is not correct.");
static_assert(BoxTypeInfo<ui32>::size == 4, "ui32 size is not correct.");
static_assert(BoxTypeInfo<ui64>::size == 8, "ui64 size is not correct.");
static_assert(BoxTypeInfo<fp32>::size == 4, "fp32 size is not correct.");
static_assert(BoxTypeInfo<fp64>::size == 8, "fp64 size is not correct.");
// clang-format on

template <typename T>
struct is_supported_egg : public BoxTypeInfo<T>
{ /* EMPTY. */ };

template <typename T, bool is_unsigned, std::size_t size, bool is_floating>
struct __make_egg_type : public BoxTypeInfo<void>
{ /* EMPTY. */ };

// clang-format off
template <typename T, bool is_unsigned> struct __make_egg_type<T, is_unsigned, 4, true> : public BoxTypeInfo<float>  { /* EMPTY. */ };
template <typename T, bool is_unsigned> struct __make_egg_type<T, is_unsigned, 8, true> : public BoxTypeInfo<double> { /* EMPTY. */ };
// clang-format on

// clang-format off
template <typename T> struct __make_egg_type<T, false, 1, false> : public BoxTypeInfo<  int8_t> { /* EMPTY. */ };
template <typename T> struct __make_egg_type<T,  true, 1, false> : public BoxTypeInfo< uint8_t> { /* EMPTY. */ };
template <typename T> struct __make_egg_type<T, false, 2, false> : public BoxTypeInfo< int16_t> { /* EMPTY. */ };
template <typename T> struct __make_egg_type<T,  true, 2, false> : public BoxTypeInfo<uint16_t> { /* EMPTY. */ };
template <typename T> struct __make_egg_type<T, false, 4, false> : public BoxTypeInfo< int32_t> { /* EMPTY. */ };
template <typename T> struct __make_egg_type<T,  true, 4, false> : public BoxTypeInfo<uint32_t> { /* EMPTY. */ };
template <typename T> struct __make_egg_type<T, false, 8, false> : public BoxTypeInfo< int64_t> { /* EMPTY. */ };
template <typename T> struct __make_egg_type<T,  true, 8, false> : public BoxTypeInfo<uint64_t> { /* EMPTY. */ };
// clang-format on

template <typename T>
struct make_egg_type : public __make_egg_type<T, std::is_unsigned<T>::value, sizeof(T), std::is_floating_point<T>::value>
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

/**
 * Box class prototype.
 *
 * Containers:
 * <table>
 *   <tr>
 *     <th>NumPy</th>
 *     <th>Box</th>
 *   </tr>
 *   <tr>
 *     <td><code>a = np.array([[1, 2], [3, 4], [5, 6]])</code></td>
 *     <td><code>Box b = {{1, 2}, {3, 4}, {5, 6}};</code></td>
 *   </tr>
 *   <tr>
 *     <td><code>a.resize([2, 3])</code></td>
 *     <td><code>a.resize(2, 3)</code></td>
 *   </tr>
 *   <tr>
 *     <td><code>a.astype(np.double)</code></td>
 *     <td><code>b.astype<double>()</code></td>
 *   </tr>
 * </table>
 *
 * @author zer0
 * @date   2019-05-16
 */
class TBAG_API Box
{
public:
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

    using btype = libtbag::box::details::btype;
    using bdev  = libtbag::box::details::bdev;

    using box_data   = libtbag::box::details::box_data;
    using box_cursor = libtbag::box::details::box_cursor;
    using box_any    = libtbag::box::details::box_any;

    using box_opaque_delete_cb = libtbag::box::details::box_opaque_delete_cb;

    using Buffer = std::vector<ui8>;

    using size_type = ui32;
    using difference_type = std::ptrdiff_t;

    using SharedBoxData = std::shared_ptr<box_data>;

public:
    TBAG_CONSTEXPR static int const nop = libtbag::box::details::box_nop;

    TBAG_CONSTEXPR static btype const type_none() TBAG_NOEXCEPT { return get_btype<void>(); }
    TBAG_CONSTEXPR static btype const type_si8 () TBAG_NOEXCEPT { return get_btype<si8 >(); }
    TBAG_CONSTEXPR static btype const type_si16() TBAG_NOEXCEPT { return get_btype<si16>(); }
    TBAG_CONSTEXPR static btype const type_si32() TBAG_NOEXCEPT { return get_btype<si32>(); }
    TBAG_CONSTEXPR static btype const type_si64() TBAG_NOEXCEPT { return get_btype<si64>(); }
    TBAG_CONSTEXPR static btype const type_ui8 () TBAG_NOEXCEPT { return get_btype<ui8 >(); }
    TBAG_CONSTEXPR static btype const type_ui16() TBAG_NOEXCEPT { return get_btype<ui16>(); }
    TBAG_CONSTEXPR static btype const type_ui32() TBAG_NOEXCEPT { return get_btype<ui32>(); }
    TBAG_CONSTEXPR static btype const type_ui64() TBAG_NOEXCEPT { return get_btype<ui64>(); }
    TBAG_CONSTEXPR static btype const type_fp32() TBAG_NOEXCEPT { return get_btype<fp32>(); }
    TBAG_CONSTEXPR static btype const type_fp64() TBAG_NOEXCEPT { return get_btype<fp64>(); }

    TBAG_CONSTEXPR static bdev const device_none() TBAG_NOEXCEPT { return libtbag::box::details::BD_NONE; }
    TBAG_CONSTEXPR static bdev const device_cpu () TBAG_NOEXCEPT { return libtbag::box::details::BD_CPU ; }
    TBAG_CONSTEXPR static bdev const device_cuda() TBAG_NOEXCEPT { return libtbag::box::details::BD_CUDA; }
    TBAG_CONSTEXPR static bdev const device_cl  () TBAG_NOEXCEPT { return libtbag::box::details::BD_CL  ; }

private:
    SharedBoxData _data;

public:
    Box();
    explicit Box(std::nullptr_t) TBAG_NOEXCEPT;
    explicit Box(box_data && data) TBAG_NOEXCEPT;
    Box(Box const & obj) TBAG_NOEXCEPT;
    Box(Box && obj) TBAG_NOEXCEPT;
    ~Box();

public:
    template <typename T>
    Box(std::initializer_list<T> const & items) : Box()
    {
        auto const code = assign<T>(items);
        if (isFailure(code)) {
            throw ErrException(code);
        }
    }

    template <typename T>
    Box(std::initializer_list< std::initializer_list<T> > const & items) : Box()
    {
        auto const code = assign<T>(items);
        if (isFailure(code)) {
            throw ErrException(code);
        }
    }

    template <typename T>
    Box(std::initializer_list< std::initializer_list< std::initializer_list<T> > > const & items) : Box()
    {
        auto const code = assign<T>(items);
        if (isFailure(code)) {
            throw ErrException(code);
        }
    }

    template <typename T>
    Box(std::initializer_list< std::initializer_list< std::initializer_list< std::initializer_list<T> > > > const & items) : Box()
    {
        auto const code = assign<T>(items);
        if (isFailure(code)) {
            throw ErrException(code);
        }
    }

public:
    Box & operator =(Box const & obj) TBAG_NOEXCEPT;
    Box & operator =(Box && obj) TBAG_NOEXCEPT;
    Box & operator =(std::nullptr_t) TBAG_NOEXCEPT;

    inline void swap(Box & obj) TBAG_NOEXCEPT
    {
        if (this != &obj) {
            _data.swap(obj._data);
        }
    }

    inline friend void swap(Box & lh, Box & rh) TBAG_NOEXCEPT
    {
        lh.swap(rh);
    }

public:
    inline bool exists() const TBAG_NOEXCEPT
    {
        return static_cast<bool>(_data);
    }

    inline operator bool() const TBAG_NOEXCEPT
    {
        return exists();
    }

    void reset() TBAG_NOEXCEPT_SP_OP(_data.reset())
    {
        _data.reset();
    }

public:
    inline box_data       * base()       TBAG_NOEXCEPT { return _data.get(); }
    inline box_data const * base() const TBAG_NOEXCEPT { return _data.get(); }

public:
    inline btype getType() const TBAG_NOEXCEPT
    {
        if (_data) {
            return _data->type;
        }
        return type_none();
    }

    inline btype type() const TBAG_NOEXCEPT
    {
        assert(exists());
        return _data->type;
    }

    inline bool isSupportType() const TBAG_NOEXCEPT
    {
        return libtbag::box::details::box_support_type(getType());
    }

    inline char const * getTypeName() const TBAG_NOEXCEPT
    {
        return libtbag::box::details::box_get_type_name(getType());
    }

    inline ui32 getTypeByte() const TBAG_NOEXCEPT
    {
        return libtbag::box::details::box_get_type_byte(getType());
    }

    // clang-format off
    inline bool is_none() const TBAG_NOEXCEPT { return getType() == type_none(); }
    inline bool is_si8 () const TBAG_NOEXCEPT { return getType() == type_si8 (); }
    inline bool is_si16() const TBAG_NOEXCEPT { return getType() == type_si16(); }
    inline bool is_si32() const TBAG_NOEXCEPT { return getType() == type_si32(); }
    inline bool is_si64() const TBAG_NOEXCEPT { return getType() == type_si64(); }
    inline bool is_ui8 () const TBAG_NOEXCEPT { return getType() == type_ui8 (); }
    inline bool is_ui16() const TBAG_NOEXCEPT { return getType() == type_ui16(); }
    inline bool is_ui32() const TBAG_NOEXCEPT { return getType() == type_ui32(); }
    inline bool is_ui64() const TBAG_NOEXCEPT { return getType() == type_ui64(); }
    inline bool is_fp32() const TBAG_NOEXCEPT { return getType() == type_fp32(); }
    inline bool is_fp64() const TBAG_NOEXCEPT { return getType() == type_fp64(); }
    // clang-format on

    inline bool is_signed() const TBAG_NOEXCEPT
    {
        return is_si8() || is_si16() || is_si32() || is_si64();
    }

    inline bool is_unsigned() const TBAG_NOEXCEPT
    {
        return is_ui8() || is_ui16() || is_ui32() || is_ui64();
    }

    inline bool is_floating() const TBAG_NOEXCEPT
    {
        return is_fp32() || is_fp64();
    }

public:
    inline btype getDevice() const TBAG_NOEXCEPT
    {
        if (_data) {
            return _data->device;
        }
        return device_none();
    }

    inline btype device() const TBAG_NOEXCEPT
    {
        assert(exists());
        return _data->device;
    }

    inline bool isSupportDevice() const TBAG_NOEXCEPT
    {
        return libtbag::box::details::box_support_device(getDevice());
    }

    inline char const * getDeviceName(bdev dev) const TBAG_NOEXCEPT
    {
        return libtbag::box::details::box_get_device_name(getType());
    }

    // clang-format off
    inline bool is_device_none() const TBAG_NOEXCEPT { return getDevice() == device_none(); }
    inline bool is_device_cpu () const TBAG_NOEXCEPT { return getDevice() == device_cpu (); }
    inline bool is_device_cuda() const TBAG_NOEXCEPT { return getDevice() == device_cuda(); }
    inline bool is_device_cl  () const TBAG_NOEXCEPT { return getDevice() == device_cl  (); }
    // clang-format on

public:
    inline ui64 const * getExtensions() const TBAG_NOEXCEPT
    {
        if (_data) {
            return _data->ext;
        }
        return nullptr;
    }

    inline ui64 const * ext() const TBAG_NOEXCEPT
    {
        assert(exists());
        return _data->ext;
    }

    // clang-format off
    inline ui64 getExtension0() const TBAG_NOEXCEPT { if (_data) { return _data->ext[0]; } return 0; }
    inline ui64 getExtension1() const TBAG_NOEXCEPT { if (_data) { return _data->ext[1]; } return 0; }
    inline ui64 getExtension2() const TBAG_NOEXCEPT { if (_data) { return _data->ext[2]; } return 0; }
    inline ui64 getExtension3() const TBAG_NOEXCEPT { if (_data) { return _data->ext[3]; } return 0; }
    inline ui64 ext0() const TBAG_NOEXCEPT { assert(exists()); return _data->ext[0]; }
    inline ui64 ext1() const TBAG_NOEXCEPT { assert(exists()); return _data->ext[1]; }
    inline ui64 ext2() const TBAG_NOEXCEPT { assert(exists()); return _data->ext[2]; }
    inline ui64 ext3() const TBAG_NOEXCEPT { assert(exists()); return _data->ext[3]; }
    // clang-format on

    inline bool testExtensions(ui64 e0, ui64 e1, ui64 e2, ui64 e3) const TBAG_NOEXCEPT
    {
        if (_data) {
            return _data->ext[0] == e0 &&
                   _data->ext[1] == e1 &&
                   _data->ext[2] == e2 &&
                   _data->ext[3] == e3;
        }
        return false;
    }

public:
    template <typename T>
    inline void * getData() TBAG_NOEXCEPT
    {
        if (_data) {
            return static_cast<T*>(_data->data);
        }
        return nullptr;
    }

    template <typename T>
    inline void const * getData() const TBAG_NOEXCEPT
    {
        if (_data) {
            return static_cast<T*>(_data->data);
        }
        return nullptr;
    }

    inline void * getData() TBAG_NOEXCEPT
    {
        if (_data) {
            return _data->data;
        }
        return nullptr;
    }

    inline void const * getData() const TBAG_NOEXCEPT
    {
        if (_data) {
            return _data->data;
        }
        return nullptr;
    }

    template <typename T>
    inline T * data() TBAG_NOEXCEPT
    {
        assert(exists());
        return static_cast<T*>(_data->data);
    }

    template <typename T>
    inline T const * data() const TBAG_NOEXCEPT
    {
        assert(exists());
        return static_cast<T const *>(_data->data);
    }

    inline void * data() TBAG_NOEXCEPT
    {
        assert(exists());
        return _data->data;
    }

    inline void const * data() const TBAG_NOEXCEPT
    {
        assert(exists());
        return _data->data;
    }

    inline ui32 getTotalDataByte() const TBAG_NOEXCEPT
    {
        if (_data) {
            return _data->total_data_byte;
        }
        return 0;
    }

    inline ui32 total_data_byte() const TBAG_NOEXCEPT
    {
        assert(exists());
        return _data->total_data_byte;
    }

    inline ui32 getCapacity() const TBAG_NOEXCEPT
    {
        return getTotalDataByte();
    }

    inline ui32 capacity() const TBAG_NOEXCEPT
    {
        return total_data_byte();
    }

    inline ui32 getSize() const TBAG_NOEXCEPT
    {
        if (_data) {
            return _data->size;
        }
        return 0;
    }

    inline ui32 size() const TBAG_NOEXCEPT
    {
        assert(exists());
        return _data->size;
    }

    inline bool isEmpty() const TBAG_NOEXCEPT
    {
        if (_data) {
            return _data->size == 0;
        }
        return true;
    }

    inline bool empty() const TBAG_NOEXCEPT
    {
        assert(exists());
        return _data->size == 0;
    }

public:
    inline ui32 const * getDimensions() const TBAG_NOEXCEPT
    {
        if (_data) {
            return _data->dims;
        }
        return nullptr;
    }

    inline ui32 const * dims() const TBAG_NOEXCEPT
    {
        assert(exists());
        return _data->dims;
    }

    inline ui32 getDimension(ui32 i) const TBAG_NOEXCEPT
    {
        if (_data && i < _data->rank) {
            return _data->dims[i];
        }
        return 0;
    }

    inline ui32 dim(ui32 i) const TBAG_NOEXCEPT
    {
        assert(exists());
        return _data->dims[i];
    }

    inline ui32 getTotalDimsByte() const TBAG_NOEXCEPT
    {
        if (_data) {
            return _data->total_dims_byte;
        }
        return 0;
    }

    inline ui32 total_dims_byte() const TBAG_NOEXCEPT
    {
        assert(exists());
        return _data->total_dims_byte;
    }

    inline ui32 getDimensionsCapacity() const TBAG_NOEXCEPT
    {
        return getTotalDimsByte();
    }

    inline ui32 dims_capacity() const TBAG_NOEXCEPT
    {
        return total_dims_byte();
    }

    inline ui32 getRank() const TBAG_NOEXCEPT
    {
        if (_data) {
            return _data->rank;
        }
        return 0;
    }

    inline ui32 rank() const TBAG_NOEXCEPT
    {
        assert(exists());
        return _data->rank;
    }

    inline ui32 getStride(ui32 i) const TBAG_NOEXCEPT
    {
        return libtbag::box::details::box_dim_get_stride(getDimensions(), getRank(), i);
    }

    inline ui32 getStrideByte(ui32 i) const TBAG_NOEXCEPT
    {
        return getStride(i) * getTypeByte();
    }

public:
    inline ui8 * getInfo() TBAG_NOEXCEPT
    {
        if (_data) {
            return _data->info;
        }
        return nullptr;
    }

    inline ui8 const * getInfo() const TBAG_NOEXCEPT
    {
        if (_data) {
            return _data->info;
        }
        return nullptr;
    }

    inline ui8 * info() TBAG_NOEXCEPT
    {
        assert(exists());
        return _data->info;
    }

    inline ui8 const * info() const TBAG_NOEXCEPT
    {
        assert(exists());
        return _data->info;
    }

    inline ui32 getTotalInfoByte() const TBAG_NOEXCEPT
    {
        if (_data) {
            return _data->total_info_byte;
        }
        return 0;
    }

    inline ui32 total_info_byte() const TBAG_NOEXCEPT
    {
        assert(exists());
        return _data->total_info_byte;
    }

    inline ui32 getInfoCapacity() const TBAG_NOEXCEPT
    {
        return getTotalInfoByte();
    }

    inline ui32 info_capacity() const TBAG_NOEXCEPT
    {
        return total_info_byte();
    }

    inline ui32 getInfoSize() const TBAG_NOEXCEPT
    {
        if (_data) {
            return _data->info_size;
        }
        return 0;
    }

    inline ui32 info_size() const TBAG_NOEXCEPT
    {
        assert(exists());
        return _data->info_size;
    }

public:
    inline void setOpaque(void * v) const TBAG_NOEXCEPT
    {
        if (_data) {
            _data->set_opaque(v);
        }
    }

    template <typename T>
    inline void setOpaque(T v) TBAG_NOEXCEPT
    {
        if (_data) {
            _data->set_opaque(v);
        }
    }

    template <typename T>
    inline void getOpaque(T * v) const TBAG_NOEXCEPT
    {
        if (_data && v != nullptr) {
            _data->get_opaque(v);
        }
    }

    template <typename T>
    inline T getOpaque() const TBAG_NOEXCEPT
    {
        T result = T();
        getOpaque(&result);
        return result;
    }

    inline void * getOpaquePointer() const TBAG_NOEXCEPT
    {
        if (_data) {
            return _data->get_opaque_pointer();
        }
        return nullptr;
    }

    inline box_opaque_delete_cb getOpaqueDeleter() const TBAG_NOEXCEPT
    {
        if (_data) {
            return _data->opaque_deleter;
        }
        return nullptr;
    }

    inline void setOpaqueDeleter(box_opaque_delete_cb cb) TBAG_NOEXCEPT
    {
        if (_data) {
            _data->opaque_deleter = cb;
        }
    }

public:
    inline void clearData() TBAG_NOEXCEPT
    {
        if (_data) {
            _data->size = 0;
            _data->rank = 0;
        }
    }

    inline void clearInfo() TBAG_NOEXCEPT
    {
        if (_data) {
            _data->info_size = 0;
        }
    }

    inline void clear() TBAG_NOEXCEPT
    {
        clearData();
        clearInfo();
    }

public:
    void createIfNotExists();

public:
    void setInfo(ui8 const * info, ui32 size);
    void setInfo(std::string const & info);
    void setInfo(Buffer const & info);

    std::string getInfoString() const;
    Buffer getInfoBuffer() const;

public:
    struct shape_unknown_t  { /* EMPTY. */ };
    struct shape_args1_t    { /* EMPTY. */ };
    struct shape_args2_t    { /* EMPTY. */ };
    struct shape_dims1_t    { /* EMPTY. */ };
    struct shape_dims2_t    { /* EMPTY. */ };
    struct shape_ref_box1_t { /* EMPTY. */ };
    struct shape_ref_box2_t { /* EMPTY. */ };

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
        using type =
                typename std::conditional<is_shape_ref_box1, shape_ref_box1_t,
                typename std::conditional<is_shape_ref_box2, shape_ref_box2_t,
                typename std::conditional<is_shape_dims1, shape_dims1_t,
                typename std::conditional<is_shape_dims2, shape_dims2_t,
                typename std::conditional<is_shape_args1, shape_args1_t,
                typename std::conditional<is_shape_args2, shape_args2_t,
                shape_unknown_t
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
        using type =
                typename std::conditional<is_shape_dims1, shape_dims1_t,
                typename std::conditional<is_shape_dims2, shape_dims2_t,
                typename std::conditional<is_shape_args1, shape_args1_t,
                typename std::conditional<is_shape_args2, shape_args2_t,
                shape_unknown_t
                >::type // shape_args2_t
                >::type // shape_args1_t
                >::type // shape_dims2_t
                >::type;// shape_dims1_t
        // clang-format on

        TBAG_CONSTEXPR static type const value = { /* EMPTY. */ };
    };

private:
    Err _resize_args(btype type, bdev device, ui64 const * ext, ui32 rank, ...);
    Err _resize_args(btype type, ui32 rank, ...);

    Err _resize_vargs(btype type, bdev device, ui64 const * ext, ui32 rank, va_list ap);
    Err _resize_vargs(btype type, ui32 rank, va_list ap);

    Err _resize_dims(btype type, bdev device, ui64 const * ext, ui32 rank, ui32 const * dims);
    Err _resize_dims(btype type, ui32 rank, ui32 const * dims);

    Err _resize_ref_box(btype type, box_data const * reference_box);
    Err _resize_ref_box(btype type, Box const & reference_box);

private:
    template <typename ... Args>
    Err _resize(shape_unknown_t, Args && ... args)
    {
        return libtbag::E_INACCESSIBLE_BLOCK;
    }

    template <typename ... Args>
    Err _resize(shape_args1_t, btype type, bdev device, ui64 const * ext, Args ... args)
    {
        static_assert(static_cast<ui32>(sizeof...(Args)) >= 1u, "At least one Args is required.");
        return _resize_args(type, device, ext, static_cast<ui32>(sizeof...(Args)), std::forward<Args>(args) ...);
    }

    template <typename ... Args>
    Err _resize(shape_args2_t, btype type, Args ... args)
    {
        static_assert(static_cast<ui32>(sizeof...(Args)) >= 1u, "At least one Args is required.");
        return _resize_args(type, static_cast<ui32>(sizeof...(Args)), std::forward<Args>(args) ...);
    }

    Err _resize(shape_dims1_t, btype type, bdev device, ui64 const * ext, ui32 rank, ui32 const * dims)
    {
        return _resize_dims(type, device, ext, rank, dims);
    }

    Err _resize(shape_dims2_t, btype type, ui32 rank, ui32 const * dims)
    {
        return _resize_dims(type, rank, dims);
    }

    Err _resize(shape_ref_box1_t, btype type, box_data const * reference_box)
    {
        return _resize_ref_box(type, reference_box);
    }

    Err _resize(shape_ref_box2_t, btype type, Box const & reference_box)
    {
        return _resize_ref_box(type, reference_box);
    }

public:
    /**
     * Coordinate the {Header}, {Dims}, and {DataSize} sections.
     *
     * @see libtbag::box::details::box_data::resize_dims
     */
    template <typename ... Args>
    Err resize(btype type, Args ... args)
    {
        static_assert(static_cast<ui32>(sizeof...(Args)) >= 1u, "At least one Args is required.");
        return _resize(shape_selector<Args...>::value, type, std::forward<Args>(args) ...);
    }

    template <typename T, typename ... Args>
    Err resize(Args ... args)
    {
        return resize(get_btype<T>(), std::forward<Args>(args) ...);
    }

    template <typename ... Args>
    static Box array(btype type, Args ... args)
    {
        static_assert(static_cast<ui32>(sizeof...(Args)) >= 1u, "At least one Args is required.");
        Box result;
        if (isFailure(result.resize(type, std::forward<Args>(args) ...))) {
            return Box(nullptr);
        }
        return result;
    }

    template <typename T, typename ... Args>
    static Box array(Args ... args)
    {
        return array(get_btype<T>(), std::forward<Args>(args) ...);
    }

public:
    Box astype(btype type) const;

    template <typename T>
    Box astype() const
    {
        return astype(get_btype<T>());
    }

public:
    Err copyFromData(Box const & box);
    Err copyToData(Box & box) const;

public:
    Err copyFromInfo(Box const & box);
    Err copyToInfo(Box & box) const;

public:
    Err copyFrom(Box const & box);
    Err copyTo(Box & box) const;

public:
    Box clone() const;

public:
    template <typename T>
    using init1d_t = std::initializer_list<T>;
    template <typename T>
    using init2d_t = std::initializer_list< init1d_t<T> >;
    template <typename T>
    using init3d_t = std::initializer_list< init2d_t<T> >;
    template <typename T>
    using init4d_t = std::initializer_list< init3d_t<T> >;

    template <typename T>
    Err assign(bdev device, ui64 const * ext, T const * begin, T const * end)
    {
        using DataType = typename libtbag::remove_cr<T>::type;
        auto const dim_1d = static_cast<ui32>(std::distance(begin, end));
        auto const type = get_btype<DataType>();
        auto const code = _resize_args(type, device, ext, 1, dim_1d);
        if (isFailure(code)) {
            return code;
        }

        if (is_device_cpu() && type == device_cpu()) {
            auto * d = data<DataType>();
            for (; begin != end; ++begin) {
                *d = static_cast<DataType>(*begin);
                ++d;
            }
        } else {
            ui32 offset = 0;
            for (; begin != end; ++begin) {
                _data->set_data(begin, type, device, ext, offset);
                ++offset;
            }
        }
        return libtbag::E_SUCCESS;
    }

    template <typename T>
    Err assign(T const * begin, T const * end)
    {
        return assign(device_cpu(), nullptr, begin, end);
    }

    template <typename T>
    Err assign(bdev device, ui64 const * ext, init1d_t<T> const & items)
    {
        return assign(device, ext, items.begin(), items.end());
    }

    template <typename T>
    Err assign(bdev device, ui64 const * ext, init2d_t<T> const & items)
    {
        using DataType = typename libtbag::remove_cr<T>::type;
        auto const dim_1d = static_cast<ui32>(items.size());
        auto const dim_2d = static_cast<ui32>(items.begin()->size());
        auto const type = get_btype<DataType>();
        auto const code = _resize_args(type, device, ext, 2, dim_1d, dim_2d);
        if (isFailure(code)) {
            return code;
        }

        if (is_device_cpu() && type == device_cpu()) {
            auto * d = data<DataType>();
            for (auto & i1 : items) {
                assert(i1.size() == dim_2d);
                for (auto & i2 : i1) {
                    *d = static_cast<DataType>(i2);
                    ++d;
                }
            }
        } else {
            ui32 offset = 0;
            for (auto & i1 : items) {
                assert(i1.size() == dim_2d);
                for (auto & i2 : i1) {
                    _data->set_data(&i2, type, device, ext, offset);
                    ++offset;
                }
            }
        }
        return libtbag::E_SUCCESS;
    }

    template <typename T>
    Err assign(bdev device, ui64 const * ext, init3d_t<T> const & items)
    {
        using DataType = typename libtbag::remove_cr<T>::type;
        auto const dim_1d = static_cast<ui32>(items.size());
        auto const dim_2d = static_cast<ui32>(items.begin()->size());
        auto const dim_3d = static_cast<ui32>(items.begin()->begin()->size());
        auto const type = get_btype<DataType>();
        auto const code = _resize_args(type, device, ext, 3, dim_1d, dim_2d, dim_3d);
        if (isFailure(code)) {
            return code;
        }

        if (is_device_cpu() && type == device_cpu()) {
            auto * d = data<DataType>();
            for (auto & i1 : items) {
                assert(i1.size() == dim_2d);
                for (auto & i2 : i1) {
                    assert(i2.size() == dim_3d);
                    for (auto & i3 : i2) {
                        *d = static_cast<DataType>(i3);
                        ++d;
                    }
                }
            }
        } else {
            ui32 offset = 0;
            for (auto & i1 : items) {
                assert(i1.size() == dim_2d);
                for (auto & i2 : i1) {
                    assert(i2.size() == dim_3d);
                    for (auto & i3 : i2) {
                        _data->set_data(&i3, type, device, ext, offset);
                        ++offset;
                    }
                }
            }
        }
        return libtbag::E_SUCCESS;
    }

    template <typename T>
    Err assign(bdev device, ui64 const * ext, init4d_t<T> const & items)
    {
        using DataType = typename libtbag::remove_cr<T>::type;
        auto const dim_1d = static_cast<ui32>(items.size());
        auto const dim_2d = static_cast<ui32>(items.begin()->size());
        auto const dim_3d = static_cast<ui32>(items.begin()->begin()->size());
        auto const dim_4d = static_cast<ui32>(items.begin()->begin()->begin()->size());
        auto const type = get_btype<DataType>();
        auto const code = _resize_args(type, device, ext, 4, dim_1d, dim_2d, dim_3d, dim_4d);
        if (isFailure(code)) {
            return code;
        }

        if (is_device_cpu() && type == device_cpu()) {
            auto * d = data<DataType>();
            for (auto & i1 : items) {
                assert(i1.size() == dim_2d);
                for (auto & i2 : i1) {
                    assert(i2.size() == dim_3d);
                    for (auto & i3 : i2) {
                        assert(i3.size() == dim_4d);
                        for (auto & i4 : i3) {
                            *d = static_cast<DataType>(i4);
                            ++d;
                        }
                    }
                }
            }
        } else {
            ui32 offset = 0;
            for (auto & i1 : items) {
                assert(i1.size() == dim_2d);
                for (auto & i2 : i1) {
                    assert(i2.size() == dim_3d);
                    for (auto & i3 : i2) {
                        assert(i3.size() == dim_4d);
                        for (auto & i4 : i3) {
                            _data->set_data(&i4, type, device, ext, offset);
                            ++offset;
                        }
                    }
                }
            }
        }
        return libtbag::E_SUCCESS;
    }

    // clang-format off
    template <typename T> Err assign(init1d_t<T> const & items) { return assign(device_cpu(), nullptr, items); }
    template <typename T> Err assign(init2d_t<T> const & items) { return assign(device_cpu(), nullptr, items); }
    template <typename T> Err assign(init3d_t<T> const & items) { return assign(device_cpu(), nullptr, items); }
    template <typename T> Err assign(init4d_t<T> const & items) { return assign(device_cpu(), nullptr, items); }
    // clang-format on

public:
    template <typename T>
    inline T & offset(ui32 i)
    {
        assert(exists());
        assert(i < size());
        assert(is_btype_equals<T>(type()));
        return *((T*)_data->get_data_ptr_by_offset(i));
    }

    template <typename T>
    inline T const & offset(ui32 i) const
    {
        assert(exists());
        assert(i < size());
        assert(is_btype_equals<T>(type()));
        return *((T*)_data->get_data_ptr_by_offset(i));
    }

    template <typename T, typename ... Args>
    inline T & at(Args && ... args)
    {
        static_assert(static_cast<ui32>(sizeof...(Args)) >= 1u, "At least one Args is required.");
        return offset<T>(libtbag::box::details::box_dim_get_offset_args(
                dims(), static_cast<ui32>(sizeof...(Args)), std::forward<Args>(args) ...));
    }

    template <typename T, typename ... Args>
    inline T const & at(Args && ... args) const
    {
        static_assert(static_cast<ui32>(sizeof...(Args)) >= 1u, "At least one Args is required.");
        return offset<T>(libtbag::box::details::box_dim_get_offset_args(
                dims(), static_cast<ui32>(sizeof...(Args)), std::forward<Args>(args) ...));
    }

private:
    template <typename T, typename ... Args>
    Err _get(shape_unknown_t, T * result, Args && ... args)
    {
        return libtbag::E_INACCESSIBLE_BLOCK;
    }

    template <typename T, typename ... Args>
    Err _get(shape_args1_t, T * out, bdev out_device, ui64 const * out_ext, Args ... args) const
    {
        static_assert(static_cast<ui32>(sizeof...(Args)) >= 1u, "At least one Args is required.");
        if (!exists()) {
            return libtbag::E_EXPIRED;
        }
        return _data->get_data_args((void*)out, get_btype<T>(), out_device, out_ext,
                                    static_cast<ui32>(sizeof...(Args)),
                                    std::forward<Args>(args) ...);
    }

    template <typename T, typename ... Args>
    Err _get(shape_args2_t, T * out, Args ... args) const
    {
        static_assert(static_cast<ui32>(sizeof...(Args)) >= 1u, "At least one Args is required.");
        if (!exists()) {
            return libtbag::E_EXPIRED;
        }
        auto const shape_device = is_device_none() ? device_cpu() : device();
        ui64 const shape_ext[TBAG_BOX_EXT_SIZE] = { ext0(), ext1(), ext2(), ext3() };
        return _data->get_data_args((void*)out, get_btype<T>(), shape_device, shape_ext,
                                    static_cast<ui32>(sizeof...(Args)),
                                    std::forward<Args>(args) ...);
    }

    template <typename T>
    Err _get(shape_dims1_t, T * out, bdev out_device, ui64 const * out_ext,
             ui32 index_rank, ui32 const * index_dims) const
    {
        if (!exists()) {
            return libtbag::E_EXPIRED;
        }
        return _data->get_data_dims((void*)out, get_btype<T>(), out_device, out_ext, index_rank, index_dims);
    }

    template <typename T>
    Err _get(shape_dims2_t, T * out, ui32 index_rank, ui32 const * index_dims) const
    {
        if (!exists()) {
            return libtbag::E_EXPIRED;
        }
        auto const shape_device = is_device_none() ? device_cpu() : device();
        ui64 const shape_ext[TBAG_BOX_EXT_SIZE] = { ext0(), ext1(), ext2(), ext3() };
        return _data->get_data_dims((void*)out, get_btype<T>(), shape_device, shape_ext, index_rank, index_dims);
    }

public:
    template <typename T, typename ... Args>
    Err get(T * out, Args ... args) const
    {
        static_assert(static_cast<ui32>(sizeof...(Args)) >= 1u, "At least one Args is required.");
        return _get<T>(index_selector<Args...>::value, out, std::forward<Args>(args) ...);
    }

    template <typename T, typename ... Args>
    T get(Args ... args) const
    {
        static_assert(static_cast<ui32>(sizeof...(Args)) >= 1u, "At least one Args is required.");
        T out = T();
        _get<T>(index_selector<Args...>::value, &out, std::forward<Args>(args) ...);
        return out;
    }

private:
    template <typename T, typename ... Args>
    Err _set(shape_unknown_t, T const * src, Args && ... args)
    {
        return libtbag::E_INACCESSIBLE_BLOCK;
    }

    template <typename T, typename ... Args>
    Err _set(shape_args1_t, T const * src, bdev src_device, ui64 const * src_ext, Args ... args)
    {
        static_assert(static_cast<ui32>(sizeof...(Args)) >= 1u, "At least one Args is required.");
        if (!exists()) {
            return libtbag::E_EXPIRED;
        }
        return _data->set_data_args((void const *)src, get_btype<T>(), src_device, src_ext,
                                    static_cast<ui32>(sizeof...(Args)),
                                    std::forward<Args>(args) ...);
    }

    template <typename T, typename ... Args>
    Err _set(shape_args1_t, T const & src, bdev src_device, ui64 const * src_ext, Args ... args)
    {
        return _set(shape_args1_t{}, (T const *)&src, src_device, src_ext, std::forward<Args>(args) ...);
    }

    template <typename T, typename ... Args>
    Err _set(shape_args2_t, T const * src, Args ... args)
    {
        static_assert(static_cast<ui32>(sizeof...(Args)) >= 1u, "At least one Args is required.");
        if (!exists()) {
            return libtbag::E_EXPIRED;
        }
        auto const shape_device = is_device_none() ? device_cpu() : device();
        ui64 const shape_ext[TBAG_BOX_EXT_SIZE] = { ext0(), ext1(), ext2(), ext3() };
        return _data->set_data_args((void const *)src, get_btype<T>(), shape_device, shape_ext,
                                    static_cast<ui32>(sizeof...(Args)),
                                    std::forward<Args>(args) ...);
    }

    template <typename T, typename ... Args>
    Err _set(shape_args2_t, T const & src, Args ... args)
    {
        return _set(shape_args2_t{}, (T const *)&src, std::forward<Args>(args) ...);
    }

    template <typename T>
    Err _set(shape_dims1_t, T const * src, bdev src_device, ui64 const * src_ext,
             ui32 index_rank, ui32 const * index_dims)
    {
        if (!exists()) {
            return libtbag::E_EXPIRED;
        }
        return _data->set_data_dims((void const *)src, get_btype<T>(), src_device, src_ext, index_rank, index_dims);
    }

    template <typename T>
    Err _set(shape_dims1_t, T const & src, bdev src_device, ui64 const * src_ext,
             ui32 index_rank, ui32 const * index_dims)
    {
        return _set(shape_dims1_t{}, (T const *)&src, src_device, src_ext, index_rank, index_dims);
    }

    template <typename T>
    Err _set(shape_dims2_t, T const * src, ui32 index_rank, ui32 const * index_dims)
    {
        if (!exists()) {
            return libtbag::E_EXPIRED;
        }
        auto const shape_device = is_device_none() ? device_cpu() : device();
        ui64 const shape_ext[TBAG_BOX_EXT_SIZE] = { ext0(), ext1(), ext2(), ext3() };
        return _data->set_data_dims((void const *)src, get_btype<T>(), shape_device, shape_ext, index_rank, index_dims);
    }

    template <typename T>
    Err _set(shape_dims2_t, T const & src, ui32 index_rank, ui32 const * index_dims)
    {
        return _set(shape_dims2_t{}, (T const *)&src, index_rank, index_dims);
    }

public:
    template <typename T, typename ... Args>
    Err set(T const * src, Args ... args)
    {
        static_assert(static_cast<ui32>(sizeof...(Args)) >= 1u, "At least one Args is required.");
        return _set<T>(index_selector<Args...>::value, src, std::forward<Args>(args) ...);
    }

    template <typename T, typename ... Args>
    Err set(T const & src, Args ... args)
    {
        return set((T const *)&src, std::forward<Args>(args) ...);
    }

public:
    Err setData(ui8 const * info, ui32 size);
    Err setData(std::string const & info);
    Err setData(Buffer const & info);

public:
    Err getDataString(std::string & result) const;
    std::string getDataString() const;

public:
    using Builder = libtbag::box::BoxPacketBuilder;
    using Parser  = libtbag::box::BoxPacketParser;
    using Packet  = libtbag::box::BoxPacket;

public:
    Err encode(Builder & builder) const;
    Err encode(Builder & builder, Buffer & buffer) const;
    Err encode(Buffer & buffer) const;

public:
    Err decode(void const * buffer, std::size_t size, Parser const & parser, std::size_t * computed_size = nullptr);
    Err decode(void const * buffer, std::size_t size, std::size_t * computed_size = nullptr);
    Err decode(Buffer const & buffer, std::size_t * computed_size = nullptr);

public:
    Err encodeToJson(Builder & builder, std::string & json) const;
    Err encodeToJson(std::string & json) const;

public:
    Err decodeFromJson(char const * json, std::size_t size, Parser const & parser);
    Err decodeFromJson(char const * json, std::size_t size);
    Err decodeFromJson(std::string const & json);

public:
    std::string toJsonText(Err * code = nullptr) const;
    bool fromJsonText(std::string const & json, Err * code = nullptr);

public:
    class Cursor;

    struct begin_tag_t { /* EMPTY. */ };
    struct end_tag_t { /* EMPTY. */ };

    TBAG_CONSTEXPR static begin_tag_t const begin_tag = { /* EMPTY. */ };
    TBAG_CONSTEXPR static end_tag_t const end_tag = { /* EMPTY. */ };

    /**
     * Iterator class.
     *
     * @author zer0
     * @date   2019-12-16
     */
    template <typename CursorT, typename T>
    class Iterator
    {
    public:
        using iterator_category = std::forward_iterator_tag;

    public:
        using CursorType = CursorT;
        using DataType = T;

    private:
        /** Base cursor. */
        CursorType & _cursor;

        /** Base pointer. */
        DataType * _data;

    public:
        Iterator(begin_tag_t, CursorType & cursor) TBAG_NOEXCEPT : _cursor(cursor), _data((DataType*)cursor.getBegin())
        { /* EMPTY. */ }
        Iterator(end_tag_t, CursorType & cursor) TBAG_NOEXCEPT : _cursor(cursor), _data((DataType*)cursor.getEnd())
        { /* EMPTY. */ }

        Iterator(Iterator const & obj) TBAG_NOEXCEPT : _cursor(obj._cursor), _data(obj._data)
        { /* EMPTY. */ }
        Iterator(Iterator && obj) TBAG_NOEXCEPT : _cursor(obj._cursor), _data(obj._data)
        { /* EMPTY. */ }

        Iterator & operator =(Iterator const & obj) TBAG_NOEXCEPT
        {
            if (this != &obj) {
                _cursor = obj._cursor;
                _data = obj._data;
            }
            return *this;
        }

        Iterator & operator =(Iterator && obj) TBAG_NOEXCEPT
        {
            if (this != &obj) {
                _cursor = obj._cursor;
                _data = obj._data;
            }
            return *this;
        }

        inline Cursor       & cursor()       TBAG_NOEXCEPT { return _cursor; }
        inline Cursor const & cursor() const TBAG_NOEXCEPT { return _cursor; }

        inline DataType       * operator ->()       TBAG_NOEXCEPT { return _data; }
        inline DataType const * operator ->() const TBAG_NOEXCEPT { return _data; }

        inline DataType       & operator *()       TBAG_NOEXCEPT { return *_data; }
        inline DataType const & operator *() const TBAG_NOEXCEPT { return *_data; }

        inline bool operator ==(Iterator const & obj) const TBAG_NOEXCEPT
        {
            return _data == obj._data;
        }

        inline bool operator !=(Iterator const & obj) const TBAG_NOEXCEPT
        {
            return _data != obj._data;
        }

        inline void step(int i) TBAG_NOEXCEPT
        {
            _data = (DataType*)(((std::intptr_t)_data) + (_cursor.getStrideByte() * i));
        }

        inline void next() TBAG_NOEXCEPT
        {
            step(1);
        }

        inline void prev() TBAG_NOEXCEPT
        {
            step(-1);
        }

        Iterator & operator ++() TBAG_NOEXCEPT
        {
            next();
            return *this;
        }

        Iterator & operator --() TBAG_NOEXCEPT
        {
            prev();
            return *this;
        }

        Iterator operator ++(int) TBAG_NOEXCEPT
        {
            Iterator temp = *this;
            next();
            return temp;
        }

        Iterator operator --(int) TBAG_NOEXCEPT
        {
            Iterator temp = *this;
            prev();
            return temp;
        }
    };

    template <typename CursorT, typename T>
    struct IteratorGenerator
    {
        using iterator = Iterator<CursorT, T>;
        using const_iterator = Iterator<
                typename std::add_const<CursorT>::type,
                typename std::add_const<T>::type>;

        CursorT & cursor;

        IteratorGenerator(CursorT & c) : cursor(c)
        { /* EMPTY. */ }
        ~IteratorGenerator()
        { /* EMPTY. */ }

        iterator begin()
        { return iterator(begin_tag, cursor); }
        iterator end()
        { return iterator(end_tag, cursor); }

        const_iterator begin() const
        { return const_iterator(begin_tag, cursor); }
        const_iterator end() const
        { return const_iterator(end_tag, cursor); }

        const_iterator cbegin() const
        { return const_iterator(begin_tag, cursor); }
        const_iterator cend() const
        { return const_iterator(end_tag, cursor); }

        template <typename Predicated>
        Err forEach(Predicated predicated)
        {
            return forEach(begin(), end(), predicated);
        }

        template <typename IteratorT, typename Predicated>
        Err forEach(IteratorT begin, IteratorT end, Predicated predicated)
        {
            for (; begin != end; ++begin) {
                predicated(*begin);
            }
            return libtbag::E_SUCCESS;
        }
    };

    /**
     * Slice structure.
     *
     * @author zer0
     * @date   2019-12-29
     */
    struct Slice
    {
        int begin = nop;
        int end = nop;
        int step = 1;
    };

    /**
     * Cursor class.
     *
     * @author zer0
     * @date   2019-12-16
     */
    class Cursor
    {
    private:
        box_cursor _cursor;

    public:
        Cursor() TBAG_NOEXCEPT
        { /* EMPTY. */ }

        Cursor(box_cursor const & c) TBAG_NOEXCEPT : _cursor(c)
        { /* EMPTY. */ }
        Cursor(box_cursor && c) TBAG_NOEXCEPT : _cursor(c)
        { /* EMPTY. */ }

        Cursor(Cursor const & obj) TBAG_NOEXCEPT : _cursor(obj._cursor)
        { /* EMPTY. */ }
        Cursor(Cursor && obj) TBAG_NOEXCEPT : _cursor(obj._cursor)
        { /* EMPTY. */ }

        ~Cursor()
        { /* EMPTY. */ }

    public:
        Cursor & operator =(Cursor const & obj) TBAG_NOEXCEPT
        {
            if (this != &obj) {
                _cursor = obj._cursor;
            }
            return *this;
        }

        Cursor & operator =(Cursor && obj) TBAG_NOEXCEPT
        {
            if (this != &obj) {
                _cursor = obj._cursor;
            }
            return *this;
        }

    public:
        ErrPair<Cursor> sub(Slice const & slice) const
        {
            return sub(slice.begin, slice.end, slice.step);
        }

        ErrPair<Cursor> sub(int begin_index, int end_index, int step_index) const
        {
            assert(_cursor.box != nullptr);
            if ((_cursor.dim_index+1) >= _cursor.box->rank) {
                return libtbag::E_OORANGE;
            }
            auto const err_cursor = _cursor.init_cursor(_cursor.dim_index+1, begin_index, end_index, step_index);
            return { err_cursor.code, err_cursor.value };
        }

        ErrPair<Cursor> sub(int begin_index, int end_index) const
        {
            return sub(begin_index, end_index, 1);
        }

        ErrPair<Cursor> sub(int begin_index) const
        {
            return sub(begin_index, nop);
        }

        ErrPair<Cursor> sub() const
        {
            return sub(nop);
        }

    public:
        void * getBegin() TBAG_NOEXCEPT
        {
            return _cursor.begin;
        }

        void const * getBegin() const TBAG_NOEXCEPT
        {
            return _cursor.begin;
        }

        void * getEnd() TBAG_NOEXCEPT
        {
            return _cursor.end;
        }

        void const * getEnd() const TBAG_NOEXCEPT
        {
            return _cursor.end;
        }

        int getStrideByte() const TBAG_NOEXCEPT
        {
            return _cursor.stride_byte;
        }

        int getDiff() const TBAG_NOEXCEPT
        {
            return _cursor.diff();
        }

        bool isLastDimension() const TBAG_NOEXCEPT
        {
            return _cursor.is_last_dim();
        }

        bool isContinue() const TBAG_NOEXCEPT
        {
            return _cursor.is_continue();
        }

        bool isEnd() const TBAG_NOEXCEPT
        {
            return _cursor.is_end();
        }

        bool next() TBAG_NOEXCEPT
        {
            return _cursor.next();
        }

    public:
        template <typename T>
        inline T * data() TBAG_NOEXCEPT
        {
            return static_cast<T*>(_cursor.begin);
        }

        template <typename T>
        inline T const * data() const TBAG_NOEXCEPT
        {
            return static_cast<T const *>(_cursor.begin);
        }

        inline void * data() TBAG_NOEXCEPT
        {
            return _cursor.begin;
        }

        inline void const * data() const TBAG_NOEXCEPT
        {
            return _cursor.begin;
        }

    public:
        template <typename T>
        inline T & at()
        {
            assert(_cursor.box != nullptr);
            assert(is_btype_equals<T>(_cursor.box->type));
            return *((T*)_cursor.begin);
        }

        template <typename T>
        inline T const & at() const
        {
            assert(_cursor.box != nullptr);
            assert(is_btype_equals<T>(_cursor.box->type));
            return *((T*)_cursor.begin);
        }

    public:
        template <typename T>
        Err get(T * out, bdev out_device, ui64 const * out_ext) const
        {
            assert(_cursor.box != nullptr);
            return _cursor.box->get_data(out, get_btype<T>(), out_device, out_ext, _cursor.begin);
        }

        template <typename T>
        Err get(T * out) const
        {
            using namespace libtbag::box::details;
            assert(_cursor.box != nullptr);
            auto const out_device = _cursor.box->device == BD_NONE ? BD_CPU : _cursor.box->device;
            ui64 const out_ext[TBAG_BOX_EXT_SIZE] = { _cursor.box->ext[0], _cursor.box->ext[1],
                                                      _cursor.box->ext[2], _cursor.box->ext[3] };
            return _cursor.box->get_data(out, get_btype<T>(), out_device, out_ext, _cursor.begin);
        }

        template <typename T>
        T get(bdev out_device, ui64 const * out_ext) const
        {
            T out = T();
            get(&out, out_device, out_ext);
            return out;
        }

        template <typename T>
        T get() const
        {
            T out = T();
            get(&out);
            return out;
        }

    public:
        template <typename T>
        Err set(T const * src, bdev src_device, ui64 const * src_ext)
        {
            assert(_cursor.box != nullptr);
            return _cursor.box->set_data(src, get_btype<T>(), src_device, src_ext, _cursor.begin);
        }

        template <typename T>
        Err set(T const & src, bdev src_device, ui64 const * src_ext)
        {
            return set((T const *)&src, src_device, src_ext);
        }

        template <typename T>
        Err set(T const * src)
        {
            using namespace libtbag::box::details;
            assert(_cursor.box != nullptr);
            auto const src_device = _cursor.box->device == BD_NONE ? BD_CPU : _cursor.box->device;
            ui64 const src_ext[TBAG_BOX_EXT_SIZE] = { _cursor.box->ext[0], _cursor.box->ext[1],
                                                      _cursor.box->ext[2], _cursor.box->ext[3] };
            return _cursor.box->set_data(src, get_btype<T>(), src_device, src_ext, _cursor.begin);
        }

        template <typename T>
        Err set(T const & src)
        {
            return set((T const *)&src);
        }

    public:
        template <typename T>
        IteratorGenerator<Cursor, T> itr()
        {
            assert(is_btype_equals<T>(_cursor.box->type));
            return IteratorGenerator<Cursor, T>(*this);
        }

        template <typename T>
        IteratorGenerator<typename std::add_const<Cursor>::type, T> itr() const
        {
            assert(is_btype_equals<T>(_cursor.box->type));
            return IteratorGenerator<typename std::add_const<Cursor>::type, T>(*this);
        }

        template <typename Predicated>
        Err forEach(Slice const * slice_begin, Slice const * slice_end, Predicated predicated)
        {
            if (isLastDimension()) {
                using __arg0 = typename libtbag::function_traits<Predicated>::template arguments<0>::type;
                using __type = typename libtbag::remove_cpr<__arg0>::type;
                return itr<__type>().forEach(predicated);
            } else {
                do {
                    ErrPair<Cursor> err_cursor;
                    if (slice_begin == slice_end) {
                        err_cursor = sub();
                    } else {
                        err_cursor = sub(*slice_begin);
                        ++slice_begin;
                    }
                    if (!err_cursor) {
                        return err_cursor.code;
                    }
                    auto const code = err_cursor.value.forEach(slice_begin, slice_end, predicated);
                    if (isFailure(code)) {
                        return code;
                    }
                } while (next());
                return libtbag::E_SUCCESS;
            }
        }

        template <typename T, typename Predicated>
        Err forEach(Slice const * slice, std::size_t size, Predicated predicated)
        {
            return forEach(slice, slice + size, predicated);
        }

        template <typename Predicated>
        Err forEachGet(Slice const * slice_begin, Slice const * slice_end, Predicated predicated)
        {
            if (slice_begin == slice_end || isLastDimension()) {
                do {
                    using __arg0 = typename libtbag::function_traits<Predicated>::template arguments<0>::type;
                    using __type = typename libtbag::remove_cpr<__arg0>::type;
                    predicated(get<__type>());
                } while (next());
            } else {
                do {
                    auto err_cursor = sub(*slice_begin);
                    if (isFailure(err_cursor)) {
                        return err_cursor.code;
                    }
                    auto const code = err_cursor.value.forEachGet(slice_begin+1, slice_end, predicated);
                    if (isFailure(code)) {
                        return code;
                    }
                } while (next());
                return libtbag::E_SUCCESS;
            }
        }
    };

public:
    ErrPair<Cursor> cursor(Slice const & slice) const;
    ErrPair<Cursor> cursor(int begin, int end, int step) const;
    ErrPair<Cursor> cursor(int begin, int end) const;
    ErrPair<Cursor> cursor(int begin) const;
    ErrPair<Cursor> cursor() const;

    template <typename Predicated>
    Err forEach(Slice const * slice_begin, Slice const * slice_end, Predicated predicated) const
    {
        ErrPair<Cursor> err_cursor;
        if (slice_begin == slice_end) {
            err_cursor = cursor();
        } else {
            err_cursor = cursor(*slice_begin);
            ++slice_begin;
        }
        if (!err_cursor) {
            return err_cursor.code;
        }
        return err_cursor.value.forEach(slice_begin, slice_end, predicated);
    }

    template <typename Predicated>
    Err forEach(Slice const * slices, std::size_t size, Predicated predicated) const
    {
        return forEach(slices, slices + size, predicated);
    }

    template <typename Predicated>
    Err forEach(Predicated predicated) const
    {
        return forEach(static_cast<Slice const *>(nullptr), static_cast<std::size_t>(0u), predicated);
    }

    std::vector<ui32> diffs(Slice const * slice_begin, Slice const * slice_end) const;
    std::vector<ui32> diffs(Slice const * slices, std::size_t size) const;
    std::vector<ui32> diffs(std::vector<Slice> const & slices) const;
    std::vector<ui32> diffs() const;

    template <typename Predicated>
    Err forEachGet(Slice const * slice_begin, Slice const * slice_end, Predicated predicated) const
    {
        ErrPair<Cursor> err_cursor;
        if (slice_begin == slice_end) {
            err_cursor = cursor();
        } else {
            err_cursor = cursor(*slice_begin);
            ++slice_begin;
        }
        if (!err_cursor) {
            return err_cursor.code;
        }
        return err_cursor.value.forEachGet(slice_begin, slice_end, predicated);
    }

    template <typename Predicated>
    Err forEachGet(Slice const * slices, std::size_t size, Predicated predicated) const
    {
        return forEachGet(slices, slices + size, predicated);
    }

    template <typename Predicated>
    Err forEachGet(Predicated predicated) const
    {
        return forEachGet(static_cast<Slice const *>(nullptr), static_cast<std::size_t>(0u), predicated);
    }

    Box slice(Slice const * slice_begin, Slice const * slice_end) const;
    Box slice(Slice const * slices, std::size_t size) const;
    Box slice(std::vector<Slice> const & slices) const;
};

} // namespace box

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#include <utility>
#include <functional>

// ------------
namespace std {
// ------------

template <>
struct hash<libtbag::box::Box>
{
    inline std::size_t operator()(libtbag::box::Box const & v) const
    {
        return reinterpret_cast<std::size_t>(v.base());
    }
};

template <>
struct equal_to<libtbag::box::Box>
{
    inline bool operator()(libtbag::box::Box const & x, libtbag::box::Box const & y) const
    {
        return x.base() == y.base();
    }
};

template <>
struct less<libtbag::box::Box>
{
    inline bool operator()(libtbag::box::Box const & x, libtbag::box::Box const & y) const
    {
        return x.base() < y.base();
    }
};

// ---------------
} // namespace std
// ---------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_BOX_BOX_HPP__

