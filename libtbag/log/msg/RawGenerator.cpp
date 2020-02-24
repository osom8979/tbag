/**
 * @file   RawGenerator.cpp
 * @brief  RawGenerator class implementation.
 * @author zer0
 * @date   2019-07-02
 */

#include <libtbag/log/msg/RawGenerator.hpp>
#include <libtbag/algorithm/MinMax.hpp>
#include <cstring>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log {
namespace msg {

RawGenerator::RawGenerator(LineFeedStyle line_feed)
        : Generator(line_feed)
{
    // EMPTY.
}

RawGenerator::RawGenerator(std::string const & line_feed)
        : Generator(line_feed)
{
    // EMPTY.
}

RawGenerator::~RawGenerator()
{
    // EMPTY.
}

int RawGenerator::getPaddingByte() const
{
    return getLineFeedLength();
}

int RawGenerator::make(char * buffer, int buffer_size,
                       char const * UNUSED_PARAM(logger),
                       int UNUSED_PARAM(level), char const * UNUSED_PARAM(level_name),
                       char const * msg, int msg_size) const
{
    auto const COPY_SIZE = libtbag::algorithm::getMin(buffer_size, msg_size);
    ::strncpy(buffer, msg, COPY_SIZE);
    if (libtbag::string::appendLineFeed(LINE_FEED, buffer, buffer_size, COPY_SIZE)) {
        if (LINE_FEED == LineFeedStyle::LFS_AUTO) {
            return COPY_SIZE + (isWindowsPlatform() ? 2 : 1);
        } else if (LINE_FEED == LineFeedStyle::LFS_UNIX) {
            return COPY_SIZE + 1;
        } else if (LINE_FEED == LineFeedStyle::LFS_WINDOWS) {
            return COPY_SIZE + 2;
        }
    }
    return COPY_SIZE;
}

} // namespace msg
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

