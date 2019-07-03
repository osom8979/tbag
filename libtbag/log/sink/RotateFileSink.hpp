/**
 * @file   RotateFileSink.hpp
 * @brief  RotateFileSink class prototype.
 * @author zer0
 * @date   2017-04-13
 * @date   2019-07-02 (Create a cpp file)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_ROTATEFILESINK_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_ROTATEFILESINK_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/log/sink/Sink.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log  {
namespace sink {

/**
 * RotateFileSink class prototype.
 *
 * @author zer0
 * @date   2017-04-13
 * @date   2019-07-02 (Create a cpp file)
 */
class TBAG_API RotateFileSink : public Sink
{
public:
    RotateFileSink(std::string const & path);
    virtual ~RotateFileSink();

public:
    virtual bool write(char const * message, int size) override;
    virtual void flush() override;
};

} // namespace sink
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_ROTATEFILESINK_HPP__

