/**
 * @file   CallbackSink.hpp
 * @brief  CallbackSink class prototype.
 * @author zer0
 * @date   2019-07-02
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_CALLBACKSINK_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_CALLBACKSINK_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/log/sink/Sink.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log  {
namespace sink {

/**
 * CallbackSink class prototype.
 *
 * @details
 *  This sink is for C-API. Creation using XML nodes is not possible.
 *
 * @author zer0
 * @date   2019-07-02
 */
class TBAG_API CallbackSink : public Sink
{
public:
    TBAG_CONSTEXPR static char const * sink_name() TBAG_NOEXCEPT
    { return "callback"; }

public:
    using UserData = void*;
    using WriteCallback = bool(*)(int, char const *, int, UserData);
    using FlushCallback = void(*)(UserData);

public:
    WriteCallback const WRITE_CALLBACK;
    FlushCallback const FLUSH_CALLBACK;
    UserData const USER_DATA;

public:
    CallbackSink(WriteCallback const & wcb,
                 FlushCallback const & fcb = nullptr,
                 void * user = nullptr);
    CallbackSink(std::string const & UNUSED_PARAM(arguments));
    virtual ~CallbackSink();

public:
    bool write(int level, char const * message, int size) override;
    bool isRequiredFlush() override;
    void flush() override;
};

} // namespace sink
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_CALLBACKSINK_HPP__

