/**
 * @file   Pool.hpp
 * @brief  Pool class prototype.
 * @author zer0
 * @date   2017-04-14
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_POOL_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_POOL_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/memory/Allocator.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace memory {

struct PoolInterface
{
    virtual void * allocate(std::size_t allocate_size, void const * hint) = 0;
    virtual void deallocate(void * pointer, std::size_t allocated_size) = 0;
};

struct Pool : public PoolInterface
{
    virtual void * allocate(std::size_t allocate_size, void const * hint) override
    {
        assert(allocate_size > 0);
        ::libtbag::memory::allocate(allocate_size);
    }

    virtual void deallocate(void * pointer, std::size_t allocated_size) override
    {
        assert(pointer != nullptr);
        ::libtbag::memory::deallocate(pointer);
    }
};

} // namespace memory

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_POOL_HPP__

