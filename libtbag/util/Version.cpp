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

Version::Version() TBAG_NOEXCEPT : _major(0), _minor(0), _patch(0)
{
    // EMPTY.
}

Version::Version(uint32_t major, uint32_t minor, uint32_t patch) TBAG_NOEXCEPT
        : _major(major), _minor(minor), _patch(patch)
{
    // EMPTY.
}

Version::Version(Version const & obj) TBAG_NOEXCEPT
{
    (*this) = obj;
}

Version::Version(Version && obj) TBAG_NOEXCEPT
{
    (*this) = std::move(obj);
}

Version::~Version()
{
    // EMPTY.
}

Version & Version::operator =(Version const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _major = obj._major;
        _minor = obj._minor;
        _patch = obj._patch;
    }
    return *this;
}

Version & Version::operator =(Version && obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        swap(obj);
    }
    return *this;
}

void Version::clear() TBAG_NOEXCEPT
{
    _major = 0;
    _minor = 0;
    _patch = 0;
}

void Version::swap(Version & obj) TBAG_NOEXCEPT
{
    algorithm::swapWithPod(_major, obj._major);
    algorithm::swapWithPod(_minor, obj._minor);
    algorithm::swapWithPod(_patch, obj._patch);
}

Err Version::fromString(std::string const & version)
{
    return fromString(version, *this);
}

std::string Version::toString() const
{
    return toString(*this);
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
    } catch (std::invalid_argument & e) {
        return Err::E_PARING; // Don't use the Err::E_ILLARGS error (Reason: duplicate return value).
    } catch (std::out_of_range & e) {
        return Err::E_OORANGE;
    } catch (...) {
        return Err::E_UNKNOWN;
    }
    return Err::E_SUCCESS;
}

std::string Version::toString(Version const & version)
{
    std::stringstream ss;
    ss << version.getMajor() << POINT_STR << version.getMinor() << POINT_STR << version.getPatch();
    return ss.str();
}

// --------------------
// Utilities functions.
// --------------------

Version getTbagVersion() TBAG_NOEXCEPT
{
    return Version(LIBTBAG_VERSION_MAJOR, LIBTBAG_VERSION_MINOR, LIBTBAG_VERSION_PATCH);
}

} // namespace util

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

