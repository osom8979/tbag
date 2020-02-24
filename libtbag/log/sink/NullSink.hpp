/**
 * @file   NullSink.hpp
 * @brief  NullSink class prototype.
 * @author zer0
 * @date   2016-07-17
 * @date   2019-07-02 (Create a cpp file)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_NULLSINK_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_NULLSINK_HPP__

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
 * NullSink class prototype.
 *
 * @author zer0
 * @date   2016-07-17
 * @date   2019-07-02 (Create a cpp file)
 */
class TBAG_API NullSink : public Sink
{
public:
    TBAG_CONSTEXPR static char const * sink_name() TBAG_NOEXCEPT
    { return "null"; }

public:
    NullSink();
    NullSink(std::string const & UNUSED_PARAM(arguments));
    virtual ~NullSink();

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

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_NULLSINK_HPP__

