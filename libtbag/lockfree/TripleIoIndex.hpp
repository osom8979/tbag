/**
 * @file   TripleIoIndex.hpp
 * @brief  TripleIoIndex class prototype.
 * @author zer0
 * @date   2017-07-14
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOCKFREE_TRIPLEIOINDEX_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOCKFREE_TRIPLEIOINDEX_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <thread>
#include <atomic>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace lockfree {

/**
 * TripleIoIndex class prototype.
 *
 * @author zer0
 * @date   2017-07-14
 *
 * @see <https://en.wikipedia.org/wiki/ABA_problem>
 */
class TBAG_API TripleIoIndex : private Noncopyable
{
public:
    using Info = uint8_t;

public:
    TBAG_CONSTEXPR static Info const UPDATE_BIT = 0xF0;
    TBAG_CONSTEXPR static Info const  INDEX_BIT = 0x0F;

private:
    bool const COMPARE_AND_SWAP_OUTPUT_LOOP;

private:
    std::atomic<Info> _input;
    std::atomic<Info> _ready;
    std::atomic<Info> _output;

public:
    TripleIoIndex(bool cas_loop = true);
    virtual ~TripleIoIndex();

public:
    inline static void update(Info & i, bool update = true) TBAG_NOEXCEPT
    { i = static_cast<Info>((i & INDEX_BIT) | (update ? UPDATE_BIT : 0x00)); }
    inline static bool isUpdate(Info i) TBAG_NOEXCEPT
    { return (i & UPDATE_BIT); }
    inline static int getIndex(Info i) TBAG_NOEXCEPT
    { return static_cast<int>(i & INDEX_BIT); }
    inline static Info getInfo(int index, bool update = true) TBAG_NOEXCEPT
    { return static_cast<Info>((index & INDEX_BIT) | (update ? UPDATE_BIT : 0x00)); }

public:
    inline int  getInputIndex() const TBAG_NOEXCEPT_SP_OP( _input.load()) { return getIndex( _input.load()); }
    inline int getOutputIndex() const TBAG_NOEXCEPT_SP_OP(_output.load()) { return getIndex(_output.load()); }

public:
    int updateNextInput();
    int updateNextOutput();
};

} // namespace lockfree

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOCKFREE_TRIPLEIOINDEX_HPP__

