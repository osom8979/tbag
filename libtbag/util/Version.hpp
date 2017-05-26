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

public:
    Version() TBAG_NOEXCEPT;
    Version(uint32_t major, uint32_t minor = 0, uint32_t patch = 0) TBAG_NOEXCEPT;
    Version(Version const & obj) TBAG_NOEXCEPT;
    Version(Version && obj) TBAG_NOEXCEPT;
    ~Version();

public:
    Version & operator =(Version const & obj) TBAG_NOEXCEPT;
    Version & operator =(Version && obj) TBAG_NOEXCEPT;

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
    void clear() TBAG_NOEXCEPT;
    void swap(Version & obj) TBAG_NOEXCEPT;

public:
    inline friend bool operator ==(Version const & lh, Version const & rh) TBAG_NOEXCEPT
    { return lh._major == rh._major && lh._minor == rh._minor && lh._patch == rh._patch; }
    inline friend bool operator !=(Version const & lh, Version const & rh) TBAG_NOEXCEPT
    { return !(lh == rh); }

    inline friend bool operator <(Version const & lh, Version const & rh) TBAG_NOEXCEPT
    {
#ifndef _TBAG_VERSION_LT_SINGLE
#define _TBAG_VERSION_LT_SINGLE(lh, rh) \
        if (lh < rh) { return true; } else if (lh > rh) { return false; }
#endif
        _TBAG_VERSION_LT_SINGLE(lh._major, rh._major);
        _TBAG_VERSION_LT_SINGLE(lh._minor, rh._minor);
        _TBAG_VERSION_LT_SINGLE(lh._patch, rh._patch);
#undef _TBAG_VERSION_LT_SINGLE
        return false;
    }

    inline friend bool operator >(Version const & lh, Version const & rh) TBAG_NOEXCEPT
    {
#ifndef _TBAG_VERSION_GT_SINGLE
#define _TBAG_VERSION_GT_SINGLE(lh, rh) \
    if (lh > rh) { return true; } else if (lh < rh) { return false; }
#endif
        _TBAG_VERSION_GT_SINGLE(lh._major, rh._major);
        _TBAG_VERSION_GT_SINGLE(lh._minor, rh._minor);
        _TBAG_VERSION_GT_SINGLE(lh._patch, rh._patch);
#undef _TBAG_VERSION_GT_SINGLE
        return false;
    }

    inline friend bool operator <=(Version const & lh, Version const & rh) TBAG_NOEXCEPT
    { return lh == rh || lh < rh; }

    inline friend bool operator >=(Version const & lh, Version const & rh) TBAG_NOEXCEPT
    { return lh == rh || lh > rh; }

public:
    Err fromString(std::string const & version);
    std::string toString();

public:
    static Err fromString(std::string const & version, Version & result);
    static std::string toString(Version const & version);
};

// --------------------
// Utilities functions.
// --------------------

TBAG_API Version getTbagVersion() TBAG_NOEXCEPT;

} // namespace util

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UTIL_VERSION_HPP__

