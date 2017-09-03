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
public:
    TBAG_CONSTEXPR static std::size_t const NAME_MEM_SIZE   =  64;
    TBAG_CONSTEXPR static std::size_t const SOURCE_MEM_SIZE = 256;

public:
    TBAG_CONSTEXPR static std::size_t getNameMemSize() TBAG_NOEXCEPT { return NAME_MEM_SIZE; }
    TBAG_CONSTEXPR static std::size_t getSourceMemSize() TBAG_NOEXCEPT { return SOURCE_MEM_SIZE; }

private:
    void const * _addr;

public:
    char name[NAME_MEM_SIZE + 1];
    char source[SOURCE_MEM_SIZE + 1];
    int  line;
    int  index;

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
    inline std::size_t toHash() TBAG_NOEXCEPT
    { return reinterpret_cast<std::size_t>(_addr); }
};

inline bool operator< (StFrame const & lh, StFrame const & rh) TBAG_NOEXCEPT { return lh.address() < rh.address(); }
inline bool operator> (StFrame const & lh, StFrame const & rh) TBAG_NOEXCEPT { return rh < lh; }
inline bool operator<=(StFrame const & lh, StFrame const & rh) TBAG_NOEXCEPT { return !(lh > rh); }
inline bool operator>=(StFrame const & lh, StFrame const & rh) TBAG_NOEXCEPT { return !(lh < rh); }
inline bool operator==(StFrame const & lh, StFrame const & rh) TBAG_NOEXCEPT { return lh.address() == rh.address(); }
inline bool operator!=(StFrame const & lh, StFrame const & rh) TBAG_NOEXCEPT { return !(lh == rh); }

} // namespace st
} // namespace debug

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_DEBUG_ST_STFRAME_HPP__

