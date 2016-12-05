/**
 * @file   Formatter.cpp
 * @brief  Formatter class implementation.
 * @author zer0
 * @date   2016-10-29
 */

#include <libtbag/log/details/Formatter.hpp>
#include <libtbag/log/details/Severity.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/time/Time.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log     {
namespace details {

std::string getMillisecFormat(std::chrono::system_clock::time_point const & time_point)
{
    std::string result;
    time::getMillisecString(time_point, result);
    return std::string(",") + result;
}

std::string getDefaultPrefix()
{
    std::stringstream ss;

    // Timestamp.
    auto tp = time::getNowSystemClock();
    tm time = {0,};

    if (time::getLocalTime(time::getTime(tp), &time)) {
        ss << time::getFormatString(time::TIMESTAMP_LONG_FORMAT, &time)
           << libtbag::log::details::getMillisecFormat(tp); // Milliseconds.
    } else {
        ss << "SINCE" << tp.time_since_epoch().count();
    }

    // Current thread.
    ss << " @" << std::this_thread::get_id();

    return ss.str();
}

// -------------------------
// Formatter implementation.
// -------------------------

std::string getDefaultSeverityString(Severity const & severity)
{
    return std::string(" [") + severity.text + "] ";
}

std::string Formatter::getDefaultPrefix(Severity const & severity) const
{
    return libtbag::log::details::getDefaultPrefix() + libtbag::log::details::getDefaultSeverityString(severity);
}

std::string Formatter::getDefaultPrefix(LogLevel level) const
{
    return getDefaultPrefix(Severity(level));
}

} // namespace details
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

