/**
 * @file   DefaultColorGenerator.cpp
 * @brief  DefaultColorGenerator class implementation.
 * @author zer0
 * @date   2019-07-02
 */

#include <libtbag/log/msg/DefaultColorGenerator.hpp>
#include <libtbag/log/msg/DefaultMsg.hpp>
#include <libtbag/log/Severity.hpp>
#include <libtbag/algorithm/MinMax.hpp>
#include <libtbag/string/Format.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/time/TimePoint.hpp>
#include <libtbag/tty/Tces.hpp>

#include <cassert>
#include <cstring>

#include <thread>
#include <sstream>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log {
namespace msg {

DefaultColorGenerator::DefaultColorGenerator(LineFeedStyle line_feed)
        : Generator(line_feed)
{
    // EMPTY.
}

DefaultColorGenerator::DefaultColorGenerator(std::string const & line_feed)
        : Generator(line_feed)
{
    // EMPTY.
}

DefaultColorGenerator::~DefaultColorGenerator()
{
    // EMPTY.
}

#ifndef _EMERGENCY_ATTRIBUTE_BG
#define _EMERGENCY_ATTRIBUTE_BG \
    TBAG_TTY_DISPLAY_ATTRIBUTE_BG_RED \
    TBAG_TTY_DISPLAY_ATTRIBUTE_FG_CYAN
#endif

inline static char const * __get_color_prefix(int level) TBAG_NOEXCEPT
{
    using namespace libtbag::tty;
    // clang-format off
    switch (level) {
    case       OFF_LEVEL: return TBAG_EMPTY_STRING;
    case EMERGENCY_LEVEL: return _EMERGENCY_ATTRIBUTE_BG;
    case     ALERT_LEVEL: return DISPLAY_ATTRIBUTE_FG_MAGENTA;
    case  CRITICAL_LEVEL: return DISPLAY_ATTRIBUTE_FG_RED;
    case     ERROR_LEVEL: return DISPLAY_ATTRIBUTE_FG_RED;
    case   WARNING_LEVEL: return DISPLAY_ATTRIBUTE_FG_YELLOW;
    case    NOTICE_LEVEL: return DISPLAY_ATTRIBUTE_FG_GREEN;
    case      INFO_LEVEL: return DISPLAY_ATTRIBUTE_FG_GREEN;
    case     DEBUG_LEVEL: return DISPLAY_ATTRIBUTE_FG_BLUE;
    case   UNKNOWN_LEVEL: return TBAG_EMPTY_STRING;
    default: /*--------*/ return TBAG_EMPTY_STRING;
    }
    // clang-format on
}

inline static char const * __get_color_suffix(int level) TBAG_NOEXCEPT
{
    if (EMERGENCY_LEVEL <= COMPARE_AND(level) <= DEBUG_LEVEL) {
        return libtbag::tty::DISPLAY_ATTRIBUTE_RESET;
    } else {
        return TBAG_EMPTY_STRING;
    }
}

int DefaultColorGenerator::getPaddingByte() const
{
    // clang-format on
    using namespace libtbag::string;
    TBAG_CONSTEXPR static auto const padding_byte =
            string_length(_EMERGENCY_ATTRIBUTE_BG) + // NOLINT
            string_length(DEFAULT_MSG_PREFIX_SAMPLE) +
            string_length(TBAG_TTY_DISPLAY_ATTRIBUTE_RESET) +
            string_length(TBAG_WINDOWS_NEW_LINE);
    return padding_byte;
    // clang-format off
}

#undef _EMERGENCY_ATTRIBUTE_BG

int DefaultColorGenerator::make(char * buffer, int buffer_size,
                                char const * UNUSED_PARAM(logger),
                                int level, char const * UNUSED_PARAM(level_name),
                                char const * msg, int msg_size) const
{
    assert(buffer != nullptr);
    assert(buffer_size >= 1);
    assert(msg != nullptr);
    assert(msg_size >= 1);

    std::stringstream ss;
    ss << __get_color_prefix(level);
    makeDefaultPrefix(ss, level);
    ss.write(msg, msg_size);
    ss << __get_color_suffix(level) << LINE_FEED_STR;

    auto const result_message = ss.str();
    auto const result_message_size = result_message.size();
    if (result_message_size <= buffer_size) {
        ::strncpy(buffer, result_message.c_str(), result_message_size);
        return result_message_size;
    }

    COMMENT("Explicit marking if message is cut off.") {
        using namespace libtbag::tty;
        using namespace libtbag::string;
        std::string suffix;
        if (EMERGENCY_LEVEL <= COMPARE_AND(level) <= DEBUG_LEVEL) {
            suffix = fformat("...{}{}", DISPLAY_ATTRIBUTE_RESET, LINE_FEED_STR);
        } else {
            suffix = fformat("...{}", LINE_FEED_STR);
        }
        if (suffix.size() > buffer_size) {
            // The buffer size is very small.
            auto const write_size = libtbag::algorithm::getMin(msg_size, buffer_size);
            ::strncpy(buffer, msg, write_size);
            return write_size;
        }
        ::strncpy(buffer, result_message.c_str(), buffer_size-suffix.size());
        ::strncpy(buffer+buffer_size-suffix.size(), suffix.c_str(), suffix.size());
    }
    assert(result_message_size > buffer_size);
    return buffer_size;
}

} // namespace msg
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

