/**
 * @file   MemoryPool.hpp
 * @brief  MemoryPool class prototype.
 * @author zer0
 * @date   2017-04-16
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_MEMORYPOOL_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_MEMORYPOOL_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/memory/Pool.hpp>

#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace memory {

/**
 * MemoryPool class prototype.
 *
 * @author zer0
 * @date   2017-04-16
 */
class MemoryPool : public PoolInterface
{
private:
    void * _memory;
    std::size_t _max;
    std::size_t _cursor;

public:
    MemoryPool(std::size_t size)
    {
        assert(size > 0);
        _memory = ::libtbag::memory::allocate(size);
        _max    = size;
        _cursor = 0;
    }

    virtual ~MemoryPool()
    {
        assert(_memory != nullptr);
        _max    = 0;
        _cursor = 0;
        ::libtbag::memory::deallocate(_memory);
        _memory = nullptr;
    }

public:
    // @formatter:off
    inline void const * memory() const TBAG_NOEXCEPT
    { return _memory; }
    inline std::size_t max() const TBAG_NOEXCEPT
    { return _max; }
    inline std::size_t cursor() const TBAG_NOEXCEPT
    { return _cursor; }
    // @formatter:on

public:
    inline std::ptrdiff_t begin() const TBAG_NOEXCEPT
    { return reinterpret_cast<std::ptrdiff_t>(_memory); }
    inline std::ptrdiff_t end() const TBAG_NOEXCEPT
    { return begin() + _max; }

public:
    virtual void * allocate(std::size_t allocate_size, void const * hint) override
    {
        if (_cursor + allocate_size > _max) {
            throw std::bad_alloc();
        }
        void * result = (void*)(reinterpret_cast<std::ptrdiff_t>(_memory) + _cursor);
        _cursor += allocate_size;
        return result;
    }

    virtual void deallocate(void * pointer, std::size_t allocated_size) override
    {
        std::ptrdiff_t const CURRENT = reinterpret_cast<std::ptrdiff_t>(pointer);
        assert(begin() <= COMPARE_AND(CURRENT) < end());
    }
};

} // namespace memory

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_MEMORYPOOL_HPP__

