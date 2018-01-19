/**
 * @file   GpuDetails.hpp
 * @brief  GpuDetails class prototype.
 * @author zer0
 * @date   2018-01-16
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_GPUDETAILS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_GPUDETAILS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <libtbag/gpu/details/GpuCommon.hpp>
#include <libtbag/gpu/details/GpuContext.hpp>
#include <libtbag/gpu/details/GpuEvent.hpp>
#include <libtbag/gpu/details/GpuKernel.hpp>
#include <libtbag/gpu/details/GpuMemory.hpp>
#include <libtbag/gpu/details/GpuStream.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace details {

// EMPTY.

} // namespace details

using namespace ::libtbag::gpu::details;

} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_GPUDETAILS_HPP__

