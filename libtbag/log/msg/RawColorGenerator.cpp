/**
 * @file   RawColorGenerator.cpp
 * @brief  RawColorGenerator class implementation.
 * @author zer0
 * @date   2020-05-09
 */

#include <libtbag/log/msg/RawColorGenerator.hpp>
#include <libtbag/log/Severity.hpp>
#include <libtbag/log/details/ColorGeneratorHelper.hpp>
#include <libtbag/algorithm/MinMax.hpp>
#include <libtbag/string/Format.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log {
namespace msg {

RawColorGenerator::RawColorGenerator(LineFeedStyle line_feed)
        : Generator(line_feed)
{
    // EMPTY.
}

RawColorGenerator::RawColorGenerator(std::string const & line_feed)
        : Generator(line_feed)
{
    // EMPTY.
}

RawColorGenerator::~RawColorGenerator()
{
    // EMPTY.
}

int RawColorGenerator::getPaddingByte() const
{
    // clang-format on
    using namespace libtbag::string;
    TBAG_CONSTEXPR static auto const padding_byte =
        string_length(_EMERGENCY_ATTRIBUTE_BG) + // NOLINT
        string_length(TBAG_TTY_DISPLAY_ATTRIBUTE_RESET) +
        string_length(TBAG_WINDOWS_NEW_LINE);
    return padding_byte;
    // clang-format off
}

int RawColorGenerator::make(char * buffer, int buffer_size,
                            char const * UNUSED_PARAM(logger),
                            int level, char const * UNUSED_PARAM(level_name),
                            char const * msg, int msg_size) const
{
    assert(buffer != nullptr);
    assert(buffer_size >= 1);
    assert(msg != nullptr);
    assert(msg_size >= 1);

    std::stringstream ss;
    ss << libtbag::log::details::getColorPrefix(level);
    ss.write(msg, msg_size);
    ss << libtbag::log::details::getColorSuffix(level) << LINE_FEED_STR;

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

