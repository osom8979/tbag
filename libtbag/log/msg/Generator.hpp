/**
 * @file   Generator.hpp
 * @brief  Generator class prototype.
 * @author zer0
 * @date   2019-07-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_MSG_GENERATOR_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_MSG_GENERATOR_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log {
namespace msg {

/**
 * GeneratorInterface class prototype.
 *
 * @author zer0
 * @date   2019-07-02
 */
struct GeneratorInterface
{
    GeneratorInterface() TBAG_NOEXCEPT { /* EMPTY. */ }
    virtual ~GeneratorInterface() { /* EMPTY. */ }

    virtual void make(char const * logger, int level, char const * level_name,
                      char const * msg, int msg_size, std::string & buffer) const = 0;
};

/**
 * Generator class prototype.
 *
 * @author zer0
 * @date   2019-07-03
 */
class TBAG_API Generator : public GeneratorInterface
{
public:
    using LineFeedStyle = libtbag::string::LineFeedStyle;

public:
    LineFeedStyle const LINE_FEED;
    std::string const LINE_FEED_TEXT;

public:
    Generator(LineFeedStyle line_feed = LineFeedStyle::LFS_UNIX);
    Generator(std::string const & line_feed);
    virtual ~Generator();
};

} // namespace msg
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_MSG_GENERATOR_HPP__

