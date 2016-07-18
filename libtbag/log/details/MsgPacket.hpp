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
#include <libtbag/log/details/Severity.hpp>
#include <libtbag/string/fmt/format.h>
#include <libtbag/Strings.hpp>
#include <libtbag/time/Time.hpp>

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
    BaseMsgPacket() : _severity(LogLevel::LEVEL_INFO), _message()
    {
        // EMPTY.
    }

    BaseMsgPacket(String const & message)
            : _severity(LogLevel::LEVEL_INFO), _message(message)
    {
        // EMPTY.
    }

    BaseMsgPacket(ValueType const * message)
            : _severity(LogLevel::LEVEL_INFO), _message(message)
    {
        // EMPTY.
    }

    BaseMsgPacket(Severity const & severity)
            : _severity(severity), _message()
    {
        // EMPTY.
    }

    BaseMsgPacket(LogLevel level)
            : _severity(level), _message()
    {
        // EMPTY.
    }

    BaseMsgPacket(Severity const & severity, String const & message)
            : _severity(severity), _message(message)
    {
        // EMPTY.
    }

    BaseMsgPacket(BaseMsgPacket const & obj) : BaseMsgPacket()
    {
        this->copy(obj);
    }

    BaseMsgPacket(BaseMsgPacket && obj) : BaseMsgPacket()
    {
        this->swap(obj);
    }

    virtual ~BaseMsgPacket()
    {
        // EMPTY.
    }

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

    operator ValueType const * () const noexcept
    {
        return _message.c_str();
    }

    operator String () const
    {
        return _message;
    }

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
    inline String getString() const noexcept
    { return _message;          }
    inline ValueType const * getStringPointer() const noexcept
    { return _message.c_str();  }
    inline std::size_t getStringSize() const noexcept
    { return _message.size();   }
    inline Severity getSeverity() const noexcept
    {   return _severity;       }

    inline void clearString() noexcept
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

public:
    template <typename ... Args>
    BaseMsgPacket & format(String const & format_string, Args && ... args)
    {
        _message = fmt::format(format_string, std::forward<Args>(args) ...);
        return *this;
    }

private:
    String getMillisecFormat(std::chrono::system_clock::time_point const & time_point) const
    {
        String result;
        time::getMillisecString(time_point, result);
        return String(CHAR_OR_WIDECHAR(ValueType, ",")) + result;
    }

public:
    String getDefaultPrefix() const
    {
        std::basic_stringstream<ValueType> ss;

        // Timestamp.
        auto tp = time::getNowSystemClock();
        time_t time = time::getTime(tp);
        ss << time::getFormatString(time::getDefaultTimestampLongFormat<ValueType>(), time::getLocalTime(time));

        // Milliseconds.
        ss << getMillisecFormat(tp);

        // Current thread.
        ss << CHAR_OR_WIDECHAR(ValueType, " @");
        ss << std::this_thread::get_id();

        // Severity.
        ss << CHAR_OR_WIDECHAR(ValueType, " [");
        ss << _severity.getText();
        ss << CHAR_OR_WIDECHAR(ValueType, "] ");

        return ss.str();
    }
};

using MsgPacket     = BaseMsgPacket<char>;
using WideMsgPacket = BaseMsgPacket<wchar_t>;

} // namespace details
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_DETAILS_MSGPACKET_HPP__

