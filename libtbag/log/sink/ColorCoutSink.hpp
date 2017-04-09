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
#include <libtbag/tty/Tces.hpp>

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
class ColorCoutSink : public CoutSink<MutexType>
{
public:
    using Parent = CoutSink<MutexType>;

public:
    ColorCoutSink(bool force_flush = false) : Parent(force_flush)
    { /* EMPTY. */ }

    virtual ~ColorCoutSink()
    { /* EMPTY. */ }

public:
    virtual void write(details::MsgPacket const & msg) override
    {
        using namespace ::libtbag::log::details;

        std::string message;
        Severity level = msg.getSeverity();

        if (level <= EMERGENCY_SEVERITY) {
            message = tces::DISPLAY_ATTRIBUTE_BG_RED;
            message += tces::DISPLAY_ATTRIBUTE_FG_CYAN;
        } else if (level <= ALERT_SEVERITY) {
            message = tces::DISPLAY_ATTRIBUTE_FG_RED;
        } else if (level <= CRITICAL_SEVERITY) {
            message = tces::DISPLAY_ATTRIBUTE_FG_RED;
        } else if (level <= ERROR_SEVERITY) {
            message = tces::DISPLAY_ATTRIBUTE_FG_YELLOW;
        } else if (level <= WARNING_SEVERITY) {
            message = tces::DISPLAY_ATTRIBUTE_FG_YELLOW;
        } else if (level <= NOTICE_SEVERITY) {
            message = tces::DISPLAY_ATTRIBUTE_FG_GREEN;
        } else if (level <= INFORMATIONAL_SEVERITY) {
            message = tces::DISPLAY_ATTRIBUTE_FG_GREEN;
        } else if (level <= DEBUG_SEVERITY) {
            message = tces::DISPLAY_ATTRIBUTE_FG_BLUE;
        }
        message += msg.getString() + tces::DISPLAY_ATTRIBUTE_RESET;

        Parent::write(details::MsgPacket(msg.getSeverity(), message));
    }
};

} // namespace sink
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_COLORCOUTSINK_HPP__

