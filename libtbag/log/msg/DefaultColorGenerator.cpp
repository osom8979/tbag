/**
 * @file   DefaultColorGenerator.cpp
 * @brief  DefaultColorGenerator class implementation.
 * @author zer0
 * @date   2019-07-02
 */

#include <libtbag/log/msg/DefaultColorGenerator.hpp>
#include <libtbag/log/msg/DefaultMsg.hpp>
#include <libtbag/log/Severity.hpp>
#include <libtbag/log/details/ColorGeneratorHelper.hpp>
#include <libtbag/algorithm/MinMax.hpp>
#include <libtbag/string/Format.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cassert>
#include <cstring>

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

void DefaultColorGenerator::make(char const * UNUSED_PARAM(logger),
                                 int level,
                                 char const * UNUSED_PARAM(level_name),
                                 char const * msg, int msg_size,
                                 std::string & result) const
{
    assert(msg != nullptr);
    assert(msg_size >= 1);

    std::stringstream ss;
    ss << libtbag::log::details::getColorPrefix(level);
    makeDefaultPrefix(ss, level);
    ss.write(msg, msg_size);
    ss << libtbag::log::details::getColorSuffix(level) << LINE_FEED_TEXT;

    result.assign(ss.str());
}

} // namespace msg
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

