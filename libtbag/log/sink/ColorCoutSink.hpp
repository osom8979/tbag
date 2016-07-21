/**
 * @file   ColorCoutSink.hpp
 * @brief  ColorCoutSink class prototype.
 * @author zer0
 * @date   2016-07-21
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_COLORCOUTSINK_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_COLORCOUTSINK_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/log/sink/CoutSink.hpp>
#include <libtbag/tty/tces.hpp>

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
class ColorCoutSink : public CoutSink<Mutex>
{
public:
    using Parent  = CoutSink<Mutex>;
    using String  = typename Parent::String;
    using Message = typename Parent::Message;

public:
    ColorCoutSink(bool force_flush = false) : Parent(force_flush)
    {
        // EMPTY.
    }

    virtual ~ColorCoutSink() = default;

public:
    virtual void write(Message const & msg) override
    {
        using namespace details;

        SeverityFlagType flag = msg.getSeverity().getFlag();
        String message;
        if (SEVERITY_FLAG_CHECK(flag, DEFAULT_SEVERITY[LOG_SEVERITY_EMERGENCY].flag)) {
            message  = tces::DISPLAY_ATTRIBUTE_BG_RED;
            message += tces::DISPLAY_ATTRIBUTE_FG_CYAN;
        } else if (SEVERITY_FLAG_CHECK(flag, DEFAULT_SEVERITY[LOG_SEVERITY_ALERT].flag)) {
            message = tces::DISPLAY_ATTRIBUTE_FG_RED;
        } else if (SEVERITY_FLAG_CHECK(flag, DEFAULT_SEVERITY[LOG_SEVERITY_CRITICAL].flag)) {
            message = tces::DISPLAY_ATTRIBUTE_FG_RED;
        } else if (SEVERITY_FLAG_CHECK(flag, DEFAULT_SEVERITY[LOG_SEVERITY_ERROR].flag)) {
            message = tces::DISPLAY_ATTRIBUTE_FG_YELLOW;
        } else if (SEVERITY_FLAG_CHECK(flag, DEFAULT_SEVERITY[LOG_SEVERITY_WARNING].flag)) {
            message = tces::DISPLAY_ATTRIBUTE_FG_YELLOW;
        } else if (SEVERITY_FLAG_CHECK(flag, DEFAULT_SEVERITY[LOG_SEVERITY_NOTICE].flag)) {
            message = tces::DISPLAY_ATTRIBUTE_FG_GREEN;
        } else if (SEVERITY_FLAG_CHECK(flag, DEFAULT_SEVERITY[LOG_SEVERITY_INFORMATIONAL].flag)) {
            message = tces::DISPLAY_ATTRIBUTE_FG_GREEN;
        } else if (SEVERITY_FLAG_CHECK(flag, DEFAULT_SEVERITY[LOG_SEVERITY_DEBUG].flag)) {
            message = tces::DISPLAY_ATTRIBUTE_FG_BLUE;
        }
        message += msg.getString() + tces::DISPLAY_ATTRIBUTE_RESET;

        Parent::write(Message(msg.getSeverity(), message));
    }
};

} // namespace sink
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_COLORCOUTSINK_HPP__

