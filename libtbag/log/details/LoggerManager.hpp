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
#include <libtbag/log/details/LoggerInterface.hpp>

#include <unordered_map>
#include <memory>
#include <string>

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

public:
    using LoggerPtr = std::unique_ptr<LoggerInterface>;
    using LoggerMap = std::unordered_map<std::string, LoggerPtr>;
    using LoggerPair = typename LoggerMap::value_type;

private:
    LoggerMap _logs;

public:
    void addLogger(std::string const & name, LoggerInterface * new_logger);
    void removeLogger(std::string const & name);
    std::size_t getSize() const noexcept;
    LoggerInterface * getLoggerPtr() noexcept;
};

} // namespace details
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_DETAILS_LOGGERMANAGER_HPP__

