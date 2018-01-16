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
    WeakedGpuStream          _stream;
    TypeTable                _type;
    SyncedHead       mutable _head;
    SharedGpuMemory  mutable _gpu;
    SharedHostMemory mutable _host;
    std::size_t              _size;

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

public:
    void toHost() const;
    void  toGpu() const;

public:
    void       * getHostData();
    void const * getHostData() const;
    void       *  getGpuData();
    void const *  getGpuData() const;

public:
    void const * castHostData() const;
    void const *  castGpuData() const;

public:
    bool cloneFrom(SyncedMemory const & obj);
    bool cloneTo(SyncedMemory & obj);

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

