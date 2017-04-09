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

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_DETAILS_MSGPACKET_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_DETAILS_MSGPACKET_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/log/details/Severity.hpp>

#include <string>
#include <sstream>
#include <utility>
#include <thread>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log     {
namespace details {

/**
 * MsgPacket class prototype.
 *
 * @author zer0
 * @date   2016-07-09
 */
class MsgPacket
{
private:
    Severity _severity;
    std::string _message;

public:
    MsgPacket() : _severity(INFORMATIONAL_SEVERITY), _message()
    { /* EMPTY. */ }

    virtual ~MsgPacket()
    { /* EMPTY. */ }

public:
    MsgPacket(Severity const & severity) : _severity(severity), _message()
    { /* EMPTY. */ }

    MsgPacket(std::string const & msg) : _severity(INFORMATIONAL_SEVERITY), _message(msg)
    { /* EMPTY. */ }

    MsgPacket(char const * msg) : _severity(INFORMATIONAL_SEVERITY), _message(msg)
    { /* EMPTY. */ }

    MsgPacket(Severity const & severity, std::string const & msg) : _severity(severity), _message(msg)
    { /* EMPTY. */ }

public:
    MsgPacket(MsgPacket const & obj) : MsgPacket()
    { copy(obj); }

    MsgPacket(MsgPacket && obj) : MsgPacket()
    { swap(obj); }

public:
    inline MsgPacket & operator =(MsgPacket const & obj)
    {
        return copy(obj);
    }

    inline MsgPacket & operator =(MsgPacket && obj)
    {
        swap(obj);
        return *this;
    }

    inline MsgPacket & operator =(std::string const & message)
    {
        _message = message;
        return *this;
    }

    inline MsgPacket & operator =(char const * message)
    {
        _message = message;
        return *this;
    }

public:
    inline operator char const * () const TBAG_NOEXCEPT
    { return _message.c_str(); }
    inline operator std::string () const
    { return _message; }

public:
    MsgPacket & copy(MsgPacket const & obj)
    {
        if (this != &obj) {
            _severity = obj._severity;
            _message  = obj._message;
        }
        return *this;
    }

    void swap(MsgPacket & obj)
    {
        if (this != &obj) {
            std::swap(_severity, obj._severity);
            std::swap(_message , obj._message);
        }
    }

    void swap(MsgPacket && obj)
    {
        if (this != &obj) {
            _severity = std::move(obj._severity);
            _message  = std::move(obj._message);
        }
    }

public:
    inline std::string getString() const TBAG_NOEXCEPT
    { return _message;          }
    inline char const * getStringPointer() const TBAG_NOEXCEPT
    { return _message.c_str();  }
    inline std::size_t getStringSize() const TBAG_NOEXCEPT
    { return _message.size();   }
    inline Severity getSeverity() const TBAG_NOEXCEPT
    {   return _severity;       }

    inline void clearString() TBAG_NOEXCEPT
    { _message.clear();         }

public:
    friend MsgPacket & operator <<(MsgPacket & msg, char value)
    {
        msg._message.append(1, value);
        return msg;
    }

#ifndef __LEFT_SHIFT_OPERATOR
#define __LEFT_SHIFT_OPERATOR(type, value, append)              \
    friend MsgPacket & operator <<(MsgPacket & msg, type value) \
    { msg._message += append; return msg; }
#endif

#ifndef __LEFT_SHIFT_OPERATOR_TO_STRING
#define __LEFT_SHIFT_OPERATOR_TO_STRING(type) \
    __LEFT_SHIFT_OPERATOR(type, value, std::to_string(static_cast<type>(value)))
#endif

public:
    __LEFT_SHIFT_OPERATOR_TO_STRING(short);
    __LEFT_SHIFT_OPERATOR_TO_STRING(int);
    __LEFT_SHIFT_OPERATOR_TO_STRING(long);
    __LEFT_SHIFT_OPERATOR_TO_STRING(long long);
    __LEFT_SHIFT_OPERATOR_TO_STRING(unsigned char);
    __LEFT_SHIFT_OPERATOR_TO_STRING(unsigned short);
    __LEFT_SHIFT_OPERATOR_TO_STRING(unsigned int);
    __LEFT_SHIFT_OPERATOR_TO_STRING(unsigned long);
    __LEFT_SHIFT_OPERATOR_TO_STRING(unsigned long long);
    __LEFT_SHIFT_OPERATOR_TO_STRING(float);
    __LEFT_SHIFT_OPERATOR_TO_STRING(double);
    __LEFT_SHIFT_OPERATOR_TO_STRING(long double);

public:
    __LEFT_SHIFT_OPERATOR(void *, v, std::to_string(reinterpret_cast<std::size_t>(v)));
    __LEFT_SHIFT_OPERATOR(char const *, v, v);
    __LEFT_SHIFT_OPERATOR(std::string const &, v, v);

#undef __LEFT_SHIFT_OPERATOR_TO_STRING
#undef __LEFT_SHIFT_OPERATOR
};

} // namespace details
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_DETAILS_MSGPACKET_HPP__

