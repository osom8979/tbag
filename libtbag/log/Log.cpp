/**
 * @file   Log.cpp
 * @brief  Log class implementation.
 * @author zer0
 * @date   2016-07-17
 */

#include <libtbag/log/Log.hpp>
#include <libtbag/lock/UvLock.hpp>

#include <cstdio>
#include <cassert>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log {

Logger * createDefaultRawStdoutLogger()
{
    return createRawStdoutLogger(TBAG_DEFAULT_LOGGER_NAME);
}

Logger * createDefaultColorStdoutLogger()
{
    return createColorStdoutLogger(TBAG_DEFAULT_LOGGER_NAME);
}

Logger * createDefaultStdoutLogger()
{
    return createStdoutLogger(TBAG_DEFAULT_LOGGER_NAME);
}

Logger * createDefaultFileLogger(std::string const & path)
{
    return createFileLogger(TBAG_DEFAULT_LOGGER_NAME, path);
}

bool removeDefaultLogger()
{
    return removeLogger(TBAG_DEFAULT_LOGGER_NAME);
}

bool existsDefaultLogger()
{
    return existsLogger(TBAG_DEFAULT_LOGGER_NAME);
}

Logger * getDefaultLogger()
{
    return getLogger(TBAG_DEFAULT_LOGGER_NAME);
}

void setDefaultSeverity(Severity const & level)
{
    setSeverity(TBAG_DEFAULT_LOGGER_NAME, level);
}

void setDefaultLevel(int level)
{
    setLevel(TBAG_DEFAULT_LOGGER_NAME, level);
}

Severity getDefaultSeverity()
{
    return getSeverity(TBAG_DEFAULT_LOGGER_NAME);
}

void setDefaultAutoFlush(bool flag)
{
    setAutoFlush(TBAG_DEFAULT_LOGGER_NAME, flag);
}

bool getDefaultAutoFlush()
{
    return getAutoFlush(TBAG_DEFAULT_LOGGER_NAME);
}

void __debug_message(char const * TBAG_RESTRICT text)
{
    assert(text != nullptr);
#if defined(ENABLE_TBAG_LIBRARY_DEBUGGING_LOG_LOCK)
    static libtbag::lock::UvLock __lock__;
    __lock__.lock();
#endif
    ::fputs(text, stderr);
#if defined(ENABLE_TBAG_LIBRARY_DEBUGGING_LOG_LOCK)
    __lock__.unlock();
#endif
}

} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

