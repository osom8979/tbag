/**
 * @file   PcDevice.hpp
 * @brief  Parallel Computing Device class prototype.
 * @author zer0
 * @date   2019-02-21
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PARALLEL_PCDEVICE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PARALLEL_PCDEVICE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/parallel/PcCommon.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace parallel {

TBAG_API bool isSupport(PcTypeId type) TBAG_NOEXCEPT;

TBAG_API int getPlatformCount(PcTypeId type);
TBAG_API PcPlatformIds getPlatformList(PcTypeId type);
TBAG_API PcPlatformInfo getPlatformInfo(PcTypeId type, PcPlatformId platform);

TBAG_API int getDeviceCount(PcTypeId type, PcPlatformId platform);
TBAG_API PcDeviceIds getDeviceList(PcTypeId type, PcPlatformId platform);
TBAG_API PcDeviceInfo getDeviceInfo(PcTypeId type, PcDeviceId device);

TBAG_API PcContextId createContext(PcTypeId type, PcDeviceId device);
TBAG_API bool deleteContext(PcTypeId type, PcContextId context);

TBAG_API PcType getPcType(PcTypeId type);
TBAG_API PcPlatform getPcPlatform(PcType type, PcPlatformId platform);
TBAG_API PcDevice getPcDevice(PcPlatform platform, PcDeviceId device);
TBAG_API PcContext getPcContext(PcDevice device, PcContextId context);

} // namespace parallel

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PARALLEL_PCDEVICE_HPP__

