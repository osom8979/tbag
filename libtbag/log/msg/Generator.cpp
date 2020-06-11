/**
 * @file   Generator.cpp
 * @brief  Generator class implementation.
 * @author zer0
 * @date   2019-07-03
 */

#include <libtbag/log/msg/Generator.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log {
namespace msg {

Generator::Generator(LineFeedStyle line_feed)
        : LINE_FEED(line_feed), LINE_FEED_TEXT(libtbag::string::getLineFeed(line_feed))
{
    // EMPTY.
}

Generator::Generator(std::string const & line_feed)
        : Generator(libtbag::string::getLineFeedStyle(line_feed))
{
    // EMPTY.
}

Generator::~Generator()
{
    // EMPTY.
}

} // namespace msg
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

