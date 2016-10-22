/**
 * @file   CoutSink.hpp
 * @brief  CoutSink class prototype.
 * @author zer0
 * @date   2016-07-17
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_COUTSINK_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_COUTSINK_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/log/sink/StreamSink.hpp>
#include <libtbag/Noncopyable.hpp>
#include <iostream>

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
template <typename Mutex = lock::FakeLock>
class CoutSink : public StreamSink<std::basic_ostream<char>, Mutex, char>
{
public:
    using Stream  = std::basic_ostream<char>;
    using Parent  = StreamSink<Stream, Mutex, char>;
    using String  = typename Parent::String;
    using Message = typename Parent::Message;

public:
    CoutSink(bool force_flush = false) : Parent(std::cout, force_flush)
    {
        // EMPTY.
    }

    virtual ~CoutSink() = default;
};

} // namespace sink
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_COUTSINK_HPP__

