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
#include <libtbag/log/sink/Sink.hpp>
#include <libtbag/Noncopyable.hpp>

#include <iostream>
#include <type_traits>
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
template <typename OutputStream, typename Mutex = lock::FakeLock, typename CharType = char>
class StreamSink : public BaseSink<Mutex, CharType>, public Noncopyable
{
public:
    using Parent  = BaseSink<Mutex, CharType>;
    using Stream  = OutputStream;
    using Message = typename Parent::Message;

    static_assert(std::is_same<typename OutputStream::char_type, CharType>::value
            , "OutputStream::char_type and CharType are not same type");

public:
    Stream & _stream;

public:
    StreamSink(Stream & stream, bool force_flush = false)
            : Parent(force_flush), _stream(stream)
    {
        // EMPTY.
    }

    virtual ~StreamSink() = default;

public:
    virtual void writeReal(std::basic_string<CharType> const & msg) override
    {
        _stream.write(msg.c_str(), msg.size());
    }

    virtual void flushReal() override
    {
        _stream.flush();
    }
};

} // namespace sink
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_STREAMSINK_HPP__

