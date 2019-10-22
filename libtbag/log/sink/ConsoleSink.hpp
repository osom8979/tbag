/**
 * @file   ConsoleSink.hpp
 * @brief  ConsoleSink class prototype.
 * @author zer0
 * @date   2016-07-21
 * @date   2019-07-02 (Create a cpp file, Rename: CoutSink -> ConsoleSink)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_CONSOLESINK_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_CONSOLESINK_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/log/sink/Sink.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log  {
namespace sink {

/**
 * ConsoleSink class prototype.
 *
 * @author zer0
 * @date   2016-07-21
 * @date   2019-07-02 (Create a cpp file, Rename: CoutSink -> ConsoleSink)
 */
class TBAG_API ConsoleSink : public Sink
{
public:
    enum class ConsoleType
    {
        CT_AUTO,
        CT_STDOUT,
        CT_STDERR,
    };

public:
    TBAG_CONSTEXPR static char const * sink_name() TBAG_NOEXCEPT
    { return "console"; }

public:
    TBAG_CONSTEXPR static char const * const AUTO_TEXT = "auto";

    TBAG_CONSTEXPR static char const * const STDOUT_TEXT = "stdout";
    TBAG_CONSTEXPR static char const * const STDOUT_TEXT2 = "out";
    TBAG_CONSTEXPR static char const * const STDOUT_TEXT3 = "1";

    TBAG_CONSTEXPR static char const * const STDERR_TEXT = "stderr";
    TBAG_CONSTEXPR static char const * const STDERR_TEXT2 = "err";
    TBAG_CONSTEXPR static char const * const STDERR_TEXT3 = "2";

public:
    static ConsoleType getConsoleType(std::string const & type);
    static char const * getConsoleTypeName(ConsoleType t) TBAG_NOEXCEPT;

public:
    ConsoleType const CONSOLE_TYPE;

public:
    ConsoleSink(ConsoleType type = ConsoleType::CT_STDOUT);
    ConsoleSink(std::string const & arguments);
    virtual ~ConsoleSink();

public:
    bool write(int level, char const * message, int size) override;
    void flush() override;
};

} // namespace sink
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_CONSOLESINK_HPP__

