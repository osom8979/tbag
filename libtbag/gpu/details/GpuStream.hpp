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
#include <libtbag/type/TypeTable.hpp>
#include <libtbag/container/AnyPod.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace details {

class GpuEvent;
class SyncedMemory;

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
    using TypeTable = type::TypeTable;
    using AnyPod = container::AnyPod;

private:
    GpuStream(GpuContext const & context);
public:
    ~GpuStream();

public:
    Err  flush() const;
    Err finish() const;

public:
    Err fill(SyncedMemory & out, AnyPod data, int count, GpuEvent * event = nullptr);
    Err add(SyncedMemory const & in1, SyncedMemory const & in2, SyncedMemory & out, int count, GpuEvent * event = nullptr);

public:
    static GpuStream * newInstance(GpuContext const & context);
};

using SharedGpuStream = std::shared_ptr<GpuStream>;
using WeakedGpuStream = std::weak_ptr<GpuStream>;

} // namespace details
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_GPUSTREAM_HPP__

