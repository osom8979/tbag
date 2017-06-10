/**
 * @file   PoolInterface.hpp
 * @brief  PoolInterface class prototype.
 * @author zer0
 * @date   2017-06-10
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_POOL_POOLINTERFACE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_POOL_POOLINTERFACE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <cstddef>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace memory {
namespace pool   {

struct PoolInterface
{
    PoolInterface() { /* EMPTY.*/ }
    virtual ~PoolInterface() { /* EMPTY.*/ }

    virtual void * allocate(std::size_t allocate_size, void const * hint) = 0;
    virtual void deallocate(void * pointer, std::size_t allocated_size) = 0;
};

} // namespace pool
} // namespace memory

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_POOL_POOLINTERFACE_HPP__

