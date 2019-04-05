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
#include <libtbag/log/msg/LogMsgPacket.hpp>

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
class TBAG_API PacketGenerator : private Noncopyable
{
public:
    enum class MakeType
    {
        DEFAULT = 0,
        DEFAULT_COLOR,
        RAW,
    };

public:
    TBAG_CONSTEXPR static char const * const DATE_MSG_SEVERITY_PREFIX = "[";
    TBAG_CONSTEXPR static char const * const DATE_MSG_SEVERITY_SUFFIX = "]";
    TBAG_CONSTEXPR static char const * const DATE_MSG_THREAD_PREFIX   = "@";
    TBAG_CONSTEXPR static char const * const DATE_MSG_SEPARATOR       = " ";

public:
    using Severity = level::Severity;
    using String   = std::string;

private:
    MakeType _type;
    String   _endl;

public:
    PacketGenerator();
    PacketGenerator(MakeType type);
    ~PacketGenerator();

public:
    // clang-format off
    inline void setType(MakeType type) TBAG_NOEXCEPT { _type = type; }
    inline MakeType getType() const TBAG_NOEXCEPT { return _type; }
    inline String getNewLine() const { return _endl; }
    // clang-format on

public:
    String make(LogMsgPacket const & packet);

public:
    String makeDefault(LogMsgPacket const & packet);
    String makeDefaultColor(LogMsgPacket const & packet);
    String makeRaw(LogMsgPacket const & packet);

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

