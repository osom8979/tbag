/**
 * @file   MultiMemoryPool.hpp
 * @brief  MultiMemoryPool class prototype.
 * @author zer0
 * @date   2017-06-10
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_POOL_MULTIMEMORYPOOL_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_POOL_MULTIMEMORYPOOL_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/memory/pool/MemoryPool.hpp>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace memory {
namespace pool   {

/**
 * MultiMemoryPool class prototype.
 *
 * @author zer0
 * @date   2017-06-10
 */
class MultiMemoryPool : public PoolInterface
{
public:
    TBAG_CONSTEXPR static std::size_t const DEFAULT_SMALL_BYTE_SIZE  =   32;
    TBAG_CONSTEXPR static std::size_t const DEFAULT_NORMAL_BYTE_SIZE =  512;
    TBAG_CONSTEXPR static std::size_t const DEFAULT_LARGE_BYTE_SIZE  = 2048;
    TBAG_CONSTEXPR static std::size_t const DEFAULT_OBJECT_COUNT     =  128;

public:
    class Fragment : private Noncopyable
    {
    private:
        MemoryPool::Fragment  _small_fragment;
        MemoryPool::Fragment _normal_fragment;
        MemoryPool::Fragment  _large_fragment;

    public:
        Fragment(MultiMemoryPool & pool) TBAG_NOEXCEPT
                : _small_fragment(pool._small), _normal_fragment(pool._normal), _large_fragment(pool._large)
        {
            // EMPTY.
        }

        virtual ~Fragment()
        {
            // EMPTY.
        }
    };

private:
    std::size_t  _small_step;
    std::size_t _normal_step;
    std::size_t  _large_step;

    MemoryPool  _small;
    MemoryPool _normal;
    MemoryPool  _large;

public:
    MultiMemoryPool(std::size_t   small_step = DEFAULT_SMALL_BYTE_SIZE,
                    std::size_t  normal_step = DEFAULT_NORMAL_BYTE_SIZE,
                    std::size_t   large_step = DEFAULT_LARGE_BYTE_SIZE,
                    std::size_t object_count = DEFAULT_OBJECT_COUNT)
            : MultiMemoryPool(small_step, normal_step, large_step,
                              small_step  * object_count,
                              normal_step * object_count,
                              large_step  * object_count)
    {
        // EMPTY.
    }

    MultiMemoryPool(std::size_t small_step, std::size_t normal_step, std::size_t large_step,
                    std::size_t small_size, std::size_t normal_size, std::size_t large_size)
            : _small_step(small_step), _normal_step(normal_step), _large_step(large_step),
              _small(small_size), _normal(normal_size), _large(large_size)
    {
        // EMPTY.
    }

    virtual ~MultiMemoryPool()
    {
        // EMPTY.
    }

public:
    MemoryPool       & atSmall ()       TBAG_NOEXCEPT { return _small;  }
    MemoryPool const & atSmall () const TBAG_NOEXCEPT { return _small;  }
    MemoryPool       & atNormal()       TBAG_NOEXCEPT { return _normal; }
    MemoryPool const & atNormal() const TBAG_NOEXCEPT { return _normal; }
    MemoryPool       & atLarge ()       TBAG_NOEXCEPT { return _large;  }
    MemoryPool const & atLarge () const TBAG_NOEXCEPT { return _large;  }

public:
    virtual void * allocate(std::size_t allocate_size, void const * hint) override
    {
        if (allocate_size <= _small_step) {
            return _small.allocate(allocate_size, hint);
        } else if (allocate_size <= _normal_step) {
            return _normal.allocate(allocate_size, hint);
        } else {
            return _large.allocate(allocate_size, hint);
        }
    }

    virtual void deallocate(void * pointer, std::size_t allocated_size) override
    {
        if (allocated_size <= _small_step) {
            assert(_small.exists(pointer));
            _small.deallocate(pointer, allocated_size);
        } else if (allocated_size <= _normal_step) {
            assert(_normal.exists(pointer));
            _normal.deallocate(pointer, allocated_size);
        } else {
            assert(_large.exists(pointer));
            _large.deallocate(pointer, allocated_size);
        }
    }
};

using MmpFragment = MultiMemoryPool::Fragment;

} // namespace pool
} // namespace memory

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_POOL_MULTIMEMORYPOOL_HPP__

