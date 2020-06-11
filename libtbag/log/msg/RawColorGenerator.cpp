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

void RawColorGenerator::make(char const * UNUSED_PARAM(logger),
                             int level,
                             char const * UNUSED_PARAM(level_name),
                             char const * msg, int msg_size,
                             std::string & result) const
{
    assert(msg != nullptr);
    assert(msg_size >= 1);

    std::stringstream ss;
    ss << libtbag::log::details::getColorPrefix(level);
    ss.write(msg, msg_size);
    ss << libtbag::log::details::getColorSuffix(level) << LINE_FEED_TEXT;

    result.assign(ss.str());
}

} // namespace msg
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

