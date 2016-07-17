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
#include <libtbag/log/sink/Sink.hpp>
#include <libtbag/Noncopyable.hpp>

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
template <typename Mutex = lock::FakeLock, typename CharType = char>
class NullSink : public BaseSink<Mutex, CharType>, public Noncopyable
{
public:
    using Parent  = BaseSink<Mutex, CharType>;
    using Stream  = OutputStream;
    using Message = typename Parent::Message;

public:
    NullSink() = default;
    virtual ~StreamSink() = default;

public:
    virtual void writeReal(Message const & msg) override
    {
        // EMPTY.
    }

    virtual void flushReal() override
    {
        // EMPTY.
    }
};

} // namespace sink
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_NULLSINK_HPP__

