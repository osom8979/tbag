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
#include <libtbag/gpu/details/GpuDetails.hpp>
#include <libtbag/type/TypeTable.hpp>

#include <memory>

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
    using TypeTable        = type::TypeTable;
    using SharedGpuMemory  = std::shared_ptr<GpuMemory>;
    using SharedHostMemory = std::shared_ptr<HostMemory>;

public:
    enum class SyncedHead
    {
        SH_UNINITIALIZED,
        SH_HEAD_AT_HOST,
        SH_HEAD_AT_GPU,
        SH_SYNCED,
    };

private:
    WeakedGpuStream    _stream;
    WeakedGpuEvent     _event;
    TypeTable          _type;
    SyncedHead mutable _head;
    SharedGpuMemory    _gpu;
    SharedHostMemory   _host;
    std::size_t        _size;
    bool               _async;

public:
    SyncedMemory();
    explicit SyncedMemory(SharedGpuStream const & stream);
    explicit SyncedMemory(WeakedGpuStream const & stream);
    SyncedMemory(SyncedMemory const & obj);
    SyncedMemory(SyncedMemory && obj);
    ~SyncedMemory();

public:
    SyncedMemory & operator =(SyncedMemory const & obj);
    SyncedMemory & operator =(SyncedMemory && obj);

public:
    void swap(SyncedMemory & obj);
    inline friend void swap(SyncedMemory & lh, SyncedMemory & rh) { lh.swap(rh); }

public:
    inline SyncedHead  head() const TBAG_NOEXCEPT { return _head; }
    inline std::size_t size() const TBAG_NOEXCEPT { return _size; }

    inline bool isAsync() const TBAG_NOEXCEPT { return _async; }
    inline void setAsyncMode(bool flag = true) TBAG_NOEXCEPT { _async = flag; }

public:
    void setEvent(SharedGpuEvent const & event);
    void setEvent(WeakedGpuEvent const & event);

public:
    Err toHost() const;
    Err  toGpu() const;

public:
    void * getHostData(); // Mutable assessor.
    void *  getGpuData(); // Mutable assessor.

public:
    void const * getHostData() const; // Immutable assessor.
    void const *  getGpuData() const; // Immutable assessor.

public:
    Err      sync() const;
    float elapsed() const;

public:
    Err alloc(std::size_t size);
    Err free();

public:
    bool cloneFrom(SyncedMemory const & obj);
    bool cloneTo(SyncedMemory & obj);

public:
    void clear();
    bool resize(std::size_t size);
};

} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_SYNCEDMEMORY_HPP__

