/**
 * @file   SyncedMemory.hpp
 * @brief  SyncedMemory class prototype.
 * @author zer0
 * @date   2017-12-28
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_SYNCEDMEMORY_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_SYNCEDMEMORY_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/gpu/Gpu.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu {

/**
 * SyncedMemory class prototype.
 *
 * @author zer0
 * @date   2017-12-28
 */
class TBAG_API SyncedMemory
{
public:
    enum class SyncedHead
    {
        SH_UNINITIALIZED,
        SH_HEAD_AT_CPU,
        SH_HEAD_AT_GPU,
        SH_SYNCED,
    };

private:
    UniqueGpu   _backend;
    SyncedHead  _head;
    std::size_t _capacity;
    std::size_t _size;

private:
    bool _own_cpu;
    bool _own_gpu;
    void * _cpu;
    void * _gpu;

public:
    explicit SyncedMemory(GpuBackendType type);
    SyncedMemory(std::size_t size = 0, GpuBackendType type = GpuBackendType::GBT_CPU);
    SyncedMemory(SyncedMemory const & obj);
    SyncedMemory(SyncedMemory && obj);
    ~SyncedMemory();

public:
    SyncedMemory & operator =(SyncedMemory const & obj);
    SyncedMemory & operator =(SyncedMemory && obj);

public:
    bool cloneFrom(SyncedMemory const & obj);
    void swap(SyncedMemory & obj);

public:
    inline friend void swap(SyncedMemory & lh, SyncedMemory & rh) { lh.swap(rh); }

public:
    inline SyncedHead      head() const TBAG_NOEXCEPT { return     _head; }
    inline std::size_t capacity() const TBAG_NOEXCEPT { return _capacity; }
    inline std::size_t     size() const TBAG_NOEXCEPT { return     _size; }

public:
    inline bool isOwnCpu() const TBAG_NOEXCEPT { return _own_cpu; }
    inline bool isOwnGpu() const TBAG_NOEXCEPT { return _own_gpu; }

    inline void ownCpu(bool flag = true) TBAG_NOEXCEPT { _own_cpu = flag; }
    inline void ownGpu(bool flag = true) TBAG_NOEXCEPT { _own_gpu = flag; }

public:
    GpuBackendType type() const TBAG_NOEXCEPT;

public:
    void clear();
    bool realloc(std::size_t size);
    bool resize(std::size_t size);
};

} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_SYNCEDMEMORY_HPP__

