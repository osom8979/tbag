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
#include <libtbag/algorithm/Swap.hpp>

#include <thread>
#include <atomic>
#include <array>

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
 *
 * @remarks
 * Memory (4byte):
 *  @code
 *    0                   1                   2                   3
 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *   +-+-------------+-+-------------+-+-------------+---------------+
 *   |F|    Input    |F|   Output    |F|    Ready    |               |
 *   |L|    Value    |L|    Value    |L|    Value    |    PADDING    |
 *   |A|   (0~127)   |A|   (0~127)   |A|   (0~127)   |     BYTE      |
 *   |G|             |G|             |G|             |               |
 *   +-+-------------+-+-------------+-+-------------+---------------+
 *  @endcode
 * Thread:
 *  @code
 *   ~~ Thread 01 ~~> ${input} ~~> ...
 *                        |
 *                        * <-- compare and swap
 *                        |
 *                    ${ready}
 *                        |
 *                        * <-- compare and swap
 *                        |
 *   ~~ Thread 02 ~~> ${output} ~~> ...
 *  @endcode
 */
class TBAG_API TripleIoIndex : private Noncopyable
{
public:
    union Info {
        uint32_t value;
        uint8_t  array[sizeof(uint32_t)];
    };

    static_assert(sizeof(Info) == 4, "Why not?");

    using AtomicInfo = std::atomic<Info>;

public:
    TBAG_CONSTEXPR static std::size_t const SIZE = 3;

    TBAG_CONSTEXPR static uint8_t const UPDATE_BIT = 0x80;
    TBAG_CONSTEXPR static uint8_t const  INDEX_BIT = 0x7F;

    TBAG_CONSTEXPR static int const   INPUT_INDEX = 0;
    TBAG_CONSTEXPR static int const  OUTPUT_INDEX = 1;
    TBAG_CONSTEXPR static int const   READY_INDEX = 2;
    TBAG_CONSTEXPR static int const PADDING_INDEX = 3;

public:
    TBAG_CONSTEXPR static std::size_t size() TBAG_NOEXCEPT { return SIZE; }

private:
    bool const COMPARE_AND_SWAP_OUTPUT_LOOP;

private:
    AtomicInfo _info;

public:
    TripleIoIndex(int  input_value = 0,
                  int output_value = 1,
                  int  ready_value = 2, bool cas_loop = true);
    ~TripleIoIndex();

public:
    static Info createInfo(int  input_value = 0, bool  input_flag = false,
                           int output_value = 1, bool output_flag = false,
                           int  ready_value = 2, bool  ready_flag = false) TBAG_NOEXCEPT;

public:
    inline static uint8_t createByte(int index, bool flag = true) TBAG_NOEXCEPT
    { return static_cast<uint8_t>((index & INDEX_BIT) | (flag ? UPDATE_BIT : 0x00)); }

    inline static bool isUpdate(uint8_t i) TBAG_NOEXCEPT
    { return static_cast<bool>(i & UPDATE_BIT); }

    inline static int getValue(uint8_t i) TBAG_NOEXCEPT
    { return static_cast<int>(i & INDEX_BIT); }

public:
    inline static void update(uint8_t & i, bool flag = true) TBAG_NOEXCEPT
    { i = createByte(getValue(i), flag); }
    inline static void update(Info & info, int index, bool flag = true) TBAG_NOEXCEPT
    { update(info.array[index], flag); }

    inline static void updateInput(Info & info, bool flag = true) TBAG_NOEXCEPT
    { update(info, INPUT_INDEX, flag); }
    inline static void updateOutput(Info & info, bool flag = true) TBAG_NOEXCEPT
    { update(info, OUTPUT_INDEX, flag); }

public:
    inline static void swap(Info & info, int lh, int rh) TBAG_NOEXCEPT
    { algorithm::swapWithPod(info.array[lh], info.array[rh]); }

    inline static void swapInputAndReady(Info & info) TBAG_NOEXCEPT
    { swap(info, INPUT_INDEX, READY_INDEX); }
    inline static void swapOutputAndReady(Info & info) TBAG_NOEXCEPT
    { swap(info, OUTPUT_INDEX, READY_INDEX); }

public:
    inline static uint8_t  getInputByte(Info const & info) TBAG_NOEXCEPT { return info.array[ INPUT_INDEX]; }
    inline static uint8_t getOutputByte(Info const & info) TBAG_NOEXCEPT { return info.array[OUTPUT_INDEX]; }
    inline static uint8_t  getReadyByte(Info const & info) TBAG_NOEXCEPT { return info.array[ READY_INDEX]; }

public:
    inline Info getInfo() const TBAG_NOEXCEPT_SP_OP(_info.load()) { return _info.load(); }

public:
    Info nextInput();
    Info nextOutput();

    int nextInputValue();
    int nextOutputValue();
};

/**
 * TripleIoBuffer class prototype.
 *
 * @author zer0
 * @date   2017-07-17
 */
template <typename BufferType>
class TripleIoBuffer : private Noncopyable
{
public:
    using Buffer = BufferType;
    using BufferArray = std::array<Buffer, TripleIoIndex::SIZE>;

public:
    using value_type             = Buffer;
    using reference              = value_type&;
    using const_reference        = const value_type&;
    using iterator               = value_type*;
    using const_iterator         = const value_type*;
    using pointer                = value_type*;
    using const_pointer          = const value_type*;
    using size_type              = std::size_t;
    using difference_type        = std::ptrdiff_t;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
    TripleIoIndex _index;
    BufferArray _buffers;

public:
    TripleIoBuffer(bool cas_loop = true) : _index(0, 1, 2, cas_loop), _buffers()
    { /* EMPTY. */ }
    ~TripleIoBuffer()
    { /* EMPTY. */ }

// Capacity methods.
public:
    TBAG_CONSTEXPR static size_type  size() TBAG_NOEXCEPT { return TripleIoIndex::SIZE; }
    TBAG_CONSTEXPR static      bool empty() TBAG_NOEXCEPT { return false; }

// Iterators methods.
public:
    inline       iterator begin()       TBAG_NOEXCEPT { return _buffers.begin(); }
    inline const_iterator begin() const TBAG_NOEXCEPT { return _buffers.begin(); }
    inline       iterator   end()       TBAG_NOEXCEPT { return _buffers.end(); }
    inline const_iterator   end() const TBAG_NOEXCEPT { return _buffers.end(); }

    inline       reverse_iterator rbegin()       TBAG_NOEXCEPT { return _buffers.rbegin(); }
    inline const_reverse_iterator rbegin() const TBAG_NOEXCEPT { return _buffers.rbegin(); }
    inline       reverse_iterator   rend()       TBAG_NOEXCEPT { return _buffers.rend(); }
    inline const_reverse_iterator   rend() const TBAG_NOEXCEPT { return _buffers.rend(); }

    inline         const_iterator  cbegin() const TBAG_NOEXCEPT { return _buffers.cbegin (); }
    inline         const_iterator    cend() const TBAG_NOEXCEPT { return _buffers.cend   (); }
    inline const_reverse_iterator crbegin() const TBAG_NOEXCEPT { return _buffers.crbegin(); }
    inline const_reverse_iterator   crend() const TBAG_NOEXCEPT { return _buffers.crend  (); }

// I/O Methods.
public:
    reference atNextInput () TBAG_NOEXCEPT { return _buffers[_index.nextInputValue ()]; }
    reference atNextOutput() TBAG_NOEXCEPT { return _buffers[_index.nextOutputValue()]; }
};

} // namespace lockfree

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOCKFREE_TRIPLEIOINDEX_HPP__

