/**
 * @file   DefaultGenerator.cpp
 * @brief  DefaultGenerator class implementation.
 * @author zer0
 * @date   2019-07-02
 */

#include <libtbag/log/msg/DefaultGenerator.hpp>
#include <libtbag/log/Severity.hpp>
#include <libtbag/algorithm/MinMax.hpp>
#include <libtbag/time/TimePoint.hpp>

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
    assert(buffer != nullptr);
    assert(buffer_size >= 1);
    assert(msg != nullptr);
    assert(msg_size >= 1);

    std::stringstream ss;
    ss << getShortPrefix(level)
       << ' ' << libtbag::time::TimePoint::now().toLocalLongString()
       << " @" << std::this_thread::get_id()
       << ' ' << std::string(msg, msg + msg_size)
       << LINE_FEED_STR;

    auto const result_message = ss.str();
    auto const result_message_size = result_message.size();
    if (result_message_size <= buffer_size) {
        ::strncpy(buffer, result_message.c_str(), result_message_size);
        return result_message_size;
    }

    COMMENT("Explicit marking if message is cut off.") {
        auto const suffix = libtbag::string::fformat("...{}", LINE_FEED_STR);
        if (suffix.size() > buffer_size) {
            // The buffer size is very small.
            auto const write_size = libtbag::algorithm::getMin<int>(msg_size, buffer_size);
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

