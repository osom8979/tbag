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
#include <libtbag/box/details/box_api.hpp>
#include <libtbag/tmp/NumberOfArguments.hpp>

#include <cstddef>
#include <cstdlib>
#include <cstdarg>
#include <cassert>

#include <type_traits>
#include <memory>

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

using btype = libtbag::box::details::btype;
using bdev  = libtbag::box::details::bdev;

using box_data   = libtbag::box::details::box_data;
using box_cursor = libtbag::box::details::box_cursor;
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

template <typename T>
TBAG_CONSTEXPR inline BoxTypeTable getBoxType() TBAG_NOEXCEPT
{
    return BoxTypeInfo<T>::value;
}

template <typename T>
inline bool isTypeEquals(BoxTypeTable type) TBAG_NOEXCEPT
{
    return type == getBoxType<T>();
}

/**
 * Box class prototype.
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

public:
    using SharedBoxData = std::shared_ptr<box_data>;

private:
    SharedBoxData _data;

public:
    Box();
    Box(std::nullptr_t) TBAG_NOEXCEPT;
    Box(Box const & obj) TBAG_NOEXCEPT;
    Box(Box && obj) TBAG_NOEXCEPT;
    ~Box();

public:
    Box & operator =(Box const & obj) TBAG_NOEXCEPT;
    Box & operator =(Box && obj) TBAG_NOEXCEPT;

public:
    void copy(Box const & obj) TBAG_NOEXCEPT;
    void swap(Box & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(Box & lh, Box & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

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

    inline bool operator ==(Box const & obj) const TBAG_NOEXCEPT
    { return get() == obj.get(); }

    inline bool operator !=(Box const & obj) const TBAG_NOEXCEPT
    { return get() != obj.get(); }

public:
    void reset();

public:
    inline btype type() const TBAG_NOEXCEPT
    { return _data->type; }

    inline btype device() const TBAG_NOEXCEPT
    { return _data->device; }

    inline ui64 ext0() const TBAG_NOEXCEPT
    { return _data->ext[0]; }
    inline ui64 ext1() const TBAG_NOEXCEPT
    { return _data->ext[1]; }
    inline ui64 ext2() const TBAG_NOEXCEPT
    { return _data->ext[2]; }
    inline ui64 ext3() const TBAG_NOEXCEPT
    { return _data->ext[3]; }

public:
    inline ui32 capacity() const TBAG_NOEXCEPT
    { return _data->total_data_byte; }

    inline ui32 size() const TBAG_NOEXCEPT
    { return _data->size; }

public:
    inline ui32 const * dims() const TBAG_NOEXCEPT
    { return _data->dims; }

    inline ui32 rank() const TBAG_NOEXCEPT
    { return _data->rank; }

public:
    inline void * opaque() const TBAG_NOEXCEPT
    { return _data->opaque; }

    inline void setOpaque(void * v) const TBAG_NOEXCEPT
    { _data->opaque = v; }

public:
    Err resize_args(btype type, bdev device, ui64 const * ext, ui32 rank, ...);
    Err resize_vargs(btype type, bdev device, ui64 const * ext, ui32 rank, va_list ap);

    template <typename ... Args>
    Err resize(btype type, bdev device, ui64 const * ext, Args && ... args) TBAG_NOEXCEPT
    {
        using namespace libtbag::tmp;
        using namespace libtbag::box::details;
        auto const n = get_number_of_arguments(std::forward<Args>(args) ...);
        return resize_args(type, device, ext, n, std::forward<Args>(args) ...);
    }

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

public:
    inline void * data() TBAG_NOEXCEPT
    { return _data->data; }

    inline void const * data() const TBAG_NOEXCEPT
    { return _data->data; }

    template <typename T>
    inline T * cast() TBAG_NOEXCEPT
    { return data(); }

    template <typename T>
    inline T const * cast() const TBAG_NOEXCEPT
    { return data(); }

    template <typename T>
    inline T & at_offset(ui32 i) TBAG_NOEXCEPT
    {
        assert(0 <= COMPARE_AND(i) < size());
        return *cast<T>()[i];
    }

    template <typename T>
    inline T const & at_offset(ui32 i) const TBAG_NOEXCEPT
    {
        assert(0 <= COMPARE_AND(i) < size());
        return *cast<T>()[i];
    }

    template <typename T, typename ... Args>
    inline T & at(Args && ... args) TBAG_NOEXCEPT
    {
        return at_offset<T>(libtbag::box::details::box_dim_get_index_args(
                dims(), libtbag::tmp::NumberOfTemplateArguments<Args ...>::value, std::forward<Args>(args) ...));
    }

    template <typename T, typename ... Args>
    inline T const & at(Args && ... args) const TBAG_NOEXCEPT
    {
        return at_offset<T>(libtbag::box::details::box_dim_get_index_args(
                dims(), libtbag::tmp::NumberOfTemplateArguments<Args ...>::value, std::forward<Args>(args) ...));
    }

    inline ui32 dim(ui32 i) const TBAG_NOEXCEPT
    {
        assert(0 <= COMPARE_AND(i) < rank());
        return *(dims() + i);
    }
};

} // namespace box

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_BOX_BOX_HPP__

