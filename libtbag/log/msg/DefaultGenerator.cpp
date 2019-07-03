/**
 * @file   DefaultGenerator.cpp
 * @brief  DefaultGenerator class implementation.
 * @author zer0
 * @date   2019-07-02
 */

#include <libtbag/log/msg/DefaultGenerator.hpp>
#include <libtbag/algorithm/MinMax.hpp>
#include <libtbag/time/TimePoint.hpp>

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
                           int UNUSED_PARAM(level), char const * level_name,
                           char const * msg, int msg_size) const
{
    std::stringstream ss;
    ss << libtbag::time::TimePoint::now().toLocalLongString()
       << " @" << std::this_thread::get_id()
       << " [" << level_name
       << "] " << std::string(msg, msg + msg_size);

    if (LINE_FEED == LineFeedStyle::LFS_AUTO) {
        ss << (isWindowsPlatform() ? libtbag::string::WINDOWS_NEW_LINE : libtbag::string::UNIX_NEW_LINE);
    } else if (LINE_FEED == LineFeedStyle::LFS_UNIX) {
        ss << libtbag::string::UNIX_NEW_LINE;
    } else if (LINE_FEED == LineFeedStyle::LFS_WINDOWS) {
        ss << libtbag::string::WINDOWS_NEW_LINE;
    }

    auto const RESULT = ss.str();
    auto const COPY_SIZE = libtbag::algorithm::getMin<int>(buffer_size, RESULT.size());
    ::strncpy(buffer, RESULT.c_str(), COPY_SIZE);
    return COPY_SIZE;
}

} // namespace msg
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

