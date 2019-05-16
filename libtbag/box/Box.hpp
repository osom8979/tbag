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

#include <cstddef>
#include <cassert>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace box {

/**
 * Box class prototype.
 *
 * @author zer0
 * @date   2019-05-16
 */
class TBAG_API Box
{
public:
    using box_data   = libtbag::box::details::box_data;
    using box_cursor = libtbag::box::details::box_cursor;

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
    inline void * data() const TBAG_NOEXCEPT
    { return _data->data; }

    inline ui32 total_data_byte() const TBAG_NOEXCEPT
    { return _data->total_data_byte; }

    inline ui32 size() const TBAG_NOEXCEPT
    { return _data->size; }

public:
    inline ui32 * dims() const TBAG_NOEXCEPT
    { return _data->dims; }

    inline ui32 total_dims_byte() const TBAG_NOEXCEPT
    { return _data->total_dims_byte; }

    inline ui32 rank() const TBAG_NOEXCEPT
    { return _data->rank; }

public:
    inline char * info() const TBAG_NOEXCEPT
    { return _data->info; }

    inline ui32 total_info_byte() const TBAG_NOEXCEPT
    { return _data->total_info_byte; }

    inline ui32 info_size() const TBAG_NOEXCEPT
    { return _data->info_size; }

public:
    inline void * opaque() const TBAG_NOEXCEPT
    { return _data->opaque; }

    inline void setOpaque(void * v) const TBAG_NOEXCEPT
    { _data->opaque = v; }

public:
    void reset();

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
//    template <typename T>
//    T & at(ui32 i)
//    {
//        assert(0 <= COMPARE_AND(i) < size());
//    }
//
//    template <typename T>
//    T const & at(ui32 i) const
//    {
//        assert(0 <= COMPARE_AND(i) < size());
//    }

    ui32 dim(ui32 i) const
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

