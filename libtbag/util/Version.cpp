/**
 * @file   Version.cpp
 * @brief  Version class implementation.
 * @author zer0
 * @date   2017-05-25
 */

#include <libtbag/util/Version.hpp>
#include <libtbag/algorithm/Swap.hpp>
#include <libtbag/string/Format.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>
#include <utility>
#include <sstream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace util {

Version::Version() : _major(0), _minor(0), _patch(0), _tweak()
{
    // EMPTY.
}

Version::Version(uint32_t major, uint32_t minor, uint32_t patch, std::string const & tweak)
        : _major(major), _minor(minor), _patch(patch), _tweak(tweak)
{
    // EMPTY.
}

Version::Version(std::string const & ver) : _major(0), _minor(0), _patch(0), _tweak()
{
    fromString(ver, *this);
}

Version::Version(uint32_t version, uint32_t major_unit, uint32_t minor_unit, uint32_t patch_unit)
{
    fromNumeric(version, major_unit, minor_unit, patch_unit);
}

Version::Version(Version const & obj)
{
    (*this) = obj;
}

Version::Version(Version && obj)
{
    (*this) = std::move(obj);
}

Version::~Version()
{
    // EMPTY.
}

Version & Version::operator =(Version const & obj)
{
    if (this != &obj) {
        _major = obj._major;
        _minor = obj._minor;
        _patch = obj._patch;
        _tweak = obj._tweak;
    }
    return *this;
}

Version & Version::operator =(Version && obj)
{
    if (this != &obj) {
        swap(obj);
    }
    return *this;
}

void Version::clear()
{
    _major = 0;
    _minor = 0;
    _patch = 0;
    _tweak.clear();
}

void Version::swap(Version & obj)
{
    using namespace libtbag::algorithm;
    swapWithPod(_major, obj._major);
    swapWithPod(_minor, obj._minor);
    swapWithPod(_patch, obj._patch);
    _tweak.swap(obj._tweak);
}

bool Version::eq(Version const & ver) const TBAG_NOEXCEPT
{
    if (ver._patch != 0) {
        return _major == ver._major && _minor == ver._minor && _patch == ver._patch;
    }
    if (ver._minor != 0) {
        return _major == ver._major && _minor == ver._minor;
    }
    return _major == ver._major;
}

bool Version::ne(Version const & ver) const TBAG_NOEXCEPT
{
    return !eq(ver);
}

bool Version::lt(Version const & ver) const TBAG_NOEXCEPT
{
    if (_major < ver._major) {
        return true;
    } else if (_major > ver._major) {
        return false;
    }
    if (_minor < ver._minor) {
        return true;
    } else if (_minor > ver._minor) {
        return false;
    }
    if (_patch < ver._patch) {
        return true;
    } else if (_patch > ver._patch) {
        return false;
    }
    return false;
}

bool Version::gt(Version const & ver) const TBAG_NOEXCEPT
{
    if (_major > ver._major) {
        return true;
    } else if (_major < ver._major) {
        return false;
    }
    if (_minor > ver._minor) {
        return true;
    } else if (_minor < ver._minor) {
        return false;
    }
    if (_patch > ver._patch) {
        return true;
    } else if (_patch < ver._patch) {
        return false;
    }
    return false;
}

bool Version::le(Version const & ver) const TBAG_NOEXCEPT
{
    return lt(ver) || eq(ver);
}

bool Version::ge(Version const & ver) const TBAG_NOEXCEPT
{
    return gt(ver) || eq(ver);
}

Err Version::fromString(std::string const & version)
{
    return fromString(version, *this);
}

Err Version::fromNumeric(uint32_t version, uint32_t major_unit, uint32_t minor_unit, uint32_t patch_unit)
{
    assert(major_unit > minor_unit);
    assert(minor_unit > patch_unit);
    _major = static_cast<uint32_t>(version/major_unit);
    _minor = static_cast<uint32_t>((version-(_major*major_unit))/minor_unit);
    _patch = static_cast<uint32_t>((version-(_major*major_unit)-(_minor*minor_unit))/patch_unit);
    return E_SUCCESS;
}

std::string Version::toString() const
{
    return toShortString(*this);
}

std::string Version::toShortString() const
{
    return toShortString(*this);
}

std::string Version::toLongString() const
{
    return toLongString(*this);
}

static Err __version_parser(char const * begin, char const * end,
                            std::uint32_t & result, std::size_t & written)
{
    if (begin == end) {
        result = 0;
        written = 0;
        return E_SUCCESS;
    }

    using namespace libtbag::string;
    auto itr = begin;
    for (; itr != end; ++itr) {
        if (*itr == Version::POINT_CHAR) {
            result = toValue<std::uint32_t>(std::string(begin, itr));
            written = std::distance(begin, itr)+1;
            return E_SUCCESS;
        }
        if (!std::isdigit(*itr)) {
            // Don't use the E_ILLARGS error (Reason: duplicate return value)
            return E_PARSING;
        }
    }

    result = toValue<std::uint32_t>(std::string(begin, itr));
    written = std::distance(begin, itr);
    return E_SUCCESS;
}

static void __version_parser_last(char const * begin, char const * end,
                                  std::uint32_t & result, std::size_t & written)
{
    if (begin == end) {
        result = 0;
        written = 0;
        return;
    }

    using namespace libtbag::string;
    auto itr = begin;
    for (; itr != end; ++itr) {
        if (!std::isdigit(*itr)) {
            break;
        }
    }

    result = toValue<std::uint32_t>(std::string(begin, itr));
    written = std::distance(begin, itr);
}

Err Version::fromString(std::string const & version, Version & result)
{
    if (version.empty()) {
        return E_ILLARGS;
    }

    using namespace libtbag::string;
    auto const src = trim(version);
    auto const size = src.size();
    auto const begin = src.c_str();
    auto const end = begin + size;

    std::size_t i = 0;
    std::size_t written;
    Err code;

    std::uint32_t major_version;
    code = __version_parser(begin+i, end, major_version, written);
    if (isFailure(code)) {
        return code;
    }

    std::uint32_t minor_version;
    i += written;
    code = __version_parser(begin+i, end, minor_version, written);
    if (isFailure(code)) {
        return code;
    }

    std::uint32_t patch_version;
    i += written;
    __version_parser_last(begin+i, end, patch_version, written);

    std::string tweak_version;
    i += written;
    if (i < size) {
        tweak_version = std::string(begin+i, end);
    }

    result = Version(major_version, minor_version, patch_version, tweak_version);
    return E_SUCCESS;
}

std::string Version::toShortString(Version const & version)
{
    auto const MAJOR = version.getMajor();
    auto const MINOR = version.getMinor();
    auto const PATCH = version.getPatch();

    if (PATCH) {
        std::stringstream ss;
        ss << MAJOR << POINT_CHAR << MINOR << POINT_CHAR << PATCH;
        return ss.str();
    }

    assert(PATCH == 0);
    if (MINOR) {
        std::stringstream ss;
        ss << MAJOR << POINT_CHAR << MINOR;
        return ss.str();
    }

    assert(PATCH == 0);
    return std::to_string(MAJOR);
}

std::string Version::toLongString(Version const & version)
{
    std::stringstream ss;
    ss << version.getMajor()
       << POINT_CHAR << version.getMinor()
       << POINT_CHAR << version.getPatch()
       << POINT_CHAR << version.getTweak();
    return ss.str();
}

// --------------------
// Utilities functions.
// --------------------

Version getTbagVersion() TBAG_NOEXCEPT
{
    return Version(LIBTBAG_VERSION_MAJOR, LIBTBAG_VERSION_MINOR, LIBTBAG_VERSION_PATCH, LIBTBAG_VERSION_TWEAK);
}

Version getTbagPacketVersion() TBAG_NOEXCEPT
{
    return Version(LIBTBAG_VERSION_PACKET_MAJOR, LIBTBAG_VERSION_PACKET_MINOR);
}

Version getTbagReleaseVersion() TBAG_NOEXCEPT
{
    return Version(LIBTBAG_VERSION_RELEASE);
}

std::string getFullVersionString()
{
    using namespace libtbag::string;
    return fformat("{}.{}.{}-{}.{}-{}",
                   LIBTBAG_VERSION_MAJOR, LIBTBAG_VERSION_MINOR, LIBTBAG_VERSION_PATCH,
                   LIBTBAG_VERSION_PACKET_MAJOR, LIBTBAG_VERSION_PACKET_MINOR,
                   LIBTBAG_VERSION_RELEASE);
}

} // namespace util

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

