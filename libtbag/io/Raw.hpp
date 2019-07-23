/**
 * @file   Raw.hpp
 * @brief  Raw class prototype.
 * @author zer0
 * @date   2017-04-13
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_IO_RAW_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_IO_RAW_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace io {

TBAG_API bool formatToMemory(char * buf, int size, char const * format, ...);

TBAG_API int putStdout(char const * TBAG_RESTRICT text);
TBAG_API int putStderr(char const * TBAG_RESTRICT text);

} // namespace io

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_IO_RAW_HPP__

