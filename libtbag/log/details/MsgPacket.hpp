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

#include <string>
#include <algorithm>

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
    BaseMsgPacket() : _severity(), _message()
    {
        // EMPTY.
    }

    BaseMsgPacket(String const & message) : _severity(), _message(message)
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

    String operator ()() const
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

public:
    inline String getString() const noexcept
    {
        return _message;
    }

    inline ValueType const * getStringPointer() const noexcept
    {
        return _message.c_str();
    }

    inline std::size_t getStringSize() const noexcept
    {
        return _message.size();
    }
};

using MsgPacket     = BaseMsgPacket<char>;
using WideMsgPacket = BaseMsgPacket<wchar_t>;

} // namespace details
} //namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_DETAILS_MSGPACKET_HPP__

