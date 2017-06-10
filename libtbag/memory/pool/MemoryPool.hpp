/**
 * @file   MemoryPool.hpp
 * @brief  MemoryPool class prototype.
 * @author zer0
 * @date   2017-06-10
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_POOL_MEMORYPOOL_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_POOL_MEMORYPOOL_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/Unit.hpp>
#include <libtbag/log/Log.hpp>

#include <libtbag/memory/pool/PoolInterface.hpp>
#include <libtbag/memory/alloc/TraceNew.hpp>

#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace memory {
namespace pool   {

/**
 * MemoryPool class prototype.
 *
 * @author zer0
 * @date   2017-06-10
 */
class MemoryPool : public PoolInterface
{
public:
    TBAG_CONSTEXPR static std::size_t const DEFAULT_MEMORY_POOL_SIZE
            = static_cast<std::size_t>(10 * MEGA_BYTE_TO_BYTE);

public:
    class Fragment : private Noncopyable
    {
    private:
        MemoryPool & _pool;
        std::size_t _cursor;

    public:
        Fragment(MemoryPool & pool) TBAG_NOEXCEPT : _pool(pool), _cursor(pool._cursor)
        {
            // EMPTY.
        }

        virtual ~Fragment()
        {
            _pool._cursor = _cursor;
        }
    };

private:
    void * _memory;
    std::size_t _max;
    std::size_t _cursor;

public:
    MemoryPool(std::size_t size = DEFAULT_MEMORY_POOL_SIZE)
    {
        assert(size > 0);
        _memory = ::libtbag::memory::alloc::allocate(size);
        _max    = size;
        _cursor = 0;
    }

    virtual ~MemoryPool()
    {
        assert(_memory != nullptr);
        _max    = 0;
        _cursor = 0;
        ::libtbag::memory::alloc::deallocate(_memory);
        _memory = nullptr;
    }

public:
    // @formatter:off
    inline void const * memory() const TBAG_NOEXCEPT { return _memory; }
    inline std::size_t     max() const TBAG_NOEXCEPT { return _max; }
    inline std::size_t    size() const TBAG_NOEXCEPT { return _cursor; }
    inline std::size_t    left() const TBAG_NOEXCEPT { return _max - _cursor; }
    // @formatter:on

public:
    inline std::ptrdiff_t begin() const TBAG_NOEXCEPT
    { return reinterpret_cast<std::ptrdiff_t>(_memory); }
    inline std::ptrdiff_t end() const TBAG_NOEXCEPT
    { return begin() + _max; }

public:
    inline bool exists(void * pointer) TBAG_NOEXCEPT
    {
        std::ptrdiff_t const CURRENT = reinterpret_cast<std::ptrdiff_t>(pointer);
        return begin() <= COMPARE_AND(CURRENT) < end();
    }

public:
    virtual void * allocate(std::size_t allocate_size, void const * hint) override
    {
        if (_cursor + allocate_size > _max) {
            throw std::bad_alloc();
        }
        void * result = (void*)(reinterpret_cast<std::ptrdiff_t>(_memory) + _cursor);
        _cursor += allocate_size;
        tDLogD("MemoryPool::allocate@{}({}) -> {}", reinterpret_cast<void*>(this), allocate_size, result);
        return result;
    }

    virtual void deallocate(void * pointer, std::size_t allocated_size) override
    {
        tDLogD("MemoryPool::deallocate@{}({}, {})", reinterpret_cast<void*>(this), pointer, allocated_size);
        std::ptrdiff_t const CURRENT = reinterpret_cast<std::ptrdiff_t>(pointer);
        assert(begin() <= COMPARE_AND(CURRENT) < end());
    }
};

using MpFragment = MemoryPool::Fragment;

} // namespace pool
} // namespace memory

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_POOL_MEMORYPOOL_HPP__

