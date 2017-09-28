/**
 * @file   LoggerManager.cpp
 * @brief  LoggerManager class implementation.
 * @author zer0
 * @date   2017-04-13
 */

#include <libtbag/log/mgr/LoggerManager.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log {
namespace mgr {

bool LoggerManager::addLogger(std::string const & name, Logger * new_logger)
{
    return _logs.insert(LoggerPair(name, LoggerPtr(new_logger))).second;
}

bool LoggerManager::removeLogger(std::string const & name)
{
    return (_logs.erase(name) >= 1 ? true : false);
}

std::size_t LoggerManager::getSize() const TBAG_NOEXCEPT
{
    return _logs.size();
}

Logger * LoggerManager::getLoggerPtr(std::string const & name)
{
    // Don't use the _logs.at() method.
    // This is to avoid the std::out_of_range exception.

    auto itr = _logs.find(name);
    if (itr == _logs.end()) {
        return nullptr;
    }
    return itr->second.get();
}

} // namespace mgr
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

