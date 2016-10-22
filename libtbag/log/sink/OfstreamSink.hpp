/**
 * @file   OfstreamSink.hpp
 * @brief  OfstreamSink class prototype.
 * @author zer0
 * @date   2016-07-18
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_OFSTREAMSINK_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_OFSTREAMSINK_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/log/sink/Sink.hpp>
#include <libtbag/Noncopyable.hpp>

#include <fstream>
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
template <typename Mutex = lock::FakeLock, typename CharType = char>
class OfstreamSink : public BaseSink<Mutex, CharType>, public Noncopyable
{
public:
    using Parent  = BaseSink<Mutex, CharType>;
    using Stream  = std::basic_ofstream<CharType>;
    using Message = typename Parent::Message;
    using String  = typename Parent::String;

    static_assert(std::is_same<typename Stream::char_type, CharType>::value
            , "Stream::char_type and CharType are not same type");

public:
    Stream _stream;

public:
    OfstreamSink(String const & path, bool force_flush = false)
            : Parent(force_flush), _stream(path /*, std::ios_base::binary | std::ios_base::out */)
    {
        // EMPTY.
    }

    virtual ~OfstreamSink()
    {
        if (_stream.is_open() == true) {
            _stream.close();
        }
    }

public:
    virtual void writeReal(String const & msg) override
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

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_OFSTREAMSINK_HPP__

