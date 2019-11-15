/**
 * @file   DefaultGenerator.cpp
 * @brief  DefaultGenerator class implementation.
 * @author zer0
 * @date   2019-07-02
 */

#include <libtbag/log/msg/DefaultGenerator.hpp>
#include <libtbag/time/TimePoint.hpp>
#include <libtbag/log/Severity.hpp>

#include <cstring>
#include <thread>
#include <sstream>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log {
namespace msg {

DefaultGenerator::DefaultGenerator(LineFeedStyle line_feed)
        : Generator(line_feed)
{
    // EMPTY.
}

DefaultGenerator::DefaultGenerator(std::string const & line_feed)
        : Generator(line_feed)
{
    // EMPTY.
}

DefaultGenerator::~DefaultGenerator()
{
    // EMPTY.
}

int DefaultGenerator::make(char * buffer, int buffer_size,
                           char const * UNUSED_PARAM(logger),
                           int level, char const * UNUSED_PARAM(level_name),
                           char const * msg, int msg_size) const
{
    std::stringstream ss;
    ss << getShortPrefix(level)
       << ' ' << libtbag::time::TimePoint::now().toLocalLongString()
       << " @" << std::this_thread::get_id()
       << ' ' << std::string(msg, msg + msg_size);

    if (LINE_FEED == LineFeedStyle::LFS_AUTO) {
        ss << (isWindowsPlatform() ? libtbag::string::WINDOWS_NEW_LINE : libtbag::string::UNIX_NEW_LINE);
    } else if (LINE_FEED == LineFeedStyle::LFS_UNIX) {
        ss << libtbag::string::UNIX_NEW_LINE;
    } else if (LINE_FEED == LineFeedStyle::LFS_WINDOWS) {
        ss << libtbag::string::WINDOWS_NEW_LINE;
    }

    auto const result_message = ss.str();
    auto const result_message_size = result_message.size();
    if (result_message_size <= buffer_size) {
        ::strncpy(buffer, result_message.c_str(), result_message_size);
        return result_message_size;
    }

    COMMENT("Explicit marking if message is cut off.") {
        ::strncpy(buffer, result_message.c_str(), buffer_size-4);
        buffer[buffer_size-4] = ' ';
        buffer[buffer_size-3] = '.';
        buffer[buffer_size-2] = '.';
        buffer[buffer_size-1] = '.';
    }
    assert(result_message_size > buffer_size);
    return buffer_size;
}

} // namespace msg
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

