/**
 * @file   ClDevice.hpp
 * @brief  ClDevice class prototype.
 * @author zer0
 * @date   2019-02-22
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PARALLEL_CL_CLDEVICE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PARALLEL_CL_CLDEVICE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/parallel/PcCommon.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace parallel {
namespace cl       {

TBAG_API bool isSupport() TBAG_NOEXCEPT;

TBAG_API int getPlatformCount();
TBAG_API PcPlatformIds getPlatformList();
TBAG_API PcPlatformInfo getPlatformInfo(PcPlatformId platform);

TBAG_API int getDeviceCount(PcPlatformId platform);
TBAG_API PcDeviceIds getDeviceList(PcPlatformId platform);
TBAG_API PcDeviceInfo getDeviceInfo(PcDeviceId device);

TBAG_API PcContextId createContext(PcDeviceId device);
TBAG_API bool deleteContext(PcContextId context);

} // namespace cl
} // namespace parallel

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PARALLEL_CL_CLDEVICE_HPP__

