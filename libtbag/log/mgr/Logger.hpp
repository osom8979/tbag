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
#include <libtbag/log/msg/PacketGenerator.hpp>
#include <libtbag/log/msg/LogMsgPacket.hpp>

#include <libtbag/3rd/fmt/format.h>
#include <libtbag/3rd/fmt/ostream.h>

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
class TBAG_API Logger : private Noncopyable
{
public:
    using LogMsgPacket = msg::LogMsgPacket;
    using Generator    = msg::PacketGenerator;
    using MakeType     = Generator::MakeType;

    using SinkType     = sink::SinkInterface;
    using SinkPtr      = std::unique_ptr<SinkType>;

    using Severity     = level::Severity;
    using String       = std::string;

private:
    SinkPtr   _sink;
    Generator _generator;
    Severity  _severity;

public:
    Logger();
    Logger(SinkType * sink, MakeType type = MakeType::DEFAULT);
    virtual ~Logger();

public:
    // clang-format off
    inline void setSeverity(Severity const & severity) TBAG_NOEXCEPT
    { _severity = severity; }
    inline Severity getSeverity() const TBAG_NOEXCEPT
    { return _severity; }

    inline void setType(Generator::MakeType type) TBAG_NOEXCEPT
    { _generator.setType(type); }
    inline Generator::MakeType getGeneratorMakeType() const TBAG_NOEXCEPT
    { return _generator.getType(); }

    inline void setSink(SinkType * sink) TBAG_NOEXCEPT_SP_OP(_sink.reset(sink))
    { _sink.reset(sink); }
    inline SinkType const * getSink() const TBAG_NOEXCEPT
    { return _sink.get(); }
    // clang-format on

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

