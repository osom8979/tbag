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
#include <libtbag/predef.hpp>
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
        String message;

        switch (msg.getSeverity().level) {
        case static_cast<int>(LogLevel::LEVEL_EMERGENCY):
            message = tces::DISPLAY_ATTRIBUTE_BG_RED;
            message += tces::DISPLAY_ATTRIBUTE_FG_CYAN;
            break;

        case static_cast<int>(LogLevel::LEVEL_ALERT    ): message = tces::DISPLAY_ATTRIBUTE_FG_RED;    break;
        case static_cast<int>(LogLevel::LEVEL_CRITICAL ): message = tces::DISPLAY_ATTRIBUTE_FG_RED;    break;
        case static_cast<int>(LogLevel::LEVEL_ERROR    ): message = tces::DISPLAY_ATTRIBUTE_FG_YELLOW; break;
        case static_cast<int>(LogLevel::LEVEL_WARNING  ): message = tces::DISPLAY_ATTRIBUTE_FG_YELLOW; break;
        case static_cast<int>(LogLevel::LEVEL_NOTICE   ): message = tces::DISPLAY_ATTRIBUTE_FG_GREEN;  break;
        case static_cast<int>(LogLevel::LEVEL_INFO     ): message = tces::DISPLAY_ATTRIBUTE_FG_GREEN;  break;
        case static_cast<int>(LogLevel::LEVEL_DEBUG    ): message = tces::DISPLAY_ATTRIBUTE_FG_BLUE;   break;
        case static_cast<int>(LogLevel::LEVEL_OFF      ): default: break;
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

