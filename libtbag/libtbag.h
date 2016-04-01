/**
 * @file   libtbag.h
 * @brief  libtbag represent header file.
 * @author zer0
 * @date   2016-04-01
 */

#ifndef __INCLUDE_MAIN__LIBMAIN___TEST_LIBRARY___H__
#define __INCLUDE_MAIN__LIBMAIN___TEST_LIBRARY___H__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>

#ifndef TBAG_PREFIX
#define TBAG_PREFIX tbag_
#define TBAG_PREFIX_FUNCTION_IMPLEMENT(prefix, v) prefix##v
#define TBAG_PREFIX_FUNCTION(v) TBAG_PREFIX_FUNCTION_IMPLEMENT(TBAG_PREFIX, v)
#endif

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Obtain libtag version.
 */
char const * const TBAG_PREFIX_FUNCTION(get_version)();

/**
 * Test function.
 */
void TBAG_PREFIX_FUNCTION(__test__)();

#if defined(__cplusplus)
}
#endif

#endif // __INCLUDE_MAIN__LIBMAIN___TEST_LIBRARY___H__

