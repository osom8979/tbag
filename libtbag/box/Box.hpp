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
#include <libtbag/box/BoxPacket.hpp>

#include <cstddef>
#include <cstdlib>
#include <cstdarg>
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
 *     <td><code>a.reshape([2, 3])</code></td>
 *     <td><code>a.reshape(2, 3)</code></td>
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

    using Buffer = std::vector<ui8>;

    using size_type = ui32;
    using difference_type = std::ptrdiff_t;

    using SharedBoxData = std::shared_ptr<box_data>;

public:
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

    TBAG_CONSTEXPR static btype const device_none() TBAG_NOEXCEPT { return libtbag::box::details::BD_NONE; }
    TBAG_CONSTEXPR static btype const device_cpu () TBAG_NOEXCEPT { return libtbag::box::details::BD_CPU ; }
    TBAG_CONSTEXPR static btype const device_cuda() TBAG_NOEXCEPT { return libtbag::box::details::BD_CUDA; }
    TBAG_CONSTEXPR static btype const device_cl  () TBAG_NOEXCEPT { return libtbag::box::details::BD_CL  ; }

private:
    SharedBoxData _data;

public:
    Box();
    explicit Box(std::nullptr_t) TBAG_NOEXCEPT;
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
    inline box_data       * getBoxData()       TBAG_NOEXCEPT { return _data.get(); }
    inline box_data const * getBoxData() const TBAG_NOEXCEPT { return _data.get(); }

    inline box_data       * ptr()       TBAG_NOEXCEPT { return getBoxData(); }
    inline box_data const * ptr() const TBAG_NOEXCEPT { return getBoxData(); }

    inline box_data       * operator ->()       TBAG_NOEXCEPT { return ptr(); }
    inline box_data const * operator ->() const TBAG_NOEXCEPT { return ptr(); }

    inline box_data       & ref()       TBAG_NOEXCEPT { return *ptr(); }
    inline box_data const & ref() const TBAG_NOEXCEPT { return *ptr(); }

    inline box_data       & operator *()       TBAG_NOEXCEPT { return ref(); }
    inline box_data const & operator *() const TBAG_NOEXCEPT { return ref(); }

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

public:
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

    template <typename T>
    inline T * cast() TBAG_NOEXCEPT
    {
        assert(exists());
        return static_cast<T*>(_data->data);
    }

    template <typename T>
    inline T const * cast() const TBAG_NOEXCEPT
    {
        assert(exists());
        return static_cast<T const *>(_data->data);
    }

    inline ui32 getCapacity() const TBAG_NOEXCEPT
    {
        if (_data) {
            return _data->total_data_byte;
        }
        return 0;
    }

    inline ui32 capacity() const TBAG_NOEXCEPT
    {
        assert(exists());
        return _data->total_data_byte;
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

    inline ui32 getDimensionsCapacity() const TBAG_NOEXCEPT
    {
        if (_data) {
            return _data->total_dims_byte;
        }
        return 0;
    }

    inline ui32 dims_capacity() const TBAG_NOEXCEPT
    {
        assert(exists());
        return _data->total_dims_byte;
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

    inline ui32 getInfoCapacity() const TBAG_NOEXCEPT
    {
        if (_data) {
            return _data->total_info_byte;
        }
        return 0;
    }

    inline ui32 info_capacity() const TBAG_NOEXCEPT
    {
        assert(exists());
        return _data->total_info_byte;
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
        assert(exists());
        _data->set_opaque(v);
    }

    template <typename T>
    void setOpaque(T v) TBAG_NOEXCEPT
    {
        assert(exists());
        assert(is_btype_equals<T>(getType()));
        _data->set_opaque(v);
    }

    inline void * getOpaquePointer() const TBAG_NOEXCEPT
    {
        assert(exists());
        return _data->get_opaque_pointer();
    }

    template <typename T>
    void getOpaque(T * v) const TBAG_NOEXCEPT
    {
        assert(exists());
        assert(is_btype_equals<T>(getType()));
        _data->get_opaque(v);
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
    Err reshape_args(btype type, bdev device, ui64 const * ext, ui32 rank, ...);
    Err reshape_args(btype type, ui32 rank, ...);

    template <typename T, typename ... Args>
    Err reshape_args_type(bdev device, ui64 const * ext, Args && ... args)
    {
        return reshape_args(get_btype<T>(), device, ext,
                            static_cast<ui32>(sizeof...(Args)),
                            std::forward<Args>(args) ...);
    }

    template <typename T, typename ... Args>
    Err reshape_args_type(Args && ... args)
    {
        return reshape_args(get_btype<T>(),
                            static_cast<ui32>(sizeof...(Args)),
                            std::forward<Args>(args) ...);
    }

    Err reshape_vargs(btype type, bdev device, ui64 const * ext, ui32 rank, va_list ap);
    Err reshape_vargs(btype type, ui32 rank, va_list ap);

    template <typename T>
    Err reshape_vargs_type(bdev device, ui64 const * ext, ui32 rank, va_list ap)
    {
        return reshape_vargs(get_btype<T>(), device, ext, rank, ap);
    }

    template <typename T>
    Err reshape_vargs_type(ui32 rank, va_list ap)
    {
        return reshape_vargs(get_btype<T>(), rank, ap);
    }

    Err reshape_dims(btype type, bdev device, ui64 const * ext, ui32 rank, ui32 const * dims);
    Err reshape_dims(btype type, ui32 rank, ui32 const * dims);

    template <typename T>
    Err reshape_dims_type(bdev device, ui64 const * ext, ui32 rank, ui32 const * dims)
    {
        return reshape_dims(get_btype<T>(), device, ext, rank, dims);
    }

    template <typename T>
    Err reshape_dims_type(ui32 rank, ui32 const * dims)
    {
        return reshape_dims(get_btype<T>(), rank, dims);
    }

    Err reshape_ref_box(box_data const * reference_box);
    Err reshape_ref_box(Box const & reference_box);

    template <typename T, typename ... Args>
    Err reshape(Args && ... args)
    {
        return reshape_args(get_btype<T>(),
                            static_cast<ui32>(sizeof...(Args)),
                            std::forward<Args>(args) ...);
    }

public:
    static Box shape_args(btype type, bdev device, ui64 const * ext, ui32 rank, ...);
    static Box shape_args(btype type, ui32 rank, ...);

    template <typename T, typename ... Args>
    static Box shape_args_type(bdev device, ui64 const * ext, Args && ... args)
    {
        return shape_args(get_btype<T>(), device, ext,
                          static_cast<ui32>(sizeof...(Args)),
                          std::forward<Args>(args) ...);
    }

    template <typename T, typename ... Args>
    static Box shape_args_type(Args && ... args)
    {
        return shape_args(get_btype<T>(),
                          static_cast<ui32>(sizeof...(Args)),
                          std::forward<Args>(args) ...);
    }

    static Box shape_vargs(btype type, bdev device, ui64 const * ext, ui32 rank, va_list ap);
    static Box shape_vargs(btype type, ui32 rank, va_list ap);

    template <typename T>
    static Box shape_vargs_type(bdev device, ui64 const * ext, ui32 rank, va_list ap)
    {
        return shape_vargs(get_btype<T>(), device, ext, rank, ap);
    }

    template <typename T>
    static Box shape_vargs_type(ui32 rank, va_list ap)
    {
        return shape_vargs(get_btype<T>(), rank, ap);
    }

    static Box shape_dims(btype type, bdev device, ui64 const * ext, ui32 rank, ui32 const * dims);
    static Box shape_dims(btype type, ui32 rank, ui32 const * dims);

    template <typename T>
    static Box shape_dims_type(bdev device, ui64 const * ext, ui32 rank, ui32 const * dims)
    {
        return shape_dims(get_btype<T>(), device, ext, rank, dims);
    }

    template <typename T>
    static Box shape_dims_type(ui32 rank, ui32 const * dims)
    {
        return shape_dims(get_btype<T>(), rank, dims);
    }

    template <typename T, typename ... Args>
    static Box shape(Args && ... args)
    {
        return shape_args(get_btype<T>(),
                          static_cast<ui32>(sizeof...(Args)),
                          std::forward<Args>(args) ...);
    }

public:
    Err copyToData(Box & box) const;
    Err copyFromData(Box const & box);

public:
    Err copyToInfo(Box & box) const;
    Err copyFromInfo(Box const & box);

public:
    Err copyTo(Box & box) const;
    Err copyFrom(Box const & box);

public:
    Box clone() const;

public:
    Box astype(btype type) const;

    template <typename T>
    Box astype() const
    {
        return astype(get_btype<T>());
    }

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
        auto const code = reshape_args(type, device, ext, 1, dim_1d);
        if (isFailure(code)) {
            return code;
        }

        if (is_device_cpu() && type == device_cpu()) {
            auto * d = cast<DataType>();
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
        return E_SUCCESS;
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
        auto const code = reshape_args(type, device, ext, 2, dim_1d, dim_2d);
        if (isFailure(code)) {
            return code;
        }

        if (is_device_cpu() && type == device_cpu()) {
            auto * d = cast<DataType>();
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
        return E_SUCCESS;
    }

    template <typename T>
    Err assign(bdev device, ui64 const * ext, init3d_t<T> const & items)
    {
        using DataType = typename libtbag::remove_cr<T>::type;
        auto const dim_1d = static_cast<ui32>(items.size());
        auto const dim_2d = static_cast<ui32>(items.begin()->size());
        auto const dim_3d = static_cast<ui32>(items.begin()->begin()->size());
        auto const type = get_btype<DataType>();
        auto const code = reshape_args(type, device, ext, 3, dim_1d, dim_2d, dim_3d);
        if (isFailure(code)) {
            return code;
        }

        if (is_device_cpu() && type == device_cpu()) {
            auto * d = cast<DataType>();
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
        return E_SUCCESS;
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
        auto const code = reshape_args(type, device, ext, 4, dim_1d, dim_2d, dim_3d, dim_4d);
        if (isFailure(code)) {
            return code;
        }

        if (is_device_cpu() && type == device_cpu()) {
            auto * d = cast<DataType>();
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
        return E_SUCCESS;
    }

    // clang-format off
    template <typename T> Err assign(init1d_t<T> const & items) { return assign(device_cpu(), nullptr, items); }
    template <typename T> Err assign(init2d_t<T> const & items) { return assign(device_cpu(), nullptr, items); }
    template <typename T> Err assign(init3d_t<T> const & items) { return assign(device_cpu(), nullptr, items); }
    template <typename T> Err assign(init4d_t<T> const & items) { return assign(device_cpu(), nullptr, items); }
    // clang-format on

public:
    template <typename T>
    inline typename libtbag::remove_cr<T>::type & getElementByOffset(ui32 i) TBAG_NOEXCEPT
    {
        using result_type = typename libtbag::remove_cr<T>::type;
        assert(exists());
        assert(is_btype_equals<result_type>(type()));
        assert(i < size());
        return *((result_type*)_data->get_data_ptr_by_offset(i));
    }

    template <typename T>
    inline typename libtbag::remove_cr<T>::type const & getElementByOffset(ui32 i) const TBAG_NOEXCEPT
    {
        using result_type = typename libtbag::remove_cr<T>::type const;
        assert(exists());
        assert(is_btype_equals<result_type>(type()));
        assert(i < size());
        return *((result_type*)_data->get_data_ptr_by_offset(i));
    }

    template <typename T, typename ... Args>
    inline typename libtbag::remove_cr<T>::type & at(Args && ... args) TBAG_NOEXCEPT
    {
        using result_type = typename libtbag::remove_cr<T>::type;
        static_assert(static_cast<ui32>(sizeof...(Args)) >= 1u, "At least one Args is required.");
        using namespace libtbag::box::details;
        auto const offset = box_dim_get_offset_args(getDimensions(),
                                                    static_cast<ui32>(sizeof...(Args)),
                                                    std::forward<Args>(args) ...);
        return getElementByOffset<result_type>(offset);
    }

    template <typename T, typename ... Args>
    inline typename libtbag::remove_cr<T>::type const & at(Args && ... args) const TBAG_NOEXCEPT
    {
        using result_type = typename libtbag::remove_cr<T>::type;
        static_assert(static_cast<ui32>(sizeof...(Args)) >= 1u, "At least one Args is required.");
        using namespace libtbag::box::details;
        auto const offset = box_dim_get_offset_args(getDimensions(),
                                                    static_cast<ui32>(sizeof...(Args)),
                                                    std::forward<Args>(args) ...);
        return getElementByOffset<result_type>(offset);
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

    struct Cursor
    {
        box_cursor cursor;

        Cursor() TBAG_NOEXCEPT
        { /* EMPTY. */ }

        Cursor(box_cursor const & c) TBAG_NOEXCEPT : cursor(c)
        { /* EMPTY. */ }
        Cursor(box_cursor && c) TBAG_NOEXCEPT : cursor(std::move(c))
        { /* EMPTY. */ }

        Cursor(Cursor const & obj) TBAG_NOEXCEPT : cursor(obj.cursor)
        { /* EMPTY. */ }
        Cursor(Cursor && obj) TBAG_NOEXCEPT : cursor(std::move(obj.cursor))
        { /* EMPTY. */ }

        ~Cursor()
        { /* EMPTY. */ }

        Cursor & operator =(Cursor const & obj) TBAG_NOEXCEPT
        {
            if (this != &obj) {
                cursor = obj.cursor;
            }
            return *this;
        }

        Cursor & operator =(Cursor && obj) TBAG_NOEXCEPT
        {
            if (this != &obj) {
                cursor = std::move(obj.cursor);
            }
            return *this;
        }

        ErrPair<Cursor> createSubCursor(ui32 dim_index, int begin_index, int end_index, int step_index)
        {
            auto const err_cursor = cursor.init_cursor(dim_index, begin_index, end_index, step_index);
            return { err_cursor.code, err_cursor.value };
        }

        ErrPair<Cursor> createSubCursor(ui32 dim_index, int begin_index, int end_index)
        {
            auto const err_cursor = cursor.init_cursor(dim_index, begin_index, end_index);
            return { err_cursor.code, err_cursor.value };
        }

        ErrPair<Cursor> createSubCursor(ui32 dim_index, int begin_index)
        {
            auto const err_cursor = cursor.init_cursor(dim_index, begin_index);
            return { err_cursor.code, err_cursor.value };
        }

        ErrPair<Cursor> createSubCursor(ui32 dim_index)
        {
            auto const err_cursor = cursor.init_cursor(dim_index);
            return { err_cursor.code, err_cursor.value };
        }

        ErrPair<Cursor> createSubCursor()
        {
            auto const err_cursor = cursor.init_cursor();
            return { err_cursor.code, err_cursor.value };
        }

        bool isEnd() const
        {
            return cursor.is_end();
        }

        bool next()
        {
            return cursor.next();
        }
    };

    ErrPair<Cursor> createCursor(void * data_begin, ui32 dim_index, int begin, int end, int step);
    ErrPair<Cursor> createCursor(ui32 dim_index, int begin, int end, int step);
    ErrPair<Cursor> createCursor(ui32 dim_index, int begin, int end);
    ErrPair<Cursor> createCursor(ui32 dim_index, int begin);
    ErrPair<Cursor> createCursor(ui32 dim_index);
    ErrPair<Cursor> createCursor();
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
        return reinterpret_cast<std::size_t>(v.ptr());
    }
};

template <>
struct equal_to<libtbag::box::Box>
{
    inline bool operator()(libtbag::box::Box const & x, libtbag::box::Box const & y) const
    {
        return x.ptr() == y.ptr();
    }
};

template <>
struct less<libtbag::box::Box>
{
    inline bool operator()(libtbag::box::Box const & x, libtbag::box::Box const & y) const
    {
        return x.ptr() < y.ptr();
    }
};

// ---------------
} // namespace std
// ---------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_BOX_BOX_HPP__

