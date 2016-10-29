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
#include <libtbag/predef.hpp>
#include <libtbag/log/details/MsgPacket.hpp>
#include <libtbag/lock/FakeLock.hpp>

#include <string>

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
class SinkInterface
{
public:
    using MsgPacket = details::MsgPacket;

public:
    TBAG_CONSTEXPR SinkInterface()
    { /* EMPTY. */ }
    virtual ~SinkInterface()
    { /* EMPTY. */ }

public:
    virtual void writeReal(std::string const & msg) = 0;
    virtual void flushReal() = 0;

public:
    virtual void write(MsgPacket const & msg) = 0;
    virtual void flush() = 0;

public:
    template <typename ... Args>
    /* TBAG_CONSTEXPR */ inline static MsgPacket makeMessage(Args && ... args) TBAG_NOEXCEPT
    {
        return MsgPacket(std::forward<Args>(args) ...);
    }
};

/**
 * Base Sink interface.
 *
 * @author zer0
 * @date   2016-07-08
 */
template <typename MutexType = lock::FakeLock>
class Sink : public SinkInterface
{
public:
    using Mutex = MutexType;

private:
    Mutex _mutex;
    bool  _force_flush;

public:
    Sink() : _mutex(), _force_flush(false)
    { /* EMPTY. */ }
    Sink(bool force_flush) : _mutex(), _force_flush(force_flush)
    { /* EMPTY. */ }
    virtual ~Sink()
    { /* EMPTY. */ }

public:
    virtual void write(MsgPacket const & msg) override
    {
        _mutex.lock();
        writeReal(msg.getString() + "\n");
        if (_force_flush) {
            flushReal();
        }
        _mutex.unlock();
    }

    virtual void flush() override
    {
        _mutex.lock();
        flushReal();
        _mutex.unlock();
    }
};

} // namespace sink
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_SINK_HPP__

