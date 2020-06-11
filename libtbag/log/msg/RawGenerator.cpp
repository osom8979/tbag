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

void RawGenerator::make(char const * UNUSED_PARAM(logger),
                        int UNUSED_PARAM(level),
                        char const * UNUSED_PARAM(level_name),
                        char const * msg, int msg_size,
                        std::string & result) const
{
    result.assign(msg, msg + msg_size);
    result += LINE_FEED_TEXT;
}

} // namespace msg
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

