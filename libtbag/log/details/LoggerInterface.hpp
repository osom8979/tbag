/**
 * @file   LoggerInterface.hpp
 * @brief  Logger interface.
 * @author zer0
 * @date   2016-07-15
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_DETAILS_LOGGERINTERFACE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_DETAILS_LOGGERINTERFACE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log     {
namespace details {

/**
 * Logger interface.
 *
 * @author zer0
 * @date   2016-07-15
 */
class LoggerInterface
{
public:
    constexpr LoggerInterface() = default;
    virtual ~LoggerInterface() = default;
};

} // namespace details
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_DETAILS_LOGGERINTERFACE_HPP__

