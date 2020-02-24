/**
 * @file   DefaultColorGenerator.hpp
 * @brief  DefaultColorGenerator class prototype.
 * @author zer0
 * @date   2019-07-02
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_MSG_DEFAULTCOLORGENERATOR_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_MSG_DEFAULTCOLORGENERATOR_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/log/msg/Generator.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log {
namespace msg {

/**
 * DefaultColorGenerator class prototype.
 *
 * @author zer0
 * @date   2019-07-02
 */
class TBAG_API DefaultColorGenerator : public Generator
{
private:
    struct _Cache {
        std::string fixed_suffix;
    } _cache;

public:
    DefaultColorGenerator(LineFeedStyle line_feed = LineFeedStyle::LFS_UNIX);
    DefaultColorGenerator(std::string const & line_feed);
    virtual ~DefaultColorGenerator();

public:
    int getPaddingByte() const override;
    int make(char * buffer, int buffer_size,
             char const * logger,
             int level, char const * level_name,
             char const * msg, int msg_size) const override;
};

} // namespace msg
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_MSG_DEFAULTCOLORGENERATOR_HPP__

