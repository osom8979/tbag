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

#include <cstdlib>
#include <iosfwd>
#include <string>
#include <array>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace debug {
namespace st    {

TBAG_CONSTEXPR int const DEFAULT_MAX_DEPTH_OF_STACKTRACE = 64;

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

public:
    void const * addr;
    char name[NAME_MEM_SIZE];
    char source[SOURCE_MEM_SIZE];
    int  offset;
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
    void clearName();
    void clearSource();

public:
    inline void const * address() const TBAG_NOEXCEPT { return addr; }
    inline std::ptrdiff_t diff() const TBAG_NOEXCEPT { return reinterpret_cast<std::ptrdiff_t>(addr); }
    inline bool empty() const TBAG_NOEXCEPT { return addr == nullptr; }
    inline bool operator!() const TBAG_NOEXCEPT { return addr == nullptr; }
    inline operator bool() const TBAG_NOEXCEPT { return addr != nullptr; }

public:
    inline std::size_t toHash() TBAG_NOEXCEPT
    { return reinterpret_cast<std::size_t>(addr); }

public:
    void demangleAssign(char const * symbol, std::size_t symbol_size);

public:
    std::string toAddressString() const;
    std::string toString() const;

public:
    // Example: ./tester(_ZN7testing8UnitTest3RunEv+0xb7) [0x7ee1eb]
    TBAG_CONSTEXPR static int const SYMBOL_STRINGS_GCC_COLUMN_MODULE  = 0;
    TBAG_CONSTEXPR static int const SYMBOL_STRINGS_GCC_COLUMN_SYMBOL  = 1;
    TBAG_CONSTEXPR static int const SYMBOL_STRINGS_GCC_COLUMN_OFFSET  = 2;
    TBAG_CONSTEXPR static int const SYMBOL_STRINGS_GCC_COLUMN_ADDRESS = 3;

    // Example: 0 tester 0x0000000105f84708 _ZN7testing8UnitTest3RunEv + 408
    TBAG_CONSTEXPR static int const SYMBOL_STRINGS_CLANG_COLUMN_INDEX   = 0;
    TBAG_CONSTEXPR static int const SYMBOL_STRINGS_CLANG_COLUMN_MODULE  = 1;
    TBAG_CONSTEXPR static int const SYMBOL_STRINGS_CLANG_COLUMN_ADDRESS = 2;
    TBAG_CONSTEXPR static int const SYMBOL_STRINGS_CLANG_COLUMN_SYMBOL  = 3;
    TBAG_CONSTEXPR static int const SYMBOL_STRINGS_CLANG_COLUMN_OFFSET  = 4;

public:
    // clang-format off
    static StFrame parseGccSymbolize  (char const * symbols_format, void const * addr = nullptr);
    static StFrame parseClangSymbolize(char const * symbols_format, void const * addr = nullptr);
    static StFrame parseSymbolize     (char const * symbols_format, void const * addr = nullptr);
    // clang-format on
};

inline bool operator< (StFrame const & lh, StFrame const & rh) TBAG_NOEXCEPT { return lh.address() < rh.address(); }
inline bool operator> (StFrame const & lh, StFrame const & rh) TBAG_NOEXCEPT { return rh < lh; }
inline bool operator<=(StFrame const & lh, StFrame const & rh) TBAG_NOEXCEPT { return !(lh > rh); }
inline bool operator>=(StFrame const & lh, StFrame const & rh) TBAG_NOEXCEPT { return !(lh < rh); }
inline bool operator==(StFrame const & lh, StFrame const & rh) TBAG_NOEXCEPT { return lh.address() == rh.address(); }
inline bool operator!=(StFrame const & lh, StFrame const & rh) TBAG_NOEXCEPT { return !(lh == rh); }

using StFrameArray = std::array<StFrame, DEFAULT_MAX_DEPTH_OF_STACKTRACE>;

} // namespace st
} // namespace debug

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_DEBUG_ST_STFRAME_HPP__

