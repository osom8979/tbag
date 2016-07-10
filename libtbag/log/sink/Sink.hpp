/**
 * @file   Sink.hpp
 * @brief  Sink class prototype.
 * @author zer0
 * @date   2016-07-08
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_SINK_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_SINK_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/log/details/MsgPacket.hpp>
#include <libtbag/lock/FakeLock.hpp>

#include <mutex>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log  {
namespace sink {

/**
 * Base Sink interface.
 *
 * @author zer0
 * @date   2016-07-08
 */
template <typename MutexType = lock::FakeLock, typename CharType = char>
class BaseSink
{
public:
    using Value   = CharType;
    using Mutex   = MutexType;
    using Message = details::BaseMsgPacket<Value>;

private:
    Mutex _mutex;
    bool  _force_flush;

public:
    BaseSink() : _mutex(), _force_flush(false) {}
    BaseSink(bool force_flush) : _mutex(), _force_flush(force_flush) {}
    virtual ~BaseSink() {}

protected:
    virtual void writeReal(Message const & msg) = 0;
    virtual void flushReal() = 0;

public:
    void write(Message const & msg)
    {
        std::lock_guard<Mutex> guard(_mutex);
        writeReal(msg);
        if (_force_flush) {
            flushReal();
        }
    }

    void flush()
    {
        std::lock_guard<Mutex> guard(_mutex);
        flushReal();
    }

public:
    template <typename ... Args>
    constexpr inline static Message makeMessage(Args && ... args) noexcept
    {
        return Message(std::forward<Args>(args) ...);
    }
};

template <typename Mutex>
using Sink = BaseSink<Mutex, char>;

template <typename Mutex>
using WideSink = BaseSink<Mutex, wchar_t>;

} // namespace sink
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_SINK_HPP__

