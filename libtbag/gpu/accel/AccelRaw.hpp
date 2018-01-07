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

#ifndef TB_ACCEL_TRUE
#define TB_ACCEL_TRUE 1
#endif

#ifndef TB_ACCEL_FALSE
#define TB_ACCEL_FALSE 0
#endif

#if defined(__cplusplus)
extern "C" {
#endif

TBAG_API int tbAccelAdd1f(float  const * v1, float  const * v2, float  * result, int count);
TBAG_API int tbAccelAdd1d(double const * v1, double const * v2, double * result, int count);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_ACCEL_ACCELRAW_HPP__

