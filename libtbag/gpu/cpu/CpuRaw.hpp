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

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu {
namespace cpu {

TBAG_API bool runCpuAdd1i(int      const * v1, int      const * v2, int      * r, int count);
TBAG_API bool runCpuAdd1u(unsigned const * v1, unsigned const * v2, unsigned * r, int count);
TBAG_API bool runCpuAdd1f(float    const * v1, float    const * v2, float    * r, int count);
TBAG_API bool runCpuAdd1d(double   const * v1, double   const * v2, double   * r, int count);

} // namespace cpu
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_CPU_CPURAW_HPP__

