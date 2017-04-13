/**
 * @file   Logger.hpp
 * @brief  Logger class prototype.
 * @author zer0
 * @date   2017-04-13
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_MGR_LOGGER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_MGR_LOGGER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/log/level/Severity.hpp>
#include <libtbag/log/sink/Sink.hpp>

#include <libtbag/3rd/fmt/format.h>

#include <string>
#include <memory>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log {
namespace mgr {

/**
 * Logger class prototype.
 *
 * @author zer0
 * @date   2016-07-15
 */
class TBAG_API Logger : public Noncopyable
{
public:
    using SinkType  = sink::SinkInterface;
    using SinkPtr   = std::unique_ptr<SinkType>;
    using Severity  = level::Severity;
    using String    = std::string;

private:
    SinkPtr  _sink;
    Severity _severity;

public:
    Logger();
    Logger(SinkType * sink);
    ~Logger();

public:
    // @formatter:off
    inline void setSeverity(Severity const & severity) TBAG_NOEXCEPT
    { _severity = severity; }
    inline Severity getSeverity() const TBAG_NOEXCEPT
    { return _severity; }
    // @formatter:on

public:
    void write(Severity const & severity, String const & message);
    void flush();

public:
    template <typename ... Args>
    void logf(Severity const & severity, std::string const & format, Args && ... args)
    {
        this->write(severity, ::fmt::format(format, std::forward<Args>(args) ...));
    }
};

} // namespace mgr
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_MGR_LOGGER_HPP__

