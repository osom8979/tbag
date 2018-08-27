/**
 * @file   CoutSink.hpp
 * @brief  CoutSink class prototype.
 * @author zer0
 * @date   2016-07-21
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_COUTSINK_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_COUTSINK_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/log/sink/Sink.hpp>

#include <cstdio>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log  {
namespace sink {

/**
 * CoutSink class prototype.
 *
 * @author zer0
 * @date   2016-07-17
 */
template <typename MutexType = lock::FakeLock>
class CoutSink : public Sink<MutexType>
{
public:
    using Parent = Sink<MutexType>;
    using String = typename Parent::String;
    using Mutex  = typename Parent::Mutex;

public:
    TBAG_CONSTEXPR static char const * const TYPE_NAME = "COUT_SINK";

public:
    CoutSink(bool force_flush = false) : Parent(force_flush)
    { /* EMPTY. */ }
    virtual ~CoutSink()
    { /* EMPTY. */ }

public:
    virtual char const * const name() const override
    {
        return TYPE_NAME;
    }

    virtual void write(String const & message) override
    {
        ::fwrite(message.c_str(), message.size(), 1, stdout);
    }

    virtual void flush() override
    {
        ::fflush(stdout);
    }
};

} // namespace sink
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_COUTSINK_HPP__

