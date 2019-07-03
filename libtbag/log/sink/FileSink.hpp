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
private:
    FILE * _file;

public:
    FileSink(char const * path);
    FileSink(std::string const & path);
    virtual ~FileSink();

public:
    virtual bool write(char const * message, int size) override;
    virtual void flush() override;
};

} // namespace sink
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_FILESINK_HPP__

