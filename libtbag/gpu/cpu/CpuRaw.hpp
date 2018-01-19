/**
 * @file   CpuRaw.hpp
 * @brief  CpuRaw class prototype.
 * @author zer0
 * @date   2018-01-10
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_CPU_CPURAW_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_CPU_CPURAW_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/type/TypeTable.hpp>
#include <libtbag/gpu/details/GpuDetails.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu {
namespace cpu {

TBAG_API bool runCpuAdd(type::TypeTable type, void const * v1, void const * v2, void * r, int count);

} // namespace cpu
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_CPU_CPURAW_HPP__

