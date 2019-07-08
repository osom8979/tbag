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
    auto const LOWER = libtbag::string::lower(libtbag::string::trim(type));
    if (LOWER == STDERR_TEXT || LOWER == "2") {
        return ConsoleType::CT_STDERR;
    }
    return ConsoleType::CT_STDOUT;
}

char const * const ConsoleSink::getConsoleTypeName(ConsoleType t) TBAG_NOEXCEPT
{
    // clang-format off
    switch (t) {
    case ConsoleType::CT_STDOUT: return STDOUT_TEXT;
    case ConsoleType::CT_STDERR: return STDERR_TEXT;
    default:                     return "";
    }
    // clang-format on
}

ConsoleSink::ConsoleSink(ConsoleType type)
        : CONSOLE_TYPE(type)
{
    // EMPTY.
}

ConsoleSink::ConsoleSink(std::string const & type)
        : CONSOLE_TYPE(getConsoleType(type))
{
    // EMPTY.
}

ConsoleSink::~ConsoleSink()
{
    // EMPTY.
}

bool ConsoleSink::write(int level, char const * message, int size)
{
    return ::fwrite(message, size, 1, CONSOLE_TYPE == ConsoleType::CT_STDOUT ? stdout : stderr) == 1;
}

void ConsoleSink::flush()
{
    ::fflush(CONSOLE_TYPE == ConsoleType::CT_STDOUT ? stdout : stderr);
}

} // namespace sink
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

