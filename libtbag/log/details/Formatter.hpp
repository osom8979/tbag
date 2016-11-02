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
#include <libtbag/3rd/fmt/format.h>

#include <string>
#include <utility>
#include <chrono>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log     {
namespace details {

// Forward declaration.
class Severity;
enum class LogLevel;

TBAG_API std::string getMillisecFormat(std::chrono::system_clock::time_point const & time_point);
TBAG_API std::string getDefaultPrefix();
TBAG_API std::string getDefaultSeverityString(Severity const & severity);

template <typename ... Args>
inline std::string format(std::string const & format_string, Args && ... args)
{
    return fmt::format(format_string, std::forward<Args>(args) ...);
}

/**
 * Formatter class prototype.
 *
 * @author zer0
 * @date   2016-07-18
 */
class TBAG_API Formatter : public Noncopyable
{
public:
    TBAG_CONSTEXPR Formatter()
    { /* EMPTY. */ }
    ~Formatter()
    { /* EMPTY. */ }

public:
    std::string getDefaultPrefix(Severity const & severity) const;
    std::string getDefaultPrefix(LogLevel level) const;

public:
    template <typename ... Args>
    inline std::string format(std::string const & format_string, Args && ... args) const
    {
        return libtbag::log::details::format(format_string, std::forward<Args>(args) ...);
    }
};

} // namespace details
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_DETAILS_FORMATTER_HPP__

