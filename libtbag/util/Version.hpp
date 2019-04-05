/**
 * @file   Version.hpp
 * @brief  Version class prototype.
 * @author zer0
 * @date   2017-05-25
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UTIL_VERSION_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UTIL_VERSION_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>

#include <cstdint>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace util {

/**
 * Version class prototype.
 *
 * @author zer0
 * @date   2017-05-25
 */
class TBAG_API Version
{
public:
    TBAG_CONSTEXPR static char const * const POINT_STR = ".";

private:
    uint32_t _major;
    uint32_t _minor;
    uint32_t _patch;

private:
    std::string _tweak;

public:
    Version();
    Version(uint32_t major, uint32_t minor = 0, uint32_t patch = 0, std::string const & tweak = std::string());
    Version(std::string const & ver);
    Version(uint32_t version, uint32_t major_unit, uint32_t minor_unit, uint32_t patch_unit);
    Version(Version const & obj);
    Version(Version && obj);
    ~Version();

public:
    Version & operator =(Version const & obj);
    Version & operator =(Version && obj);

public:
    inline uint32_t getMajor() const TBAG_NOEXCEPT { return _major; }
    inline uint32_t getMinor() const TBAG_NOEXCEPT { return _minor; }
    inline uint32_t getPatch() const TBAG_NOEXCEPT { return _patch; }

    inline void setMajor(uint32_t val) TBAG_NOEXCEPT { _major = val; }
    inline void setMinor(uint32_t val) TBAG_NOEXCEPT { _minor = val; }
    inline void setPatch(uint32_t val) TBAG_NOEXCEPT { _patch = val; }

    inline void set(uint32_t major = 0, uint32_t minor = 0, uint32_t patch = 0) TBAG_NOEXCEPT
    { _major = major; _minor = minor; _patch = patch; }

public:
    inline std::string getTweak() const { return _tweak; }
    inline void setTweak(std::string const & val) { _tweak = val; }

public:
    void clear();
    void swap(Version & obj);

public:
    inline friend bool operator ==(Version const & lh, Version const & rh) TBAG_NOEXCEPT
    { return lh._major == rh._major && lh._minor == rh._minor && lh._patch == rh._patch; }
    inline friend bool operator !=(Version const & lh, Version const & rh) TBAG_NOEXCEPT
    { return !(lh == rh); }

    inline friend bool operator <(Version const & lh, Version const & rh) TBAG_NOEXCEPT
    {
        // clang-format off
        if (lh._major < rh._major) { return true; } else if (lh._major > rh._major) { return false; }
        if (lh._minor < rh._minor) { return true; } else if (lh._minor > rh._minor) { return false; }
        if (lh._patch < rh._patch) { return true; } else if (lh._patch > rh._patch) { return false; }
        // clang-format on
        return false;
    }

    inline friend bool operator >(Version const & lh, Version const & rh) TBAG_NOEXCEPT
    {
        // clang-format off
        if (lh._major > rh._major) { return true; } else if (lh._major < rh._major) { return false; }
        if (lh._minor > rh._minor) { return true; } else if (lh._minor < rh._minor) { return false; }
        if (lh._patch > rh._patch) { return true; } else if (lh._patch < rh._patch) { return false; }
        // clang-format on
        return false;
    }

    inline friend bool operator <=(Version const & lh, Version const & rh) TBAG_NOEXCEPT
    { return !(lh > rh); }

    inline friend bool operator >=(Version const & lh, Version const & rh) TBAG_NOEXCEPT
    { return !(lh < rh); }

public:
    Err fromString(std::string const & version);
    Err fromNumeric(uint32_t version, uint32_t major_unit = 10000, uint32_t minor_unit = 100, uint32_t patch_unit = 1);

    std::string      toString() const;
    std::string toShortString() const;
    std::string  toLongString() const;

public:
    static Err fromString(std::string const & version, Version & result);
    static std::string toShortString(Version const & version);
    static std::string toLongString(Version const & version);
};

// --------------------
// Utilities functions.
// --------------------

TBAG_API Version getTbagVersion() TBAG_NOEXCEPT;
TBAG_API Version getTbagPacketVersion() TBAG_NOEXCEPT;
TBAG_API Version getTbagReleaseVersion() TBAG_NOEXCEPT;

} // namespace util

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UTIL_VERSION_HPP__

