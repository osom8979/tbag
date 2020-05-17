/**
 * @file   Byte.cpp
 * @brief  Byte class implementation.
 * @author zer0
 * @date   2020-05-17
 */

#include <libtbag/util/Byte.hpp>
#include <libtbag/util/ByteString.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/Unit.hpp>

#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace util {

Byte::Byte() TBAG_NOEXCEPT : _byte()
{
    // EMPTY.
}

Byte::Byte(std::size_t byte) TBAG_NOEXCEPT : _byte(byte)
{
    // EMPTY.
}

Byte::Byte(std::string const & text)
{
    fromString(text);
}

Byte::Byte(Byte const & obj) TBAG_NOEXCEPT : _byte(obj._byte)
{
    // EMPTY.
}

Byte::Byte(Byte && obj) TBAG_NOEXCEPT : _byte(std::move(obj._byte))
{
    // EMPTY.
}

Byte::~Byte()
{
    // EMPTY.
}

Byte & Byte::operator =(std::size_t byte) TBAG_NOEXCEPT
{
    _byte = byte;
    return *this;
}

Byte & Byte::operator =(std::string const & text)
{
    fromString(text);
    return *this;
}

Byte & Byte::operator =(Byte const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _byte = obj._byte;
    }
    return *this;
}

Byte & Byte::operator =(Byte && obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _byte = std::move(obj._byte);
    }
    return *this;
}

void Byte::swap(Byte & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        std::swap(_byte, obj._byte);
    }
}

// clang-format off
static_assert(KILO_BYTE_TO_BYTE >= 1, "Avoid zero division errors.");
static_assert(MEGA_BYTE_TO_BYTE >= 1, "Avoid zero division errors.");
static_assert(GIGA_BYTE_TO_BYTE >= 1, "Avoid zero division errors.");
static_assert(TERA_BYTE_TO_BYTE >= 1, "Avoid zero division errors.");
static_assert(PETA_BYTE_TO_BYTE >= 1, "Avoid zero division errors.");
static_assert( EXA_BYTE_TO_BYTE >= 1, "Avoid zero division errors.");
std::size_t Byte::toByte    () const TBAG_NOEXCEPT { return _byte; }
std::size_t Byte::toKiloByte() const TBAG_NOEXCEPT { return _byte / KILO_BYTE_TO_BYTE; }
std::size_t Byte::toMegaByte() const TBAG_NOEXCEPT { return _byte / MEGA_BYTE_TO_BYTE; }
std::size_t Byte::toGigaByte() const TBAG_NOEXCEPT { return _byte / GIGA_BYTE_TO_BYTE; }
std::size_t Byte::toTeraByte() const TBAG_NOEXCEPT { return _byte / TERA_BYTE_TO_BYTE; }
std::size_t Byte::toPetaByte() const TBAG_NOEXCEPT { return _byte / PETA_BYTE_TO_BYTE; }
std::size_t Byte::toExaByte () const TBAG_NOEXCEPT { return _byte / EXA_BYTE_TO_BYTE; }
// clang-format on

std::string Byte::toUpperByteText() const
{
    return libtbag::util::toUpperByteText(_byte);
}

Err Byte::fromString(std::string const & text)
{
    _byte = libtbag::util::toByteSize(text);
    return E_SUCCESS;
}

std::string Byte::toString() const
{
    return toUpperByteText();
}

} // namespace util

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

