/**
 * @file   Log.cpp
 * @brief  Log class implementation.
 * @author zer0
 * @date   2016-07-17
 */

#include <libtbag/log/Log.hpp>

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

bool removeLogger(LoggerId id)
{
    return LoggerManager::getInstance()->removeLogger(id);
}

bool removeLogger(std::string const & name)
{
    return removeLogger(getLoggerId(name));
}

bool removeDefaultLogger()
{
    return removeLogger(TBAG_DEFAULT_LOGGER_NAME);
}

Logger * getLogger(LoggerId id)
{
    return LoggerManager::getInstance()->getLoggerPtr(id);
}

Logger * getLogger(std::string const & name)
{
    return getLogger(getLoggerId(name));
}

Logger * getDefaultLogger()
{
    return getLogger(TBAG_DEFAULT_LOGGER_NAME);
}

bool existsLogger(LoggerId id)
{
    return getLogger(id) != nullptr;
}

bool existsLogger(std::string const & name)
{
    return getLogger(name) != nullptr;
}

void setSeverity(LoggerId id, Severity const & level)
{
    Logger * logger = getLogger(id);
    if (logger != nullptr) {
        logger->setSeverity(level);
    }
}

void setSeverity(std::string const & name, Severity const & level)
{
    setSeverity(getLoggerId(name), level);
}

void setDefaultSeverity(Severity const & level)
{
    setSeverity(TBAG_DEFAULT_LOGGER_NAME, level);
}

void setLevel(LoggerId id, int level)
{
    Logger * logger = getLogger(id);
    if (logger != nullptr) {
        logger->setLevel(level);
    }
}

void setLevel(std::string const & name, int level)
{
    setLevel(getLoggerId(name), level);
}

void setDefaultLevel(int level)
{
    setLevel(TBAG_DEFAULT_LOGGER_NAME, level);
}

Severity getSeverity(LoggerId id)
{
    Logger * logger = getLogger(id);
    if (logger != nullptr) {
        return logger->getSeverity();
    }
    return OFF_SEVERITY;
}

Severity getSeverity(std::string const & name)
{
    return getSeverity(getLoggerId(name));
}

Severity getDefaultSeverity()
{
    return getSeverity(TBAG_DEFAULT_LOGGER_NAME);
}

void setAutoFlush(LoggerId id, bool flag)
{
    Logger * logger = getLogger(id);
    if (logger != nullptr) {
        logger->setAutoFlush(flag);
    }
}

void setAutoFlush(std::string const & name, bool flag)
{
    setAutoFlush(getLoggerId(name), flag);
}

void setDefaultAutoFlush(bool flag)
{
    setAutoFlush(TBAG_DEFAULT_LOGGER_NAME, flag);
}

bool getAutoFlush(LoggerId id)
{
    Logger * logger = getLogger(id);
    if (logger != nullptr) {
        return logger->getAutoFlush();
    }
    return false;
}

bool getAutoFlush(std::string const & name)
{
    return getAutoFlush(getLoggerId(name));
}

bool getDefaultAutoFlush()
{
    return getAutoFlush(TBAG_DEFAULT_LOGGER_NAME);
}

} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

