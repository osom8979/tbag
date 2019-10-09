/**
 * @file   ConsoleSink.cpp
 * @brief  ConsoleSink class implementation.
 * @author zer0
 * @date   2019-07-02
 */

#include <libtbag/log/sink/ConsoleSink.hpp>
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
    if (type_text == STDOUT_TEXT || type_text == "1") {
        return ConsoleType::CT_STDOUT;
    } else if (type_text == STDERR_TEXT || type_text == "2") {
        return ConsoleType::CT_STDERR;
    }
    return ConsoleType::CT_UNKNOWN;
}

char const * ConsoleSink::getConsoleTypeName(ConsoleType t) TBAG_NOEXCEPT
{
    switch (t) {
    case ConsoleType::CT_STDOUT:
        return STDOUT_TEXT;
    case ConsoleType::CT_STDERR:
        return STDERR_TEXT;
    default:
        return "";
    }
}

// -----------
// ConsoleSink
// -----------

ConsoleSink::ConsoleSink(ConsoleType type)
        : CONSOLE_TYPE(type)
{
    if (CONSOLE_TYPE == ConsoleType::CT_UNKNOWN) {
        throw std::bad_alloc();
    }
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
    case ConsoleType::CT_UNKNOWN:
        return false;
    default:
        return false;
    }
}

void ConsoleSink::flush()
{
    switch (CONSOLE_TYPE) {
    case ConsoleType::CT_STDOUT:
        ::fflush(stdout);
    case ConsoleType::CT_STDERR:
        ::fflush(stderr);
    default:
        break;
    }
}

} // namespace sink
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

