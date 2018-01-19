/**
 * @file   AccelRaw.hpp
 * @brief  AccelRaw class prototype.
 * @author zer0
 * @date   2018-01-08
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_ACCEL_ACCELRAW_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_ACCEL_ACCELRAW_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/macro/attributes.hpp>
#include <libtbag/macro/types.hpp>

#if defined(__cplusplus)
extern "C" {
#endif

TBAG_API tbBOOL tbAccelAdd1i(int      const * v1, int      const * v2, int      * r, unsigned count);
TBAG_API tbBOOL tbAccelAdd1u(unsigned const * v1, unsigned const * v2, unsigned * r, unsigned count);
TBAG_API tbBOOL tbAccelAdd1f(float    const * v1, float    const * v2, float    * r, unsigned count);
TBAG_API tbBOOL tbAccelAdd1d(double   const * v1, double   const * v2, double   * r, unsigned count);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_ACCEL_ACCELRAW_HPP__

