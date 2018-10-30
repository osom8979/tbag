/**
 * @file   TbuiLog.cpp
 * @brief  TbuiLog class implementation.
 * @author zer0
 * @date   2018-10-30
 */

#include <libtbui/log/TbuiLog.hpp>

namespace libtbui {
namespace log     {

Logger * createColorStdoutLogger()
{
    return libtbag::log::createStdoutLogger(DEFAULT_LOGGER_NAME,
                                            libtbag::log::MakeType::DEFAULT_COLOR,
                                            TBUI_LOG_MUTEX_FLAG, TBUI_LOG_FLUSH_FLAG);
}

Logger * createRawStdoutLogger()
{
    return libtbag::log::createStdoutLogger(DEFAULT_LOGGER_NAME,
                                            libtbag::log::MakeType::RAW,
                                            TBUI_LOG_MUTEX_FLAG, TBUI_LOG_FLUSH_FLAG);
}

Logger * createStdoutLogger()
{
    return libtbag::log::createStdoutLogger(DEFAULT_LOGGER_NAME,
                                            libtbag::log::MakeType::DEFAULT,
                                            TBUI_LOG_MUTEX_FLAG, TBUI_LOG_FLUSH_FLAG);
}

Logger * getLogger()
{
    return libtbag::log::getLogger(DEFAULT_LOGGER_NAME);
}

bool removeLogger()
{
    return libtbag::log::removeLogger(DEFAULT_LOGGER_NAME);
}

void setLevel(int level)
{
    libtbag::log::setLevel(DEFAULT_LOGGER_NAME, level);
}

void setSeverity(Severity level)
{
    libtbag::log::setSeverity(DEFAULT_LOGGER_NAME, level);
}

} // namespace log
} // namespace libtbui

