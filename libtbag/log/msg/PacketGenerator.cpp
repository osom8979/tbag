/**
 * @file   PacketGenerator.cpp
 * @brief  PacketGenerator class implementation.
 * @author zer0
 * @date   2017-04-13
 */

#include <libtbag/log/msg/PacketGenerator.hpp>
#include <libtbag/time/TimePoint.hpp>
#include <libtbag/tty/Tces.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <sstream>
#include <thread>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log {
namespace msg {

PacketGenerator::PacketGenerator() : _type(MakeType::DEFAULT), _endl(string::NEW_LINE)
{
    // EMPTY.
}

PacketGenerator::PacketGenerator(MakeType type) : _type(type), _endl(string::NEW_LINE)
{
    // EMPTY.
}

PacketGenerator::~PacketGenerator()
{
    // EMPTY.
}

PacketGenerator::String PacketGenerator::make(LogMsgPacket const & packet)
{
    switch (_type) {
    case MakeType::DEFAULT:
        return makeDefault(packet);
    case MakeType::DEFAULT_COLOR:
        return makeDefaultColor(packet);
    case MakeType::RAW:
        return makeRaw(packet);
    default:
        return packet.getMessage();
    }
}

PacketGenerator::String PacketGenerator::makeDefault(LogMsgPacket const & packet)
{
    std::stringstream ss;
    ss << getNowTimeString() << DATE_MSG_SEPARATOR
       << getCurrentThreadString() << DATE_MSG_SEPARATOR
       << getSeverityString(packet.getSeverity()) << DATE_MSG_SEPARATOR
       << packet.c_str() << _endl;
    return ss.str();
}

PacketGenerator::String PacketGenerator::makeDefaultColor(LogMsgPacket const & packet)
{
    using namespace libtbag::log::level;

    Severity level = packet.getSeverity();
    std::stringstream ss;

    if (level <= EMERGENCY_SEVERITY) {
        ss << tces::DISPLAY_ATTRIBUTE_BG_RED << tces::DISPLAY_ATTRIBUTE_FG_CYAN;
    } else if (level <= ALERT_SEVERITY) {
        ss << tces::DISPLAY_ATTRIBUTE_FG_MAGENTA;
    } else if (level <= CRITICAL_SEVERITY) {
        ss << tces::DISPLAY_ATTRIBUTE_FG_RED;
    } else if (level <= ERROR_SEVERITY) {
        ss << tces::DISPLAY_ATTRIBUTE_FG_RED;
    } else if (level <= WARNING_SEVERITY) {
        ss << tces::DISPLAY_ATTRIBUTE_FG_YELLOW;
    } else if (level <= NOTICE_SEVERITY) {
        ss << tces::DISPLAY_ATTRIBUTE_FG_GREEN;
    } else if (level <= INFORMATIONAL_SEVERITY) {
        ss << tces::DISPLAY_ATTRIBUTE_FG_GREEN;
    } else if (level <= DEBUG_SEVERITY) {
        ss << tces::DISPLAY_ATTRIBUTE_FG_BLUE;
    }

    ss << getNowTimeString() << DATE_MSG_SEPARATOR
       << getCurrentThreadString() << DATE_MSG_SEPARATOR
       << getSeverityString(packet.getSeverity()) << DATE_MSG_SEPARATOR
       << packet.c_str() << tces::DISPLAY_ATTRIBUTE_RESET << _endl;
    return ss.str();
}

PacketGenerator::String PacketGenerator::makeRaw(LogMsgPacket const & packet)
{
    return packet.getMessage();
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

