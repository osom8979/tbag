/**
 * @file   GpuIdWrapper.hpp
 * @brief  GpuIdWrapper class prototype.
 * @author zer0
 * @date   2018-01-16
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_GPUIDWRAPPER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_GPUIDWRAPPER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/gpu/details/GpuCommon.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace details {

class GpuContext;

/**
 * GpuIdWrapper structure.
 *
 * @author zer0
 * @date   2018-01-15
 */
class TBAG_API GpuIdWrapper
{
protected:
    GpuContext const * _context;
    GpuId _id;

public:
    GpuIdWrapper(GpuContext const * c = nullptr, GpuId i = UNKNOWN_ID);
    GpuIdWrapper(GpuIdWrapper const & obj);
    GpuIdWrapper(GpuIdWrapper && obj);
    virtual ~GpuIdWrapper();

public:
    inline GpuIdWrapper & operator =(GpuIdWrapper const & obj);
    inline GpuIdWrapper & operator =(GpuIdWrapper && obj);

public:
    void assign(GpuIdWrapper const & obj);
    void swap(GpuIdWrapper & obj);

public:
    inline bool isSameContext(GpuContext const * c) const TBAG_NOEXCEPT { return _context == c; }
    inline GpuContext const * getContextPtr() const TBAG_NOEXCEPT { return _context; }

    inline bool      existsId() const TBAG_NOEXCEPT { return _id != UNKNOWN_ID; }
    inline GpuId        getId() const TBAG_NOEXCEPT { return _id; }
    inline GpuId       & atId()       TBAG_NOEXCEPT { return _id; }
    inline GpuId const & atId() const TBAG_NOEXCEPT { return _id; }

    inline void clearId() TBAG_NOEXCEPT { _id = UNKNOWN_ID; }

    template <typename T> inline void setId(T i)    TBAG_NOEXCEPT { _id = (GpuId)i; }
    template <typename T> inline T   castId() const TBAG_NOEXCEPT { return (T)_id; }

    inline bool validate() const TBAG_NOEXCEPT
    { return (_context != nullptr) && existsId(); }
    inline bool validate(GpuContext const * c) const TBAG_NOEXCEPT
    { return (_context != nullptr) && isSameContext(c) && existsId(); }
};

} // namespace details
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_GPUIDWRAPPER_HPP__

