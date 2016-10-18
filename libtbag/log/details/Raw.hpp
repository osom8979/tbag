/**
 * @file   Raw.hpp
 * @brief  Raw logging methods prototype.
 * @author zer0
 * @date   2016-07-09
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_DETAILS_RAW_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_DETAILS_RAW_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/macro/attributes.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log     {
namespace details {

TBAG_EXPORTS bool formatToMemory(char * buf, int size, char const * format, ...);

} // namespace details
} //namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_DETAILS_RAW_HPP__

