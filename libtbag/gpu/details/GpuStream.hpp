/**
 * @file   GpuStream.hpp
 * @brief  GpuStream class prototype.
 * @author zer0
 * @date   2018-01-16
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_GPUSTREAM_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_GPUSTREAM_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <libtbag/gpu/details/GpuCommon.hpp>
#include <libtbag/gpu/details/GpuIdWrapper.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace details {

/**
 * GpuStream class prototype.
 *
 * @author zer0
 * @date   2018-01-14
 * @date   2018-01-15 (struct -> class)
 */
class TBAG_API GpuStream : public GpuIdWrapper
{
public:
    GpuStream(GpuContext const * c = nullptr, GpuId i = UNKNOWN_ID);
    GpuStream(GpuStream const & obj);
    GpuStream(GpuStream && obj);
    virtual ~GpuStream();

public:
    GpuStream & operator =(GpuStream const & obj);
    GpuStream & operator =(GpuStream && obj);

public:
    void swap(GpuStream & obj);
    inline friend void swap(GpuStream & lh, GpuStream & rh) { lh.swap(rh); }

public:
    bool create();
    bool release();

public:
    static GpuStream create(GpuContext const * c);
};

using SharedGpuStream = std::shared_ptr<GpuStream>;
using WeakedGpuStream = std::weak_ptr<GpuStream>;

} // namespace details
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_GPUSTREAM_HPP__

