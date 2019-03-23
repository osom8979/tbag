/**
 * @file   CpuDevice.hpp
 * @brief  CpuDevice class prototype.
 * @author zer0
 * @date   2019-02-22
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PARALLEL_CPU_CPUDEVICE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PARALLEL_CPU_CPUDEVICE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/parallel/ParallelCommon.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace parallel {
namespace cpu      {

TBAG_API bool isSupport() TBAG_NOEXCEPT;

TBAG_API int getPlatformCount();
TBAG_API ParallelPlatformIds getPlatformList();
TBAG_API ParallelPlatformInfo getPlatformInfo(ParallelPlatformId platform);

TBAG_API int getDeviceCount(ParallelPlatformId platform);
TBAG_API ParallelDeviceIds getDeviceList(ParallelPlatformId platform);
TBAG_API ParallelDeviceInfo getDeviceInfo(ParallelDeviceId device);

TBAG_API ParallelContextId createContext(ParallelDeviceId device);
TBAG_API bool deleteContext(ParallelContextId context);

} // namespace cpu
} // namespace parallel

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PARALLEL_CPU_CPUDEVICE_HPP__

