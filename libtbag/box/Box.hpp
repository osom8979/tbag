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

public:
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
        auto const CODE = assign<T>(items);
        if (isFailure(CODE)) {
            throw ErrException(CODE);
        }
    }

    template <typename T>
    Box(std::initializer_list< std::initializer_list<T> > const & items) : Box()
    {
        auto const CODE = assign<T>(items);
        if (isFailure(CODE)) {
            throw ErrException(CODE);
        }
    }

    template <typename T>
    Box(std::initializer_list< std::initializer_list< std::initializer_list<T> > > const & items) : Box()
    {
        auto const CODE = assign<T>(items);
        if (isFailure(CODE)) {
            throw ErrException(CODE);
        }
    }

    template <typename T>
    Box(std::initializer_list< std::initializer_list< std::initializer_list< std::initializer_list<T> > > > const & items) : Box()
    {
        auto const CODE = assign<T>(items);
        if (isFailure(CODE)) {
            throw ErrException(CODE);
        }
    }

public:
    Box & operator =(Box const & obj) TBAG_NOEXCEPT;
    Box & operator =(Box && obj) TBAG_NOEXCEPT;
    Box & operator =(std::nullptr_t) TBAG_NOEXCEPT;

public:
    inline void reset() TBAG_NOEXCEPT_SP_OP(_data.reset())
    {
        _data.reset();
    }

public:
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
    { return static_cast<bool>(_data); }

    inline operator bool() const TBAG_NOEXCEPT
    { return exists(); }

public:
    inline box_data       * get()       TBAG_NOEXCEPT { return _data.get(); }
    inline box_data const * get() const TBAG_NOEXCEPT { return _data.get(); }

    inline box_data       * operator ->()       TBAG_NOEXCEPT { return get(); }
    inline box_data const * operator ->() const TBAG_NOEXCEPT { return get(); }

    inline box_data       & ref()       TBAG_NOEXCEPT { return *get(); }
    inline box_data const & ref() const TBAG_NOEXCEPT { return *get(); }

    inline box_data       & operator *()       TBAG_NOEXCEPT { return ref(); }
    inline box_data const & operator *() const TBAG_NOEXCEPT { return ref(); }

public:
    friend inline bool operator <(Box const & x, Box const & y) TBAG_NOEXCEPT
    { return x.get() < y.get(); }

    friend inline bool operator >(Box const & x, Box const & y) TBAG_NOEXCEPT
    { return x.get() > y.get(); }

    friend inline bool operator <=(Box const & x, Box const & y) TBAG_NOEXCEPT
    { return x.get() <= y.get(); }

    friend inline bool operator >=(Box const & x, Box const & y) TBAG_NOEXCEPT
    { return x.get() >= y.get(); }

public:
    inline bool operator ==(Box const & obj) const TBAG_NOEXCEPT
    { return get() == obj.get(); }

    inline bool operator !=(Box const & obj) const TBAG_NOEXCEPT
    { return get() != obj.get(); }

public:
    inline btype type() const TBAG_NOEXCEPT
    { return _data->type; }

public:
    // clang-format off
    inline bool is_none() const TBAG_NOEXCEPT { return type() == type_none(); }
    inline bool is_si8 () const TBAG_NOEXCEPT { return type() == type_si8 (); }
    inline bool is_si16() const TBAG_NOEXCEPT { return type() == type_si16(); }
    inline bool is_si32() const TBAG_NOEXCEPT { return type() == type_si32(); }
    inline bool is_si64() const TBAG_NOEXCEPT { return type() == type_si64(); }
    inline bool is_ui8 () const TBAG_NOEXCEPT { return type() == type_ui8 (); }
    inline bool is_ui16() const TBAG_NOEXCEPT { return type() == type_ui16(); }
    inline bool is_ui32() const TBAG_NOEXCEPT { return type() == type_ui32(); }
    inline bool is_ui64() const TBAG_NOEXCEPT { return type() == type_ui64(); }
    inline bool is_fp32() const TBAG_NOEXCEPT { return type() == type_fp32(); }
    inline bool is_fp64() const TBAG_NOEXCEPT { return type() == type_fp64(); }
    // clang-format on

public:
    inline btype device() const TBAG_NOEXCEPT
    { return _data->device; }

public:
    // clang-format off
    inline bool is_device_none() const TBAG_NOEXCEPT { return device() == device_none(); }
    inline bool is_device_cpu () const TBAG_NOEXCEPT { return device() == device_cpu (); }
    inline bool is_device_cuda() const TBAG_NOEXCEPT { return device() == device_cuda(); }
    inline bool is_device_cl  () const TBAG_NOEXCEPT { return device() == device_cl  (); }
    // clang-format on

public:
    inline ui64 const * ext() const TBAG_NOEXCEPT
    { return _data->ext; }

public:
    // clang-format off
    inline ui64 ext0() const TBAG_NOEXCEPT { return ext()[0]; }
    inline ui64 ext1() const TBAG_NOEXCEPT { return ext()[1]; }
    inline ui64 ext2() const TBAG_NOEXCEPT { return ext()[2]; }
    inline ui64 ext3() const TBAG_NOEXCEPT { return ext()[3]; }
    // clang-format on

public:
    inline ui32 capacity() const TBAG_NOEXCEPT
    { return _data->total_data_byte; }

    inline ui32 size() const TBAG_NOEXCEPT
    { return _data->size; }

    inline bool empty() const TBAG_NOEXCEPT
    { return _data->size == 0; }

public:
    inline ui32 const * dims() const TBAG_NOEXCEPT
    { return _data->dims; }

    inline ui32 dim(ui32 i) const TBAG_NOEXCEPT
    {
        assert(0 <= COMPARE_AND(i) < rank());
        return *(dims() + i);
    }

    inline ui32 dims_capacity() const TBAG_NOEXCEPT
    { return _data->total_dims_byte; }

    inline ui32 rank() const TBAG_NOEXCEPT
    { return _data->rank; }

public:
    inline void * opaque() const TBAG_NOEXCEPT
    { return _data->opaque; }

    inline void setOpaque(void * v) const TBAG_NOEXCEPT
    { _data->opaque = v; }

    template <typename T>
    void setOpaque(T v) TBAG_NOEXCEPT
    {
        assert(exists());
        assert(is_btype_equals<T>(type()));
        libtbag::box::details::box_opaque_set(get(), v);
    }

    template <typename T>
    void getOpaque(T * v) const TBAG_NOEXCEPT
    {
        assert(exists());
        assert(is_btype_equals<T>(type()));
        libtbag::box::details::box_opaque_get(get(), v);
    }

public:
    inline ui8 * info() TBAG_NOEXCEPT
    { return _data->info; }

    inline ui8 const * info() const TBAG_NOEXCEPT
    { return _data->info; }

    inline ui32 info_capacity() const TBAG_NOEXCEPT
    { return _data->total_info_byte; }

    inline ui32 info_size() const TBAG_NOEXCEPT
    { return _data->info_size; }

public:
    Err setInfo(ui8 const * info, ui32 size);
    Err setInfo(std::string const & info);
    Err setInfo(Buffer const & info);

    std::string getInfoString() const;
    Buffer getInfoBuffer() const;

public:
    Err reshape_args(btype type, bdev device, ui64 const * ext, ui32 rank, ...);
    Err reshape_args(btype type, ui32 rank, ...);
    Err reshape_vargs(btype type, bdev device, ui64 const * ext, ui32 rank, va_list ap);
    Err reshape_vargs(btype type, ui32 rank, va_list ap);
    Err reshape_dims(btype type, bdev device, ui64 const * ext, ui32 rank, ui32 const * dims);
    Err reshape_dims(btype type, ui32 rank, ui32 const * dims);
    Err reshape_box(box_data const * reference_box_data);
    Err reshape_box(Box const & reference_box_data);

    template <typename T, typename ... Args>
    Err reshapeEx(bdev device, ui64 const * ext, Args && ... args)
    { return reshape_args(get_btype<T>(), device, ext, sizeof...(Args), std::forward<Args>(args) ...); }

    template <typename T, typename ... Args>
    Err reshape(Args && ... args)
    { return reshape_args(get_btype<T>(), sizeof...(Args), std::forward<Args>(args) ...); }

    template <typename T>
    Err reshapeDims(bdev device, ui64 const * ext, ui32 rank, ui32 const * dims)
    { return reshape_dims(get_btype<T>(), device, ext, rank, dims); }

    template <typename T>
    Err reshapeDims(ui32 rank, ui32 const * dims)
    { return reshape_dims(get_btype<T>(), rank, dims); }

public:
    static Box shape_args(btype type, bdev device, ui64 const * ext, ui32 rank, ...);
    static Box shape_args(btype type, ui32 rank, ...);
    static Box shape_vargs(btype type, bdev device, ui64 const * ext, ui32 rank, va_list ap);
    static Box shape_vargs(btype type, ui32 rank, va_list ap);
    static Box shape_dims(btype type, bdev device, ui64 const * ext, ui32 rank, ui32 const * dims);
    static Box shape_dims(btype type, ui32 rank, ui32 const * dims);

    template <typename T, typename ... Args>
    static Box shapeEx(bdev device, ui64 const * ext, Args && ... args)
    { return shape_args(get_btype<T>(), device, ext, sizeof...(Args), std::forward<Args>(args) ...); }

    template <typename T, typename ... Args>
    static Box shape(Args && ... args)
    { return shape_args(get_btype<T>(), sizeof...(Args), std::forward<Args>(args) ...); }

    template <typename T>
    static Box shapeDims(bdev device, ui64 const * ext, ui32 rank, ui32 const * dims)
    { return shape_dims(get_btype<T>(), device, ext, rank, dims); }

    template <typename T>
    static Box shapeDims(ui32 rank, ui32 const * dims)
    { return shape_dims(get_btype<T>(), rank, dims); }

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
    Box astype(btype type) const;

    template <typename T>
    Box astype() const
    { return astype(get_btype<T>()); }

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
                libtbag::box::details::box_data_set(get(), begin, type, device, ext, offset);
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
                    libtbag::box::details::box_data_set(get(), &i2, type, device, ext, offset);
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
                        libtbag::box::details::box_data_set(get(), &i3, type, device, ext, offset);
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
                            libtbag::box::details::box_data_set(get(), &i4, type, device, ext, offset);
                            ++offset;
                        }
                    }
                }
            }
        }
        return E_SUCCESS;
    }

public:
    // clang-format off
    template <typename T> Err assign(init1d_t<T> const & items) { return assign(device_cpu(), nullptr, items); }
    template <typename T> Err assign(init2d_t<T> const & items) { return assign(device_cpu(), nullptr, items); }
    template <typename T> Err assign(init3d_t<T> const & items) { return assign(device_cpu(), nullptr, items); }
    template <typename T> Err assign(init4d_t<T> const & items) { return assign(device_cpu(), nullptr, items); }
    // clang-format on

public:
    inline bool isSupportType() const TBAG_NOEXCEPT
    { return libtbag::box::details::box_support_type(type()); }

    inline bool isSupportDevice() const TBAG_NOEXCEPT
    { return libtbag::box::details::box_support_device(device()); }

    inline char const * const getTypeName() const TBAG_NOEXCEPT
    { return libtbag::box::details::box_get_type_name(type()); }

    inline char const * const getDeviceName(bdev dev) const TBAG_NOEXCEPT
    { return libtbag::box::details::box_get_device_name(type()); }

    inline ui32 getTypeByte() const TBAG_NOEXCEPT
    { return libtbag::box::details::box_get_type_byte(type()); }

    inline ui32 getStride(ui32 i) const TBAG_NOEXCEPT
    { return libtbag::box::details::box_dim_get_stride(dims(), rank(), i); }

    inline ui32 getStrideByte(ui32 i) const TBAG_NOEXCEPT
    { return getStride(i) * getTypeByte(); }

public:
    inline void * data() TBAG_NOEXCEPT
    { return _data->data; }

    inline void const * data() const TBAG_NOEXCEPT
    { return _data->data; }

    template <typename T>
    inline T * cast() TBAG_NOEXCEPT
    { return static_cast<T*>(data()); }

    template <typename T>
    inline T const * cast() const TBAG_NOEXCEPT
    { return static_cast<T const *>(data()); }

    template <typename T>
    inline typename libtbag::remove_cr<T>::type & offset(ui32 i) TBAG_NOEXCEPT
    {
        using ResultType = typename libtbag::remove_cr<T>::type;
        assert(exists());
        assert(is_btype_equals<ResultType>(type()));
        assert(0 <= COMPARE_AND(i) < size());
        return *((ResultType*)libtbag::box::details::box_data_ptr_offset(get(), i));
    }

    template <typename T>
    inline typename libtbag::remove_cr<T>::type const & offset(ui32 i) const TBAG_NOEXCEPT
    {
        using ResultType = typename libtbag::remove_cr<T>::type;
        assert(exists());
        assert(is_btype_equals<ResultType>(type()));
        assert(0 <= COMPARE_AND(i) < size());
        return *((ResultType*)libtbag::box::details::box_data_ptr_offset(get(), i));
    }

    template <typename T>
    inline typename libtbag::remove_cr<T>::type & operator [](ui32 i) TBAG_NOEXCEPT
    { return offset<typename libtbag::remove_cr<T>::type>(i); }

    template <typename T>
    inline typename libtbag::remove_cr<T>::type const & operator [](ui32 i) const TBAG_NOEXCEPT
    { return offset<typename libtbag::remove_cr<T>::type>(i); }

    template <typename T, typename ... Args>
    inline typename libtbag::remove_cr<T>::type & at(Args && ... args) TBAG_NOEXCEPT
    {
        using ResultType = typename libtbag::remove_cr<T>::type;
        static_assert(sizeof...(Args) >= 1, "At least one Args is required.");
        using namespace libtbag::box::details;
        return offset<ResultType>(box_dim_get_index_args(dims(), sizeof...(Args), std::forward<Args>(args) ...));
    }

    template <typename T, typename ... Args>
    inline typename libtbag::remove_cr<T>::type const & at(Args && ... args) const TBAG_NOEXCEPT
    {
        using ResultType = typename libtbag::remove_cr<T>::type;
        static_assert(sizeof...(Args) >= 1, "At least one Args is required.");
        using namespace libtbag::box::details;
        return offset<ResultType>(box_dim_get_index_args(dims(), sizeof...(Args), std::forward<Args>(args) ...));
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
};

} // namespace box

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#include <utility>

// ------------
namespace std {
// ------------

template <>
struct hash<libtbag::box::Box>
{
    inline std::size_t operator()(libtbag::box::Box const & v) const TBAG_NOEXCEPT
    {
        return reinterpret_cast<std::size_t>(v.get());
    }
};

// ---------------
} // namespace std
// ---------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_BOX_BOX_HPP__

