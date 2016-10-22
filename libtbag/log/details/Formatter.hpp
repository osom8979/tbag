/**
 * @file   Formatter.hpp
 * @brief  Formatter class prototype.
 * @author zer0
 * @date   2016-07-18
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_DETAILS_FORMATTER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_DETAILS_FORMATTER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/string/fmt/format.h>
#include <libtbag/string/Strings.hpp>
#include <libtbag/time/Time.hpp>
#include <libtbag/log/details/Severity.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log     {
namespace details {

template <typename CharType, typename ... Args>
inline std::basic_string<CharType> format(std::basic_string<CharType> const & format_string, Args && ... args)
{
    return fmt::format(format_string, std::forward<Args>(args) ...);
}

template <typename CharType>
std::basic_string<CharType> getMillisecFormat(std::chrono::system_clock::time_point const & time_point)
{
    std::basic_string<CharType> result;
    time::getMillisecString(time_point, result);
    return std::basic_string<CharType>(CHAR_OR_WIDECHAR(CharType, ",")) + result;
}

template <typename CharType>
std::basic_string<CharType> getDefaultPrefix()
{
    std::basic_stringstream<CharType> ss;

    // Timestamp.
    auto tp = time::getNowSystemClock();
    tm time = {0,};

    if (time::getLocalTime(time::getTime(tp), &time)) {
        ss << time::getFormatString(time::getDefaultTimestampLongFormat<CharType>(), &time)
           << ::libtbag::log::details::getMillisecFormat<CharType>(tp); // Milliseconds.
    } else {
        ss << CHAR_OR_WIDECHAR(CharType, "SINCE")
           << tp.time_since_epoch().count();
    }

    // Current thread.
    ss << CHAR_OR_WIDECHAR(CharType, " @")
       << std::this_thread::get_id();

    return ss.str();
}

template <typename CharType>
std::basic_string<CharType> getDefaultSeverityString(Severity const & severity)
{
    std::basic_string<CharType> result;
    result += CHAR_OR_WIDECHAR(CharType, " [");
    result += severity.getText();
    result += CHAR_OR_WIDECHAR(CharType, "] ");
    return result;
}

/**
 * Formatter class prototype.
 *
 * @author zer0
 * @date   2016-07-18
 */
template <typename CharType = char>
class BaseFormatter : public Noncopyable
{
public:
    using ValueType = CharType;
    using String    = std::basic_string<ValueType>;

public:
    BaseFormatter() = default;
    ~BaseFormatter() = default;

public:
    template <typename ... Args>
    inline String format(String const & format_string, Args && ... args) const
    {
        return ::libtbag::log::details::format<ValueType>(format_string, std::forward<Args>(args) ...);
    }

    inline String getDefaultPrefix(Severity const & severity) const
    {
        return ::libtbag::log::details::getDefaultPrefix<ValueType>()
             + ::libtbag::log::details::getDefaultSeverityString<ValueType>(severity);
    }

    inline String getDefaultPrefix(LogLevel level) const
    {
        return this->getDefaultPrefix(Severity(level));
    }
};

using Formatter     = BaseFormatter<char>;
using WideFormatter = BaseFormatter<wchar_t>;

} // namespace details
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_DETAILS_FORMATTER_HPP__

