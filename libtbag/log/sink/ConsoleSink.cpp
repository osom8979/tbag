/**
 * @file   ConsoleSink.cpp
 * @brief  ConsoleSink class implementation.
 * @author zer0
 * @date   2019-07-02
 */

#include <libtbag/log/sink/ConsoleSink.hpp>
#include <libtbag/log/Severity.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <cstdio>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log  {
namespace sink {

ConsoleSink::ConsoleType ConsoleSink::getConsoleType(std::string const & type)
{
    using namespace libtbag::string;
    auto const type_text = lower(trim(type));
    if (type_text == STDOUT_TEXT || type_text == STDOUT_TEXT2 || type_text == STDOUT_TEXT3) {
        return ConsoleType::CT_STDOUT;
    } else if (type_text == STDERR_TEXT || type_text == STDERR_TEXT2 || type_text == STDERR_TEXT3) {
        return ConsoleType::CT_STDERR;
    }
    return ConsoleType::CT_AUTO;
}

char const * ConsoleSink::getConsoleTypeName(ConsoleType t) TBAG_NOEXCEPT
{
    switch (t) {
    case ConsoleType::CT_STDOUT:
        return STDOUT_TEXT;
    case ConsoleType::CT_STDERR:
        return STDERR_TEXT;
    default:
        return AUTO_TEXT;
    }
}

// -----------
// ConsoleSink
// -----------

ConsoleSink::ConsoleSink(ConsoleType type)
        : CONSOLE_TYPE(type)
{
    // EMPTY.
}

ConsoleSink::ConsoleSink(std::string const & arguments)
        : ConsoleSink(getConsoleType(arguments))
{
    // EMPTY.
}

ConsoleSink::~ConsoleSink()
{
    // EMPTY.
}

bool ConsoleSink::write(int level, char const * message, int size)
{
    switch (CONSOLE_TYPE) {
    case ConsoleType::CT_STDOUT:
        return ::fwrite(message, size, 1, stdout) == 1;

    case ConsoleType::CT_STDERR:
        return ::fwrite(message, size, 1, stderr) == 1;

    case ConsoleType::CT_AUTO:
        switch (level) {
        case OFF_LEVEL:
            return true;

        case EMERGENCY_LEVEL:
            TBAG_FALLTHROUGH
        case ALERT_LEVEL:
            TBAG_FALLTHROUGH
        case CRITICAL_LEVEL:
            TBAG_FALLTHROUGH
        case ERROR_LEVEL:
            return ::fwrite(message, size, 1, stderr) == 1;

        case WARNING_LEVEL:
            TBAG_FALLTHROUGH
        case NOTICE_LEVEL:
            TBAG_FALLTHROUGH
        case INFO_LEVEL:
            TBAG_FALLTHROUGH
        case DEBUG_LEVEL:
            TBAG_FALLTHROUGH
        case UNKNOWN_LEVEL:
            return ::fwrite(message, size, 1, stdout) == 1;

        default:
            return false;
        }

    default:
        return false;
    }
}

void ConsoleSink::flush()
{
    switch (CONSOLE_TYPE) {
    case ConsoleType::CT_STDOUT:
        ::fflush(stdout);
        break;

    case ConsoleType::CT_STDERR:
        ::fflush(stderr);
        break;

    case ConsoleType::CT_AUTO:
        ::fflush(stdout);
        ::fflush(stderr);
        break;

    default:
        break;
    }
}

} // namespace sink
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

