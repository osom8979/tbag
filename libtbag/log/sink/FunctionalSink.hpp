/**
 * @file   FunctionalSink.hpp
 * @brief  FunctionalSink class prototype.
 * @author zer0
 * @date   2019-07-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_FUNCTIONALSINK_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_FUNCTIONALSINK_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/log/sink/Sink.hpp>

#include <functional>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log  {
namespace sink {

/**
 * FunctionalSink class prototype.
 *
 * @author zer0
 * @date   2019-07-03
 */
class TBAG_API FunctionalSink : public Sink
{
public:
    using UserData = void *;
    using WriteCallback = std::function<bool(char const *, int, UserData)>;
    using FlushCallback = std::function<void(UserData)>;

public:
    WriteCallback const WRITE_CALLBACK;
    FlushCallback const FLUSH_CALLBACK;

public:
    UserData const USER_DATA;

public:
    FunctionalSink(WriteCallback const & wcb, FlushCallback const & fcb = nullptr, void * user = nullptr);
    virtual ~FunctionalSink();

public:
    virtual bool write(char const * message, int size) override;
    virtual void flush() override;
};

} // namespace sink
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_FUNCTIONALSINK_HPP__

