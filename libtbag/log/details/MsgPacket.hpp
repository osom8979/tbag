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
 * BaseMsgPacket class prototype.
 *
 * @author zer0
 * @date   2016-07-09
 */
template <typename CharType = char>
class BaseMsgPacket
{
public:
    using ValueType = CharType;
    using String    = std::basic_string<ValueType>;

    static_assert(std::is_pod<ValueType>::value
            , "Character type of BaseStrings must be a POD");
    static_assert(std::is_same<ValueType, typename String::value_type>::value
            , "String::value_type must be the same type as ValueType");

private:
    Severity _severity;
    String   _message;

public:
    // @formatter:off
    BaseMsgPacket()                                               : _severity(LogLevel::LEVEL_INFO), _message()    { }
    BaseMsgPacket(LogLevel level)                                 : _severity(level),                _message()    { }
    BaseMsgPacket(Severity  const & severity)                     : _severity(severity),             _message()    { }
    BaseMsgPacket(String    const & msg)                          : _severity(LogLevel::LEVEL_INFO), _message(msg) { }
    BaseMsgPacket(ValueType const * msg)                          : _severity(LogLevel::LEVEL_INFO), _message(msg) { }
    BaseMsgPacket(Severity  const & severity, String const & msg) : _severity(severity),             _message(msg) { }
    virtual ~BaseMsgPacket() { }
    // @formatter:on

public:
    BaseMsgPacket(BaseMsgPacket const & obj) : BaseMsgPacket()
    { this->copy(obj); }
    BaseMsgPacket(BaseMsgPacket && obj) : BaseMsgPacket()
    { this->swap(obj); }

public:
    BaseMsgPacket & operator =(BaseMsgPacket const & obj)
    {
        return this->copy(obj);
    }

    BaseMsgPacket & operator =(BaseMsgPacket && obj)
    {
        this->swap(obj);
        return *this;
    }

    BaseMsgPacket & operator =(String const & message)
    {
        _message = message;
        return *this;
    }

    BaseMsgPacket & operator =(ValueType const * message)
    {
        _message = message;
        return *this;
    }

public:
    inline operator ValueType const * () const TBAG_NOEXCEPT
    { return _message.c_str(); }
    inline operator String () const
    { return _message; }

public:
    BaseMsgPacket & copy(BaseMsgPacket const & obj)
    {
        if (this != &obj) {
            _severity = obj._severity;
            _message = obj._message;
        }
        return *this;
    }

    void swap(BaseMsgPacket & obj)
    {
        if (this != &obj) {
            std::swap(_severity, obj._severity);
            std::swap(_message, obj._message);
        }
    }

    void swap(BaseMsgPacket && obj)
    {
        if (this != &obj) {
            _severity = std::move(obj._severity);
            _message  = std::move(obj._message);
        }
    }

public:
    inline String getString() const TBAG_NOEXCEPT
    { return _message;          }
    inline ValueType const * getStringPointer() const TBAG_NOEXCEPT
    { return _message.c_str();  }
    inline std::size_t getStringSize() const TBAG_NOEXCEPT
    { return _message.size();   }
    inline Severity getSeverity() const TBAG_NOEXCEPT
    {   return _severity;       }

    inline void clearString() TBAG_NOEXCEPT
    { _message.clear();         }

public:
#define __LEFT_SHIFT_OPERATOR(type, value, append)  \
    friend BaseMsgPacket &                          \
    operator <<(BaseMsgPacket & msg, type value)    \
    {                                               \
        msg._message += append;                     \
        return msg;                                 \
    }

#define __LEFT_SHIFT_OPERATOR_TO_STRING(type) \
    __LEFT_SHIFT_OPERATOR(type, value, std::to_string(static_cast<type>(value)))

public:
    friend BaseMsgPacket & operator <<(BaseMsgPacket & msg, ValueType value)
    {
        msg._message.append(1, value);
        return msg;
    }

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
    __LEFT_SHIFT_OPERATOR(void *, value, std::to_string(reinterpret_cast<std::size_t>(value)));
    __LEFT_SHIFT_OPERATOR(ValueType const *, value, value);
    __LEFT_SHIFT_OPERATOR(String const &, value, value);
};

using MsgPacket     = BaseMsgPacket<char>;
using WideMsgPacket = BaseMsgPacket<wchar_t>;

} // namespace details
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_DETAILS_MSGPACKET_HPP__

