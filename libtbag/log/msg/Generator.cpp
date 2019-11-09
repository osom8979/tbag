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

Generator::Generator(LineFeedStyle line_feed) TBAG_NOEXCEPT
        : LINE_FEED(line_feed)
{
    // EMPTY.
}

Generator::Generator(std::string const & line_feed)
        : LINE_FEED(libtbag::string::getLineFeedStyle(line_feed))
{
    // EMPTY.
}

Generator::~Generator()
{
    // EMPTY.
}

std::string Generator::make_string(char const * logger, int level, char const * level_name,
                                   char const * msg, int msg_size) const
{
    char buffer[Generator::STACK_BUFFER_SIZE];
    int const copy_size = make(buffer, Generator::STACK_BUFFER_SIZE, logger, level, level_name, msg, msg_size);
    if (copy_size >= 1) {
        return std::string(buffer, buffer + copy_size);
    } else {
        return std::string(msg, msg + msg_size);
    }
}

} // namespace msg
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

