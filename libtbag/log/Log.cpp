/**
 * @file   Log.cpp
 * @brief  Log class implementation.
 * @author zer0
 * @date   2016-07-17
 */

#include <libtbag/log/Log.hpp>
#include <libtbag/log/details/LoggerManager.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log {

using LogMgr = ::libtbag::log::details::LoggerManager;
using Logger = ::libtbag::log::details::Logger;

Logger * createDefaultConsoleLogger()
{
    try {
        // LogMgr::getInstance()->addLogger(TBAG_DEFAULT_LOGGER_NAME, );
    } catch (...) {
        // EMPTY.
    }
    return nullptr;
}

Logger * createDefaultFileLogger(std::string const & path, bool auto_flush)
{
    try {
    } catch (...) {
        // EMPTY.
    }
    return nullptr;
}

Logger * getDefaultLogger()
{
    try {
    } catch (...) {
        // EMPTY.
    }
    return nullptr;
}

void setDefaultLevel(LogLevel level)
{
}

} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

