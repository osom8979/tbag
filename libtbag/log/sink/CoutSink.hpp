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
template <typename MutexType = lock::FakeLock>
class CoutSink : public StreamSink<std::ostream, MutexType>
{
public:
    using Parent = StreamSink<std::ostream, MutexType>;

public:
    CoutSink(bool force_flush = false) : Parent(std::cout, force_flush)
    { /* EMPTY */ }

    virtual ~CoutSink()
    { /* EMPTY */ }
};

} // namespace sink
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_COUTSINK_HPP__

