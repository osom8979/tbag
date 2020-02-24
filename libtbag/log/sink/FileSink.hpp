/**
 * @file   FileSink.hpp
 * @brief  FileSink class prototype.
 * @author zer0
 * @date   2017-04-13
 * @date   2019-07-02 (Create a cpp file)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_FILESINK_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_FILESINK_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/log/sink/Sink.hpp>

#include <cstdio>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log  {
namespace sink {

/**
 * FileSink class prototype.
 *
 * @author zer0
 * @date   2017-04-13
 * @date   2019-07-02 (Create a cpp file)
 */
class TBAG_API FileSink : public Sink
{
public:
    TBAG_CONSTEXPR static char const * sink_name() TBAG_NOEXCEPT
    { return "file"; }

private:
    FILE * _file;

public:
    explicit FileSink(char const * path);
    explicit FileSink(std::string const & arguments);
    virtual ~FileSink();

public:
    bool write(int level, char const * message, int size) override;
    bool isRequiredFlush() override;
    void flush() override;
};

} // namespace sink
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_FILESINK_HPP__

