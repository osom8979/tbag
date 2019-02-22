/**
 * @file   Uuid.cpp
 * @brief  Uuid class implementation.
 * @author zer0
 * @date   2017-07-01
 */

#include <libtbag/id/Uuid.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cstring>
#include <algorithm>
#include <utility>
#include <exception>

#include <openssl/rand.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace id {

Uuid::Uuid() TBAG_NOEXCEPT
{
    fill(0x00);
}

Uuid::Uuid(Version version)
{
    init(version);
}

Uuid::Uuid(std::string const & str)
{
    if (fromString(str) != Err::E_SUCCESS) {
        fill(0x00);
    }
}

Uuid::Uuid(Uuid const & obj) TBAG_NOEXCEPT
{
    (*this) = obj;
}

Uuid::Uuid(Uuid && obj) TBAG_NOEXCEPT
{
    (*this) = std::move(obj);
}

Uuid::~Uuid()
{
    // EMPTY.
}

Uuid & Uuid::operator =(Uuid const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        std::copy(obj.begin(), obj.end(), begin());
    }
    return *this;
}

Uuid & Uuid::operator =(Uuid && obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        swap(obj);
    }
    return *this;
}

bool Uuid::operator ==(Uuid const & obj) const TBAG_NOEXCEPT
{
    for (std::size_t i = 0; i < BYTE_SIZE; ++i) {
        if (id.data[i] != obj.id.data[i]) {
            return false;
        }
    }
    return true;
}

bool Uuid::operator !=(Uuid const & obj) const TBAG_NOEXCEPT
{
    return !((*this) == obj);
}

bool Uuid::operator ==(std::string const & str) const
{
    return (*this) == Uuid(str);
}

bool Uuid::operator !=(std::string const & str) const
{
    return !((*this) == str);
}

void Uuid::fill(value_type const & value) TBAG_NOEXCEPT
{
    ::memset(id.data, value, BYTE_SIZE);
}

void Uuid::swap(Uuid & obj) TBAG_NOEXCEPT
{
    std::swap_ranges(obj.id.data, obj.id.data + BYTE_SIZE, id.data);
}

Uuid::reference Uuid::at(size_type i)
{
    if (i >= BYTE_SIZE) {
        throw std::out_of_range(std::string("Uuid::at() Out of range exception."));
    }
    return id.data[i];
}

Uuid::const_reference Uuid::at(size_type i) const
{
    if (i >= BYTE_SIZE) {
        throw std::out_of_range(std::string("Uuid::at() Out of range exception."));
    }
    return id.data[i];
}

bool Uuid::initVersion1()
{
    return false;
}

bool Uuid::initVersion2()
{
    return false;
}

bool Uuid::initVersion3()
{
    return false;
}

bool Uuid::initVersion4()
{
    // RAND_bytes() puts num cryptographically strong pseudo-random bytes into buf.
    // An error occurs if the PRNG has not been seeded with enough randomness to ensure an unpredictable byte sequence.
    // See: https://www.openssl.org/docs/man1.0.2/crypto/RAND_bytes.html
    if (::RAND_bytes(id.data, sizeof(id)) == 0) {
        return false;
    }

    // 01. Set the two most significant bits (bits 6 and 7) of the
    //     clock_seq_hi_and_reserved to zero and one, respectively.
    // 02. Set the four most significant bits (bits 12 through 15) of the
    //     time_hi_and_version field to the 4-bit version number from Section 4.1.3.
    // 03. Set all the other bits to randomly (or pseudo-randomly) chosen values.

    id.clock_seq_hi_and_reserved &= 0x3F/*0b00111111*/;
    id.clock_seq_hi_and_reserved |= VARIANT_10X;

    id.data[6] &= 0xFF;
    id.data[6] |= HI_VERSION_04;
    return true;
}

bool Uuid::initVersion5()
{
    return false;
}

bool Uuid::init(Version version)
{
    switch (version) {
    case Version::UUID_VER_1: return initVersion1();
    case Version::UUID_VER_2: return initVersion2();
    case Version::UUID_VER_3: return initVersion3();
    case Version::UUID_VER_4: return initVersion4();
    case Version::UUID_VER_5: return initVersion5();
    default: return initVersion4();
    }
}

std::string Uuid::toString()
{
    std::stringstream ss;
    ss << string::convertByteToHexString(id.data[ 0])
       << string::convertByteToHexString(id.data[ 1])
       << string::convertByteToHexString(id.data[ 2])
       << string::convertByteToHexString(id.data[ 3]) << UUID_STR_DASH
       << string::convertByteToHexString(id.data[ 4])
       << string::convertByteToHexString(id.data[ 5]) << UUID_STR_DASH
       << string::convertByteToHexString(id.data[ 6])
       << string::convertByteToHexString(id.data[ 7]) << UUID_STR_DASH
       << string::convertByteToHexString(id.data[ 8])
       << string::convertByteToHexString(id.data[ 9]) << UUID_STR_DASH
       << string::convertByteToHexString(id.data[10])
       << string::convertByteToHexString(id.data[11])
       << string::convertByteToHexString(id.data[12])
       << string::convertByteToHexString(id.data[13])
       << string::convertByteToHexString(id.data[14])
       << string::convertByteToHexString(id.data[15]);
    return ss.str();
}

Err Uuid::fromString(std::string const & str)
{
    if (str.size() != NIL_UUID_STR_LENGTH) {
        return Err::E_PARSING;
    }

    // @formatter:off
    using namespace libtbag::string;
    if (convertHexCharToByte(str[0], str[1], id.data[0]) != Err::E_SUCCESS) { return Err::E_PARSING; }
    if (convertHexCharToByte(str[2], str[3], id.data[1]) != Err::E_SUCCESS) { return Err::E_PARSING; }
    if (convertHexCharToByte(str[4], str[5], id.data[2]) != Err::E_SUCCESS) { return Err::E_PARSING; }
    if (convertHexCharToByte(str[6], str[7], id.data[3]) != Err::E_SUCCESS) { return Err::E_PARSING; }
    if (str[8] != UUID_STR_DASH) { return Err::E_PARSING; }
    if (convertHexCharToByte(str[ 9], str[10], id.data[4]) != Err::E_SUCCESS) { return Err::E_PARSING; }
    if (convertHexCharToByte(str[11], str[12], id.data[5]) != Err::E_SUCCESS) { return Err::E_PARSING; }
    if (str[13] != UUID_STR_DASH) { return Err::E_PARSING; }
    if (convertHexCharToByte(str[14], str[15], id.data[6]) != Err::E_SUCCESS) { return Err::E_PARSING; }
    if (convertHexCharToByte(str[16], str[17], id.data[7]) != Err::E_SUCCESS) { return Err::E_PARSING; }
    if (str[18] != UUID_STR_DASH) { return Err::E_PARSING; }
    if (convertHexCharToByte(str[19], str[20], id.data[8]) != Err::E_SUCCESS) { return Err::E_PARSING; }
    if (convertHexCharToByte(str[21], str[22], id.data[9]) != Err::E_SUCCESS) { return Err::E_PARSING; }
    if (str[23] != UUID_STR_DASH) { return Err::E_PARSING; }
    if (convertHexCharToByte(str[24], str[25], id.data[10]) != Err::E_SUCCESS) { return Err::E_PARSING; }
    if (convertHexCharToByte(str[26], str[27], id.data[11]) != Err::E_SUCCESS) { return Err::E_PARSING; }
    if (convertHexCharToByte(str[28], str[29], id.data[12]) != Err::E_SUCCESS) { return Err::E_PARSING; }
    if (convertHexCharToByte(str[30], str[31], id.data[13]) != Err::E_SUCCESS) { return Err::E_PARSING; }
    if (convertHexCharToByte(str[32], str[33], id.data[14]) != Err::E_SUCCESS) { return Err::E_PARSING; }
    if (convertHexCharToByte(str[34], str[35], id.data[15]) != Err::E_SUCCESS) { return Err::E_PARSING; }
    // @formatter:on
    return Err::E_SUCCESS;
}

Uuid Uuid::nil() TBAG_NOEXCEPT
{
    return Uuid();
}

Uuid Uuid::ver4() TBAG_NOEXCEPT
{
    return Uuid(Uuid::Version::UUID_VER_4);
}

} // namespace id

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

