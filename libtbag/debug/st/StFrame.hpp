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
#include <cstdint>

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
    TBAG_CONSTEXPR static std::size_t const NAME_MEM_SIZE   = 2048;
    TBAG_CONSTEXPR static std::size_t const SOURCE_MEM_SIZE = 2048;

public:
    TBAG_CONSTEXPR static std::size_t getNameMemSize() TBAG_NOEXCEPT { return NAME_MEM_SIZE; }
    TBAG_CONSTEXPR static std::size_t getSourceMemSize() TBAG_NOEXCEPT { return SOURCE_MEM_SIZE; }

public:
    /**
     * x86 and x64 compatible type.
     *
     * @warning
     *  The <code>void*</code> or <code>size_t</code> type is 4 bytes on x86. @n
     *  You must use the <code>uint64_t</code> type to read address information of 8 bytes (x64).
     */
    std::uint64_t addr;
    char name[NAME_MEM_SIZE];
    char source[SOURCE_MEM_SIZE];
    int offset;
    int index;

public:
    StFrame(std::uint64_t addr = 0) TBAG_NOEXCEPT;
    explicit StFrame(void const * addr) TBAG_NOEXCEPT;
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
    inline void const * address() const TBAG_NOEXCEPT
    { return (void const *)(addr); }

    inline bool empty() const TBAG_NOEXCEPT
    { return addr == 0; }

public:
    inline bool operator!() const TBAG_NOEXCEPT
    { return addr == 0; }

    inline operator bool() const TBAG_NOEXCEPT
    { return addr != 0; }

public:
    inline std::uint64_t diff() const TBAG_NOEXCEPT
    { return addr; }

    inline std::uint64_t toHash() const TBAG_NOEXCEPT
    { return addr; }

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
    static StFrame parseRawGccSymbolize  (char const * symbols_format, std::uint64_t addr = 0);
    static StFrame parseRawClangSymbolize(char const * symbols_format, std::uint64_t addr = 0);
    static StFrame parseRawSymbolize     (char const * symbols_format, std::uint64_t addr = 0);
    // clang-format on

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

