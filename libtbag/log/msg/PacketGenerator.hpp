/**
 * @file   PacketGenerator.hpp
 * @brief  PacketGenerator class prototype.
 * @author zer0
 * @date   2017-04-13
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_MSG_PACKETGENERATOR_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_MSG_PACKETGENERATOR_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/log/level/Severity.hpp>
#include <libtbag/log/msg/MsgPacket.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log {
namespace msg {

/**
 * PacketGenerator class prototype.
 *
 * @author zer0
 * @date   2017-04-13
 */
class TBAG_API PacketGenerator : public Noncopyable
{
public:
    enum class MakeType
    {
        DEFAULT = 0,
        DEFAULT_COLOR,
    };

public:
    using Severity = level::Severity;
    using String   = std::string;

private:
    MakeType _type;

public:
    PacketGenerator();
    PacketGenerator(MakeType type);
    ~PacketGenerator();

public:
    // @formatter:off
    inline void setType(MakeType type) TBAG_NOEXCEPT
    { _type = type; }
    inline MakeType getType() const TBAG_NOEXCEPT
    { return _type; }
    // @formatter:on

public:
    String make(MsgPacket const & packet);

    static String makeDefault(MsgPacket const & packet);
    static String makeDefaultColor(MsgPacket const & packet);

public:
    static String getNowTimeString();
    static String getCurrentThreadString();
    static String getSeverityString(Severity const & severity);
};

} // namespace msg
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_MSG_PACKETGENERATOR_HPP__

