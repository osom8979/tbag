/**
 * @file   DefaultGenerator.cpp
 * @brief  DefaultGenerator class implementation.
 * @author zer0
 * @date   2019-07-02
 */

#include <libtbag/log/msg/DefaultGenerator.hpp>
#include <libtbag/log/msg/DefaultMsg.hpp>
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

void DefaultGenerator::make(char const * UNUSED_PARAM(logger),
                            int level,
                            char const * UNUSED_PARAM(level_name),
                            char const * msg, int msg_size,
                            std::string & result) const
{
    assert(msg != nullptr);
    assert(msg_size >= 1);

    std::stringstream ss;
    makeDefaultPrefix(ss, level);
    ss.write(msg, msg_size);
    ss << LINE_FEED_TEXT;

    result.assign(ss.str());
}

} // namespace msg
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

