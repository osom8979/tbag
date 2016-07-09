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
template <typename MutexType, typename CharType = char>
class BaseSink
{
public:
    using Message = details::BaseMsgPacket<CharType>;

public:
    constexpr BaseSink() = default;
    virtual ~BaseSink() = default;

protected:
    virtual void logReal(Message const & msg) = 0;
    virtual void flushReal() = 0;

public:
    void log(Message const & msg)
    {
        std::lock_guard<MutexType> guard;
        logReal(msg);
    }

    void flush()
    {
        std::lock_guard<MutexType> guard;
        flushReal();
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

