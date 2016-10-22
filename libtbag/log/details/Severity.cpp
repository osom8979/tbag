/**
 * @file   Severity.cpp
 * @brief  Severity class implementation.
 * @author zer0
 * @date   2016-07-10
 */

#include <libtbag/log/details/Severity.hpp>
#include <iostream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log     {
namespace details {

Severity::Severity() TBAG_NOEXCEPT : _text(), _flag()
{
    // EMPTY.
}

Severity::Severity(String const & text, Flag flag) TBAG_NOEXCEPT
        : _text(text), _flag(flag)
{
    // EMPTY.
}

Severity::Severity(DefaultSeverityProperty const & property) TBAG_NOEXCEPT
        : Severity(property.text, property.flag)
{
    // EMPTY.
}

Severity::Severity(LogLevel level) TBAG_NOEXCEPT : Severity(DEFAULT_SEVERITY[static_cast<int>(level)])
{
    // EMPTY.
}

Severity::Severity(Severity const & obj) TBAG_NOEXCEPT
{
    this->copy(obj);
}

Severity::Severity(Severity && obj) TBAG_NOEXCEPT
{
    this->swap(obj);
}

Severity::~Severity() TBAG_NOEXCEPT
{
    // EMPTY.
}

Severity & Severity::operator =(Severity const & obj) TBAG_NOEXCEPT
{
    return this->copy(obj);
}

Severity & Severity::operator =(Severity && obj) TBAG_NOEXCEPT
{
    this->swap(obj);
    return *this;
}

Severity::operator String() const TBAG_NOEXCEPT
{
    return _text;
}

Severity & Severity::copy(Severity const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _text = obj._text;
        _flag = obj._flag;
    }
    return *this;
}

void Severity::swap(Severity & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        std::swap(_text, obj._text);
        std::swap(_flag, obj._flag);
    }
}

Severity & Severity::operator |=(Severity const & obj) TBAG_NOEXCEPT
{
    _flag |= obj._flag;
    return *this;
}

Severity & Severity::operator ^=(Severity const & obj) TBAG_NOEXCEPT
{
    _flag ^= obj._flag;
    return *this;
}

Severity & Severity::operator &=(Severity const & obj) TBAG_NOEXCEPT
{
    _flag &= obj._flag;
    return *this;
}

bool Severity::operator ==(Severity const & obj) const TBAG_NOEXCEPT
{
    return _flag == obj._flag;
}

bool Severity::operator !=(Severity const & obj) const TBAG_NOEXCEPT
{
    return _flag != obj._flag;
}

void Severity::setText(String const & text) TBAG_NOEXCEPT
{
    _text = text;
}

Severity::String Severity::getText() const TBAG_NOEXCEPT
{
    return _text;
}

void Severity::setFlag(Flag flag) TBAG_NOEXCEPT
{
    _flag = flag;
}

Severity::Flag Severity::getFlag() const TBAG_NOEXCEPT
{
    return _flag;
}

bool Severity::isContain(Flag flag)
{
    return ((_flag & flag) == flag);
}

} // namespace details
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

