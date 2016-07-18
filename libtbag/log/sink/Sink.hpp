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
#include <libtbag/Strings.hpp>

#include <string>
#include <mutex>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log  {
namespace sink {

/**
 * Sink interface.
 *
 * @author zer0
 * @date   2016-07-16
 */
template <typename CharType = char>
class BaseSinkInterface
{
public:
    using Message = details::BaseMsgPacket<CharType>;
    using String  = typename Message::String;

public:
    constexpr BaseSinkInterface() = default;
    virtual ~BaseSinkInterface() = default;

public:
    virtual void writeReal(String const & msg) = 0;
    virtual void flushReal() = 0;

public:
    virtual void write(Message const & msg) = 0;
    virtual void flush() = 0;

public:
    template <typename ... Args>
    constexpr inline static Message makeMessage(Args && ... args) noexcept
    {
        return Message(std::forward<Args>(args) ...);
    }
};

using SinkInterface     = BaseSinkInterface<char>;
using WideSinkInterface = BaseSinkInterface<wchar_t>;

/**
 * Base Sink interface.
 *
 * @author zer0
 * @date   2016-07-08
 */
template <typename MutexType = lock::FakeLock, typename CharType = char>
class BaseSink : public BaseSinkInterface<CharType>
{
public:
    using Value   = CharType;
    using Mutex   = MutexType;
    using Parent  = BaseSinkInterface<CharType>;
    using Message = typename Parent::Message;

private:
    Mutex _mutex;
    bool  _force_flush;

public:
    BaseSink() : _mutex(), _force_flush(false) {}
    BaseSink(bool force_flush) : _mutex(), _force_flush(force_flush) {}
    virtual ~BaseSink() {}

public:
    virtual void write(Message const & msg) override
    {
        std::lock_guard<Mutex> guard(_mutex);

        this->writeReal(msg.getString() + CHAR_OR_WIDECHAR(Value, "\n"));
        if (_force_flush) {
            this->flushReal();
        }
    }

    virtual void flush() override
    {
        std::lock_guard<Mutex> guard(_mutex);
        this->flushReal();
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

