/**
 * @file   StFrame.hpp
 * @brief  StFrame class prototype.
 * @author zer0
 * @date   2017-05-28
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_DEBUG_ST_STFRAME_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_DEBUG_ST_STFRAME_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <cstdlib>
#include <iosfwd>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace debug {
namespace st    {

/**
 * StFrame class prototype.
 *
 * @author zer0
 * @date   2017-05-28
 */
class TBAG_API StFrame
{
private:
    void const * _addr;

public:
    StFrame() TBAG_NOEXCEPT;
    StFrame(void const * addr) TBAG_NOEXCEPT;
    StFrame(StFrame const & obj) TBAG_NOEXCEPT;
    StFrame(StFrame && obj) TBAG_NOEXCEPT;
    ~StFrame();

public:
    StFrame & operator =(StFrame const & obj) TBAG_NOEXCEPT;
    StFrame & operator =(StFrame && obj) TBAG_NOEXCEPT;

public:
    inline void const * address() const TBAG_NOEXCEPT { return _addr; }
    inline bool empty() const TBAG_NOEXCEPT { return _addr == nullptr; }
    inline bool operator!() const TBAG_NOEXCEPT { return _addr == nullptr; }
    inline operator bool() const TBAG_NOEXCEPT { return _addr != nullptr; }

public:
    std::string name() const;
    std::string source() const;
    std::size_t line() const;

public:
    inline std::size_t toHash() TBAG_NOEXCEPT
    { return reinterpret_cast<std::size_t>(_addr); }
};

inline bool operator< (StFrame const & lh, StFrame const & rh) TBAG_NOEXCEPT { return lh.address() < rh.address(); }
inline bool operator> (StFrame const & lh, StFrame const & rh) TBAG_NOEXCEPT { return rh < lh; }
inline bool operator<=(StFrame const & lh, StFrame const & rh) TBAG_NOEXCEPT { return !(lh > rh); }
inline bool operator>=(StFrame const & lh, StFrame const & rh) TBAG_NOEXCEPT { return !(lh < rh); }
inline bool operator==(StFrame const & lh, StFrame const & rh) TBAG_NOEXCEPT { return lh.address() == rh.address(); }
inline bool operator!=(StFrame const & lh, StFrame const & rh) TBAG_NOEXCEPT { return !(lh == rh); }

template <class CharT, class TraitsT>
std::basic_ostream<CharT, TraitsT> & operator<<(std::basic_ostream<CharT, TraitsT> & os, StFrame const & f)
{
    std::string const NAME = f.name();
    if (NAME.empty() == false) {
        os << NAME;
    } else {
        os << f.address();
    }

    std::size_t const LINE = f.line();
    if (LINE) {
        os << " at " << f.source() << ':' << LINE;
    }
    return os;
}

} // namespace st
} // namespace debug

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_DEBUG_ST_STFRAME_HPP__

