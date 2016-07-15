/**
 * @file   LoggerManager.hpp
 * @brief  LoggerManager class prototype.
 * @author zer0
 * @date   2016-07-15
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_DETAILS_LOGGERMANAGER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_DETAILS_LOGGERMANAGER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/pattern/Singleton.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log     {
namespace details {

/**
 * LoggerManager class prototype.
 *
 * @author zer0
 * @date   2016-07-15
 */
class LoggerManager : SINGLETON_INHERITANCE(LoggerManager)
{
public:
    SINGLETON_RESTRICT(LoggerManager);
};

} // namespace details
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_DETAILS_LOGGERMANAGER_HPP__

