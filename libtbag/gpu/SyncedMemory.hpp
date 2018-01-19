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
    template <typename T>
    using TypeInfo  = type::TypeInfo<T>;
    using TypeTable = type::TypeTable;

public:
    enum class SyncedHead
    {
        SH_UNINITIALIZED,
        SH_HEAD_AT_HOST,
        SH_HEAD_AT_GPU,
        SH_SYNCED,
    };

private:
    WeakedGpuStream _stream;
    WeakedGpuEvent  _event;

private:
    TypeTable       _type;
    HostMemoryFlag  _flag;

private:
    SyncedHead mutable _head;
    SharedGpuMemory    _gpu;
    SharedHostMemory   _host;

private:
    std::size_t _elem_size;
    std::size_t _elem_count;
    bool        _async;

public:
    SyncedMemory();
    explicit SyncedMemory(SharedGpuStream const & stream, SharedGpuEvent const & event = SharedGpuEvent(), bool async = true);
    explicit SyncedMemory(WeakedGpuStream const & stream, WeakedGpuEvent const & event = WeakedGpuEvent(), bool async = true);
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
    inline std::size_t size() const TBAG_NOEXCEPT { return _elem_size * _elem_count; }

    inline std::size_t  sizeOfElem() const TBAG_NOEXCEPT { return _elem_size;  }
    inline std::size_t countOfElem() const TBAG_NOEXCEPT { return _elem_count; }

    inline HostMemoryFlag getFlag() const TBAG_NOEXCEPT { return _flag; }
    inline void setFlag(HostMemoryFlag flag = HostMemoryFlag::HMF_DEFAULT) TBAG_NOEXCEPT { _flag = flag; }

    inline bool isAsync() const TBAG_NOEXCEPT { return _async; }
    inline void setAsyncMode(bool flag = true) TBAG_NOEXCEPT { _async = flag; }

public:
    void setEvent(SharedGpuEvent const & event);
    void setEvent(WeakedGpuEvent const & event);

public:
    Err toHost() const;
    Err  toGpu() const;

public:
    void       * getHostData(); // Mutable assessor.
    void       *  getGpuData(); // Mutable assessor.
    void const * getHostData() const; // Immutable assessor.
    void const *  getGpuData() const; // Immutable assessor.

public:
    template <typename T> T       * castHostData()       { return getHostData(); }
    template <typename T> T       *  castGpuData()       { return  getGpuData(); }
    template <typename T> T const * castHostData() const { return getHostData(); }
    template <typename T> T const *  castGpuData() const { return  getGpuData(); }

// Event by-pass methods.
public:
    Err      sync() const;
    Err   elapsed(float * millisec) const;
    float elapsed() const;

// State checker.
public:
    bool exists() const;
    bool empty() const;

protected:
    Err alloc(std::size_t size, std::size_t count);
    Err free();

public:
    Err cloneFrom(SyncedMemory const & obj);
    Err cloneTo(SyncedMemory & obj) const;

public:
    Err resize(std::size_t size, std::size_t count);

public:
    template <typename T>
    Err resize(std::size_t size)
    {
        _type = TypeInfo<T>::table();
        return resize(sizeof(T), size);
    }

    Err resize(std::size_t size)
    {
        _type = TypeTable::TT_CHAR;
        return resize(1, size);
    }
};

} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_SYNCEDMEMORY_HPP__

