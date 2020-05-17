/**
 * @file   Byte.hpp
 * @brief  Byte class prototype.
 * @author zer0
 * @date   2020-05-17
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UTIL_BYTE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UTIL_BYTE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>

#include <string>
#include <ostream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace util {

/**
 * Byte class prototype.
 *
 * @author zer0
 * @date   2020-05-17
 */
class TBAG_API Byte
{
private:
    std::size_t _byte;

public:
    Byte() TBAG_NOEXCEPT;
    Byte(std::size_t byte) TBAG_NOEXCEPT;
    Byte(std::string const & text);
    Byte(Byte const & obj) TBAG_NOEXCEPT;
    Byte(Byte && obj) TBAG_NOEXCEPT;
    ~Byte();

public:
    Byte & operator =(std::size_t byte) TBAG_NOEXCEPT;
    Byte & operator =(std::string const & text);
    Byte & operator =(Byte const & obj) TBAG_NOEXCEPT;
    Byte & operator =(Byte && obj) TBAG_NOEXCEPT;

public:
    void swap(Byte & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(Byte & lh, Byte & rh) TBAG_NOEXCEPT { lh.swap(rh); }

public:
    bool operator < (Byte const & obj) const TBAG_NOEXCEPT { return _byte <  obj._byte; }
    bool operator <=(Byte const & obj) const TBAG_NOEXCEPT { return _byte <= obj._byte; }
    bool operator > (Byte const & obj) const TBAG_NOEXCEPT { return _byte >  obj._byte; }
    bool operator >=(Byte const & obj) const TBAG_NOEXCEPT { return _byte >= obj._byte; }
    bool operator ==(Byte const & obj) const TBAG_NOEXCEPT { return _byte == obj._byte; }
    bool operator !=(Byte const & obj) const TBAG_NOEXCEPT { return _byte != obj._byte; }

public:
    template <typename T>
    friend bool operator < (T const & lh, Byte const & rh) TBAG_NOEXCEPT { return lh <  rh._byte; }
    template <typename T>
    friend bool operator <=(T const & lh, Byte const & rh) TBAG_NOEXCEPT { return lh <= rh._byte; }
    template <typename T>
    friend bool operator > (T const & lh, Byte const & rh) TBAG_NOEXCEPT { return lh >  rh._byte; }
    template <typename T>
    friend bool operator >=(T const & lh, Byte const & rh) TBAG_NOEXCEPT { return lh >= rh._byte; }
    template <typename T>
    friend bool operator ==(T const & lh, Byte const & rh) TBAG_NOEXCEPT { return lh == rh._byte; }
    template <typename T>
    friend bool operator !=(T const & lh, Byte const & rh) TBAG_NOEXCEPT { return lh != rh._byte; }

public:
    std::size_t toByte() const TBAG_NOEXCEPT;
    std::size_t toKiloByte() const TBAG_NOEXCEPT;
    std::size_t toMegaByte() const TBAG_NOEXCEPT;
    std::size_t toGigaByte() const TBAG_NOEXCEPT;
    std::size_t toTeraByte() const TBAG_NOEXCEPT;
    std::size_t toPetaByte() const TBAG_NOEXCEPT;
    std::size_t toExaByte() const TBAG_NOEXCEPT;

public:
    std::string toUpperByteText() const;

public:
    Err fromString(std::string const & text);
    std::string toString() const;

public:
    template <class CharT, class TraitsT>
    friend std::basic_ostream<CharT, TraitsT> & operator <<(std::basic_ostream<CharT, TraitsT> & os,
                                                            Byte const & byte)
    {
        return os << byte.toString();
    }
};

} // namespace util

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UTIL_BYTE_HPP__

