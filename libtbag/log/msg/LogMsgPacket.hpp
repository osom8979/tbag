/**
 * @file   LogMsgPacket.hpp
 * @brief  LogMsgPacket class prototype.
 * @author zer0
 * @date   2016-07-09
 * @date   2016-11-14 (MsgPacket -> LogMsgPacket)
 *
 * @remarks
 *  Log message packet.
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_MSG_LOGMSGPACKET_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_MSG_LOGMSGPACKET_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/log/level/Severity.hpp>

#include <string>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log {
namespace msg {

/**
 * LogMsgPacket class prototype.
 *
 * @author zer0
 * @date   2016-07-09
 * @date   2016-11-14 (MsgPacket -> LogMsgPacket)
 */
class LogMsgPacket
{
public:
    using Severity = level::Severity;
    using String   = std::string;

private:
    Severity _severity;
    String   _message;

public:
    // clang-format off
    LogMsgPacket() : _severity(level::INFORMATIONAL_SEVERITY), _message()
    { /* EMPTY. */ }
    LogMsgPacket(Severity const & severity) : _severity(severity), _message()
    { /* EMPTY. */ }
    LogMsgPacket(String const & msg) : _severity(level::INFORMATIONAL_SEVERITY), _message(msg)
    { /* EMPTY. */ }
    LogMsgPacket(Severity const & severity, String const & msg) : _severity(severity), _message(msg)
    { /* EMPTY. */ }
    LogMsgPacket(LogMsgPacket const & obj) : LogMsgPacket()
    { (*this) = obj; }
    LogMsgPacket(LogMsgPacket && obj) : LogMsgPacket()
    { (*this) = std::move(obj); }
    virtual ~LogMsgPacket()
    { /* EMPTY. */ }
    // clang-format on

public:
    inline LogMsgPacket & operator =(LogMsgPacket const & obj)
    {
        if (this != &obj) {
            _severity = obj._severity;
            _message  = obj._message;
        }
        return *this;
    }

    inline LogMsgPacket & operator =(LogMsgPacket && obj)
    {
        if (this != &obj) {
            _severity = std::move(obj._severity);
            _message.swap(obj._message);
        }
        return *this;
    }

    inline LogMsgPacket & operator =(char const * message)
    {
        _message = message;
        return *this;
    }

    inline LogMsgPacket & operator =(String const & message)
    {
        _message = message;
        return *this;
    }

    inline friend void swap(LogMsgPacket & lh, LogMsgPacket & rh)
    {
        if (&lh != &rh) {
            std::swap(lh._severity, rh._severity);
            std::swap(lh._message, rh._message);
        }
    }

public:
    // clang-format off
    inline operator char const * () const
    { return _message.c_str(); }
    inline operator String () const
    { return _message; }
    // clang-format on

public:
    // clang-format off
    inline Severity getSeverity() const TBAG_NOEXCEPT
    { return _severity; }
    inline String getMessage() const
    { return _message; }

    inline void setSeverity(Severity const & severity) TBAG_NOEXCEPT
    { _severity = severity; }
    inline void setMessage(String const & message)
    { _message = message; }
    // clang-format on

public:
    // clang-format off
    inline char const * c_str() const TBAG_NOEXCEPT_SP_OP(_message.c_str())
    { return _message.c_str(); }
    inline bool empty() const TBAG_NOEXCEPT_SP_OP(_message.empty())
    { return _message.empty(); }
    inline std::size_t size() const TBAG_NOEXCEPT_SP_OP(_message.size())
    { return _message.size(); }
    inline void clear() TBAG_NOEXCEPT_SP_OP(_message.clear())
    { _message.clear(); }
    // clang-format on

public:
    friend LogMsgPacket & operator <<(LogMsgPacket & msg, char value)
    {
        msg._message.append(1, value);
        return msg;
    }

#ifndef MSGPACKET_LEFT_SHIFT_OPERATOR
#define MSGPACKET_LEFT_SHIFT_OPERATOR(type, value, append)      \
    friend LogMsgPacket & operator <<(LogMsgPacket & msg, type value) \
    { msg._message += append; return msg; }
#endif

#ifndef MSGPACKET_LEFT_SHIFT_OPERATOR_TO_STRING
#define MSGPACKET_LEFT_SHIFT_OPERATOR_TO_STRING(type) \
    MSGPACKET_LEFT_SHIFT_OPERATOR(type, value, std::to_string(static_cast<type>(value)))
#endif

    MSGPACKET_LEFT_SHIFT_OPERATOR_TO_STRING(short);
    MSGPACKET_LEFT_SHIFT_OPERATOR_TO_STRING(int);
    MSGPACKET_LEFT_SHIFT_OPERATOR_TO_STRING(long);
    MSGPACKET_LEFT_SHIFT_OPERATOR_TO_STRING(long long);
    MSGPACKET_LEFT_SHIFT_OPERATOR_TO_STRING(unsigned char);
    MSGPACKET_LEFT_SHIFT_OPERATOR_TO_STRING(unsigned short);
    MSGPACKET_LEFT_SHIFT_OPERATOR_TO_STRING(unsigned int);
    MSGPACKET_LEFT_SHIFT_OPERATOR_TO_STRING(unsigned long);
    MSGPACKET_LEFT_SHIFT_OPERATOR_TO_STRING(unsigned long long);
    MSGPACKET_LEFT_SHIFT_OPERATOR_TO_STRING(float);
    MSGPACKET_LEFT_SHIFT_OPERATOR_TO_STRING(double);
    MSGPACKET_LEFT_SHIFT_OPERATOR_TO_STRING(long double);

    MSGPACKET_LEFT_SHIFT_OPERATOR(void *, v, std::to_string(reinterpret_cast<std::size_t>(v)));
    MSGPACKET_LEFT_SHIFT_OPERATOR(char const *, v, v);
    MSGPACKET_LEFT_SHIFT_OPERATOR(String const &, v, v);

#undef MSGPACKET_LEFT_SHIFT_OPERATOR_TO_STRING
#undef MSGPACKET_LEFT_SHIFT_OPERATOR
};

} // namespace msg
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_MSG_LOGMSGPACKET_HPP__

