/**
 * @file   MsgPacket.hpp
 * @brief  MsgPacket class prototype.
 * @author zer0
 * @date   2016-07-09
 *
 * @remarks
 *  Log message packet.
 *
 * @translate{ko, 한국인을 울린 그 맛!}
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_MSG_MSGPACKET_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_MSG_MSGPACKET_HPP__

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
 * MsgPacket class prototype.
 *
 * @author zer0
 * @date   2016-07-09
 */
class MsgPacket
{
public:
    using Severity = level::Severity;
    using String   = std::string;

private:
    Severity _severity;
    String   _message;

public:
    // @formatter:off
    MsgPacket() : _severity(level::INFORMATIONAL_SEVERITY), _message()
    { /* EMPTY. */ }
    MsgPacket(Severity const & severity) : _severity(severity), _message()
    { /* EMPTY. */ }
    MsgPacket(String const & msg) : _severity(level::INFORMATIONAL_SEVERITY), _message(msg)
    { /* EMPTY. */ }
    MsgPacket(Severity const & severity, String const & msg) : _severity(severity), _message(msg)
    { /* EMPTY. */ }
    MsgPacket(MsgPacket const & obj) : MsgPacket()
    { (*this) = obj; }
    MsgPacket(MsgPacket && obj) : MsgPacket()
    { (*this) = std::move(obj); }
    virtual ~MsgPacket()
    { /* EMPTY. */ }
    // @formatter:on

public:
    inline MsgPacket & operator =(MsgPacket const & obj)
    {
        if (this != &obj) {
            _severity = obj._severity;
            _message  = obj._message;
        }
        return *this;
    }

    inline MsgPacket & operator =(MsgPacket && obj)
    {
        if (this != &obj) {
            _severity = std::move(obj._severity);
            _message.swap(obj._message);
        }
        return *this;
    }

    inline MsgPacket & operator =(char const * message)
    {
        _message = message;
        return *this;
    }

    inline MsgPacket & operator =(String const & message)
    {
        _message = message;
        return *this;
    }

    inline friend void swap(MsgPacket & lh, MsgPacket & rh)
    {
        if (&lh != &rh) {
            std::swap(lh._severity, rh._severity);
            std::swap(lh._message, rh._message);
        }
    }

public:
    // @formatter:off
    inline operator char const * () const
    { return _message.c_str(); }
    inline operator String () const
    { return _message; }
    // @formatter:on

public:
    // @formatter:off
    inline Severity getSeverity() const TBAG_NOEXCEPT
    { return _severity; }
    inline String getMessage() const
    { return _message; }

    inline void setSeverity(Severity const & severity) TBAG_NOEXCEPT
    { _severity = severity; }
    inline void setMessage(String const & message)
    { _message = message; }
    // @formatter:on

public:
    // @formatter:off
    inline char const * c_str() const TBAG_NOEXCEPT_SPECIFIER(TBAG_NOEXCEPT_OPERATOR(_message.c_str()))
    { return _message.c_str(); }
    inline bool empty() const TBAG_NOEXCEPT_SPECIFIER(TBAG_NOEXCEPT_OPERATOR(_message.empty()))
    { return _message.empty(); }
    inline std::size_t size() const TBAG_NOEXCEPT_SPECIFIER(TBAG_NOEXCEPT_OPERATOR(_message.size()))
    { return _message.size(); }
    inline void clear() TBAG_NOEXCEPT_SPECIFIER(TBAG_NOEXCEPT_OPERATOR(_message.clear()))
    { _message.clear(); }
    // @formatter:on

public:
    friend MsgPacket & operator <<(MsgPacket & msg, char value)
    {
        msg._message.append(1, value);
        return msg;
    }

#ifndef MSGPACKET_LEFT_SHIFT_OPERATOR
#define MSGPACKET_LEFT_SHIFT_OPERATOR(type, value, append)      \
    friend MsgPacket & operator <<(MsgPacket & msg, type value) \
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

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_MSG_MSGPACKET_HPP__

