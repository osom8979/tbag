/**
 * @file   PoolAllocator.hpp
 * @brief  PoolAllocator class prototype.
 * @author zer0
 * @date   2017-04-15
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_POOLALLOCATOR_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_POOLALLOCATOR_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/memory/Allocator.hpp>
#include <libtbag/memory/pool/PoolInterface.hpp>
#include <libtbag/memory/pool/BasePool.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace memory {

/**
 * PoolAllocator class prototype.
 *
 * @author zer0
 * @date   2017-04-15
 */
template <typename Type>
struct PoolAllocator : public BaseAllocator<Type>
{
    TBAG_ALLOCATOR_BASE(PoolAllocator, Type, BaseAllocator);

    using Pool = pool::PoolInterface;

    Pool * pool;

    PoolAllocator(Pool * p) : pool(p)
    { /* EMPTY. */ }

    template <typename Up>
    PoolAllocator(PoolAllocator<Up> const & obj) TBAG_NOEXCEPT : pool(obj.pool)
    { /* EMPTY. */ }

    ~PoolAllocator()
    { /* EMPTY. */ }

    /** allocates uninitialized storage. */
    pointer allocate(size_type size, void const * hint = 0)
    {
        assert(pool != nullptr);
        assert(size > 0);
        return static_cast<pointer>(pool->allocate(size * sizeof(value_type), hint));
    }

    /** deallocates storage. */
    void deallocate(pointer ptr, size_type allocated_size)
    {
        assert(pool != nullptr);
        assert(ptr != nullptr);
        pool->deallocate(ptr, allocated_size);
    }
};

} // namespace memory

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_POOLALLOCATOR_HPP__

