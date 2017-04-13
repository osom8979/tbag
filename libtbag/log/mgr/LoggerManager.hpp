/**
 * @file   LoggerManager.hpp
 * @brief  LoggerManager class prototype.
 * @author zer0
 * @date   2017-04-13
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_MGR_LOGGERMANAGER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_MGR_LOGGERMANAGER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/pattern/Singleton2.hpp>
#include <libtbag/log/mgr/Logger.hpp>

#include <unordered_map>
#include <memory>
#include <string>


// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log {
namespace mgr {

/**
 * LoggerManager class prototype.
 *
 * @author zer0
 * @date   2016-07-15
 *
 * @remarks
 *  - Fix the C2280 error in MSVC14: Requires inheritance Noncopyable class.
 */
class TBAG_API LoggerManager : public pattern::Singleton2<LoggerManager>
{
public:
    SINGLETON2_PROTOTYPE(LoggerManager);

public:
    using LoggerPtr  = std::unique_ptr<Logger>;
    using LoggerMap  = std::unordered_map<std::string, LoggerPtr>;
    using LoggerPair = LoggerMap::value_type;

private:
    LoggerMap _logs;

public:
    bool addLogger(std::string const & name, Logger * new_logger);
    bool removeLogger(std::string const & name);
    std::size_t getSize() const TBAG_NOEXCEPT;
    Logger * getLoggerPtr(std::string const & name);
};

} // namespace mgr
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_MGR_LOGGERMANAGER_HPP__

