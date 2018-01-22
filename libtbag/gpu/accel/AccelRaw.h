/**
 * @file   AccelRaw.h
 * @brief  AccelRaw raw prototype.
 * @author zer0
 * @date   2018-01-19
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_ACCEL_ACCELRAW_H__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_ACCEL_ACCELRAW_H__

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

TBAG_API tbBOOL tbAccelAdd_i(int      const * v1, int      const * v2, int      * r, int count);
TBAG_API tbBOOL tbAccelAdd_u(unsigned const * v1, unsigned const * v2, unsigned * r, int count);
TBAG_API tbBOOL tbAccelAdd_f(float    const * v1, float    const * v2, float    * r, int count);
TBAG_API tbBOOL tbAccelAdd_d(double   const * v1, double   const * v2, double   * r, int count);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_ACCEL_ACCELRAW_H__

