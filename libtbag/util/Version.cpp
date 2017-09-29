/**
 * @file   Version.cpp
 * @brief  Version class implementation.
 * @author zer0
 * @date   2017-05-25
 */

#include <libtbag/util/Version.hpp>
#include <libtbag/algorithm/Swap.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/log/Log.hpp>

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
    algorithm::swapWithPod(_major, obj._major);
    algorithm::swapWithPod(_minor, obj._minor);
    algorithm::swapWithPod(_patch, obj._patch);
    _tweak.swap(obj._tweak);
}

Err Version::fromString(std::string const & version)
{
    return fromString(version, *this);
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

Err Version::fromString(std::string const & version, Version & result)
{
    if (version.empty()) {
        return Err::E_ILLARGS;
    }

    auto tokens = string::splitTokens(version, std::string(POINT_STR));
    auto size = tokens.size();

    try {
        // @formatter:off
        if (size >= 1) { result.setMajor(static_cast<uint32_t>(std::stoul(tokens[0]))); }
        if (size >= 2) { result.setMinor(static_cast<uint32_t>(std::stoul(tokens[1]))); }
        if (size >= 3) { result.setPatch(static_cast<uint32_t>(std::stoul(tokens[2]))); }
        // @formatter:on
        if (size >= 4) {
            result.setTweak(version.substr(tokens[0].size() + tokens[1].size() + tokens[2].size() + 3/*POINT('.') COUNT*/));
        }
    } catch (std::invalid_argument & e) {
        return Err::E_PARSING; // Don't use the Err::E_ILLARGS error (Reason: duplicate return value).
    } catch (std::out_of_range & e) {
        return Err::E_OORANGE;
    } catch (...) {
        return Err::E_UNKNOWN;
    }
    return Err::E_SUCCESS;
}

std::string Version::toShortString(Version const & version)
{
    std::stringstream ss;
    ss << version.getMajor();
    if (version.getMinor() != 0) {
        ss << POINT_STR << version.getMinor();
    }
    if (version.getPatch() != 0) {
        ss << POINT_STR << version.getPatch();
    }
    return ss.str();
}

std::string Version::toLongString(Version const & version)
{
    std::stringstream ss;
    ss << version.getMajor()
       << POINT_STR << version.getMinor()
       << POINT_STR << version.getPatch()
       << POINT_STR << version.getTweak();
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

} // namespace util

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

