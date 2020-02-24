/**
 * @file   Sink.hpp
 * @brief  Sink class prototype.
 * @author zer0
 * @date   2019-07-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_SINK_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_SINK_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log  {
namespace sink {

/**
 * Sink interface.
 *
 * @author zer0
 * @date   2016-07-08
 * @date   2019-07-02 (Refactoring)
 */
struct SinkInterface
{
    SinkInterface() TBAG_NOEXCEPT { /* EMPTY. */ }
    virtual ~SinkInterface() { /* EMPTY. */ }

    virtual bool write(int level, char const * message, int size) = 0;
    virtual bool isRequiredFlush() = 0;
    virtual void flush() = 0;
};

/**
 * Sink class prototype.
 *
 * @author zer0
 * @date   2019-07-03
 */
class TBAG_API Sink : public SinkInterface
{
public:
    Sink() TBAG_NOEXCEPT;
    virtual ~Sink();
};

} // namespace sink
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_SINK_HPP__

