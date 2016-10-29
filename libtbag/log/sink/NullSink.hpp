/**
 * @file   NullSink.hpp
 * @brief  NullSink class prototype.
 * @author zer0
 * @date   2016-07-17
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_NULLSINK_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_NULLSINK_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
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
 */
class NullSink : public Sink<lock::FakeLock>, public Noncopyable
{
public:
    using Parent = Sink<lock::FakeLock>;

public:
    NullSink()
    { /* EMPTY. */ }
    virtual ~NullSink()
    { /* EMPTY. */ }

public:
    virtual void writeReal(std::string const & msg) override
    { /* EMPTY. */ }
    virtual void flushReal() override
    { /* EMPTY. */ }
};

} // namespace sink
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_NULLSINK_HPP__

