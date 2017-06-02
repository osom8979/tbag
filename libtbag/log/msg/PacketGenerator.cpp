/**
 * @file   PacketGenerator.cpp
 * @brief  PacketGenerator class implementation.
 * @author zer0
 * @date   2017-04-13
 */

#include <libtbag/log/msg/PacketGenerator.hpp>
#include <libtbag/time/TimePoint.hpp>
#include <libtbag/tty/Tces.hpp>

#include <sstream>
#include <thread>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log {
namespace msg {

char const * const DATE_MSG_SEVERITY_PREFIX = "[";
char const * const DATE_MSG_SEVERITY_SUFFIX = "]";
char const * const DATE_MSG_THREAD_PREFIX   = "@";
char const * const DATE_MSG_SEPARATOR       = " ";

PacketGenerator::PacketGenerator() : _type(MakeType::DEFAULT)
{
    // EMPTY.
}

PacketGenerator::PacketGenerator(MakeType type) : _type(type)
{
    // EMPTY.
}

PacketGenerator::~PacketGenerator()
{
    // EMPTY.
}

PacketGenerator::String PacketGenerator::make(MsgPacket const & packet)
{
    switch (_type) {
    case MakeType::DEFAULT:
        return makeDefault(packet);
    case MakeType::DEFAULT_COLOR:
        return makeDefaultColor(packet);
    default:
        return packet.getMessage();
    }
}

PacketGenerator::String PacketGenerator::makeDefault(MsgPacket const & packet)
{
    return getNowTimeString() + DATE_MSG_SEPARATOR
           + getCurrentThreadString() + DATE_MSG_SEPARATOR
           + getSeverityString(packet.getSeverity()) + DATE_MSG_SEPARATOR
           + packet.getMessage();
}

PacketGenerator::String PacketGenerator::makeDefaultColor(MsgPacket const & packet)
{
    using namespace libtbag::log::level;

    String prefix;

    Severity level = packet.getSeverity();
    if (level <= EMERGENCY_SEVERITY) {
        prefix = tces::DISPLAY_ATTRIBUTE_BG_RED;
        prefix += tces::DISPLAY_ATTRIBUTE_FG_CYAN;
    } else if (level <= ALERT_SEVERITY) {
        prefix = tces::DISPLAY_ATTRIBUTE_FG_RED;
    } else if (level <= CRITICAL_SEVERITY) {
        prefix = tces::DISPLAY_ATTRIBUTE_FG_RED;
    } else if (level <= ERROR_SEVERITY) {
        prefix = tces::DISPLAY_ATTRIBUTE_FG_YELLOW;
    } else if (level <= WARNING_SEVERITY) {
        prefix = tces::DISPLAY_ATTRIBUTE_FG_YELLOW;
    } else if (level <= NOTICE_SEVERITY) {
        prefix = tces::DISPLAY_ATTRIBUTE_FG_GREEN;
    } else if (level <= INFORMATIONAL_SEVERITY) {
        prefix = tces::DISPLAY_ATTRIBUTE_FG_GREEN;
    } else if (level <= DEBUG_SEVERITY) {
        prefix = tces::DISPLAY_ATTRIBUTE_FG_BLUE;
    }
    return prefix + makeDefault(packet) + tces::DISPLAY_ATTRIBUTE_RESET;
}

PacketGenerator::String PacketGenerator::getNowTimeString()
{
    return time::TimePoint::now().toLocalLongString();
}

PacketGenerator::String PacketGenerator::getCurrentThreadString()
{
    std::basic_stringstream<typename String::value_type> ss;
    ss << DATE_MSG_THREAD_PREFIX << std::this_thread::get_id();
    return ss.str();
}

PacketGenerator::String PacketGenerator::getSeverityString(Severity const & severity)
{
    return String(DATE_MSG_SEVERITY_PREFIX) + severity.getText() + DATE_MSG_SEVERITY_SUFFIX;
}

} // namespace msg
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

