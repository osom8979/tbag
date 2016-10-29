/**
 * @file   StreamSink.hpp
 * @brief  StreamSink class prototype.
 * @author zer0
 * @date   2016-07-10
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_STREAMSINK_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_STREAMSINK_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/log/sink/Sink.hpp>

#include <iostream>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log  {
namespace sink {

/**
 * StreamSink class prototype.
 *
 * @author zer0
 * @date   2016-07-10
 */
template <typename OutputStream, typename MutexType = lock::FakeLock>
class StreamSink : public Sink<MutexType>, public Noncopyable
{
public:
    using Parent  = Sink<MutexType>;
    using OStream = OutputStream;

public:
    OStream & _ostream;

public:
    StreamSink(OStream & ostream, bool force_flush = false) : Parent(force_flush), _ostream(ostream)
    { /* EMPTY. */ }
    virtual ~StreamSink()
    { /* EMPTY. */ }

public:
    virtual void writeReal(std::string const & msg) override
    {
        _ostream.write(msg.c_str(), msg.size());
    }

    virtual void flushReal() override
    {
        _ostream.flush();
    }
};

} // namespace sink
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_STREAMSINK_HPP__

