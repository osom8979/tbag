/**
 * @file   NullSink.hpp
 * @brief  NullSink class prototype.
 * @author zer0
 * @date   2016-07-17
 * @date   2019-07-02 (Create a cpp file)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_NULLSINK_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_NULLSINK_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/log/sink/Sink.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log  {
namespace sink {

/**
 * NullSink class prototype.
 *
 * @author zer0
 * @date   2016-07-17
 * @date   2019-07-02 (Create a cpp file)
 */
class TBAG_API NullSink : public Sink
{
public:
    NullSink();
    virtual ~NullSink();

public:
    virtual bool write(char const * message, int size) override;
    virtual void flush() override;
};

} // namespace sink
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_NULLSINK_HPP__

