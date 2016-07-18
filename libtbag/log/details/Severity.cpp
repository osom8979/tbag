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

Severity::Severity() noexcept : _text(), _flag()
{
    // EMPTY.
}

Severity::Severity(String const & text, Flag flag) noexcept
        : _text(text), _flag(flag)
{
    // EMPTY.
}

Severity::Severity(DefaultSeverityProperty const & property) noexcept
        : Severity(property.text, property.flag)
{
    // EMPTY.
}

Severity::Severity(LogLevel level) noexcept : Severity(DEFAULT_SEVERITY[static_cast<int>(level)])
{
    // EMPTY.
}

Severity::Severity(Severity const & obj) noexcept
{
    this->copy(obj);
}

Severity::Severity(Severity && obj) noexcept
{
    this->swap(obj);
}

Severity::~Severity() noexcept
{
    // EMPTY.
}

Severity & Severity::operator =(Severity const & obj) noexcept
{
    return this->copy(obj);
}

Severity & Severity::operator =(Severity && obj) noexcept
{
    this->swap(obj);
    return *this;
}

Severity::operator String() const noexcept
{
    return _text;
}

Severity & Severity::copy(Severity const & obj) noexcept
{
    if (this != &obj) {
        _text = obj._text;
        _flag = obj._flag;
    }
    return *this;
}

void Severity::swap(Severity & obj) noexcept
{
    if (this != &obj) {
        std::swap(_text, obj._text);
        std::swap(_flag, obj._flag);
    }
}

Severity & Severity::operator |=(Severity const & obj) noexcept
{
    _flag |= obj._flag;
    return *this;
}

Severity & Severity::operator ^=(Severity const & obj) noexcept
{
    _flag ^= obj._flag;
    return *this;
}

Severity & Severity::operator &=(Severity const & obj) noexcept
{
    _flag &= obj._flag;
    return *this;
}

bool Severity::operator ==(Severity const & obj) const noexcept
{
    return _flag == obj._flag;
}

bool Severity::operator !=(Severity const & obj) const noexcept
{
    return _flag != obj._flag;
}

void Severity::setText(String const & text) noexcept
{
    _text = text;
}

Severity::String Severity::getText() const noexcept
{
    return _text;
}

void Severity::setFlag(Flag flag) noexcept
{
    _flag = flag;
}

Severity::Flag Severity::getFlag() const noexcept
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

