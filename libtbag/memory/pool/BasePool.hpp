/**
 * @file   BasePool.hpp
 * @brief  BasePool class prototype.
 * @author zer0
 * @date   2017-06-10
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_POOL_BASEPOOL_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_POOL_BASEPOOL_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/memory/pool/PoolInterface.hpp>
#include <libtbag/memory/alloc/TraceNew.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace memory {
namespace pool   {

/**
 * BasePool class prototype.
 *
 * @author zer0
 * @date   2017-06-10
 */
class BasePool : public PoolInterface
{
    BasePool() { /* EMPTY.*/ }
    virtual ~BasePool() { /* EMPTY.*/ }

    virtual void * allocate(std::size_t allocate_size, void const * hint) override
    {
        assert(allocate_size > 0);
        return ::libtbag::memory::alloc::allocate(allocate_size);
    }

    virtual void deallocate(void * pointer, std::size_t allocated_size) override
    {
        assert(pointer != nullptr);
        ::libtbag::memory::alloc::deallocate(pointer);
    }
};

} // namespace pool
} // namespace memory

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_POOL_BASEPOOL_HPP__

