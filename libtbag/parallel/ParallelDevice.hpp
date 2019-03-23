/**
 * @file   ParallelDevice.hpp
 * @brief  Parallel Computing Device class prototype.
 * @author zer0
 * @date   2019-02-21
 * @date   2019-03-23 (Rename: PcDevice -> ParallelDevice)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PARALLEL_PARALLELDEVICE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PARALLEL_PARALLELDEVICE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/parallel/ParallelCommon.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace parallel {

TBAG_API bool isSupport(ParallelTypeId type) TBAG_NOEXCEPT;

TBAG_API int getPlatformCount(ParallelTypeId type);
TBAG_API ParallelPlatformIds getPlatformList(ParallelTypeId type);
TBAG_API ParallelPlatformInfo getPlatformInfo(ParallelTypeId type, ParallelPlatformId platform);

TBAG_API int getDeviceCount(ParallelTypeId type, ParallelPlatformId platform);
TBAG_API ParallelDeviceIds getDeviceList(ParallelTypeId type, ParallelPlatformId platform);
TBAG_API ParallelDeviceInfo getDeviceInfo(ParallelTypeId type, ParallelDeviceId device);

TBAG_API ParallelContextId createContext(ParallelTypeId type, ParallelDeviceId device);
TBAG_API bool deleteContext(ParallelTypeId type, ParallelContextId context);

TBAG_API ParallelType getParallelType(ParallelTypeId type);
TBAG_API ParallelPlatform getParallelPlatform(ParallelType type, ParallelPlatformId platform);
TBAG_API ParallelDevice getParallelDevice(ParallelPlatform platform, ParallelDeviceId device);
TBAG_API ParallelContext getParallelContext(ParallelDevice device, ParallelContextId context);

} // namespace parallel

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PARALLEL_PARALLELDEVICE_HPP__

