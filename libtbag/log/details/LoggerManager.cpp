/**
 * @file   LoggerManager.cpp
 * @brief  LoggerManager class implementation.
 * @author zer0
 * @date   2016-07-15
 */

#include <libtbag/log/details/LoggerManager.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log     {
namespace details {

bool LoggerManager::addLogger(std::string const & name, Logger * new_logger)
{
    return _logs.insert(LoggerPair(name, LoggerPtr(new_logger))).second;
}

bool LoggerManager::removeLogger(std::string const & name)
{
    return (_logs.erase(name) >= 1 ? true : false);
}

std::size_t LoggerManager::getSize() const noexcept
{
    return _logs.size();
}

Logger * LoggerManager::getLoggerPtr(std::string const & name)
{
    return _logs.at(name).get();
}

} // namespace details
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

