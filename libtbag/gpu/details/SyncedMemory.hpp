/**
 * @file   SyncedMemory.hpp
 * @brief  SyncedMemory class prototype.
 * @author zer0
 * @date   2018-01-26
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_SYNCEDMEMORY_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_SYNCEDMEMORY_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/gpu/details/GpuCommon.hpp>
#include <libtbag/type/TypeTable.hpp>
#include <libtbag/container/AnyPod.hpp>

#include <cassert>
#include <memory>
#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace details {

class GpuStream;
class GpuEvent;
class GpuMemory;
class HostMemory;

/**
 * SyncedMemory class prototype.
 *
 * @author zer0
 * @date   2017-12-28
 * @date   2018-01-26 (Move: libtbag/gpu -> libtbag/gpu/details)
 */
class TBAG_API SyncedMemory : private Noncopyable
{
public:
    template <typename T>
    using TypeInfo  = type::TypeInfo<T>;
    using TypeTable = type::TypeTable;
    using AnyPod    = container::AnyPod;

    using SharedGpuStream  = std::shared_ptr<GpuStream>;
    using WeakedGpuStream  = std::weak_ptr<GpuStream>;
    using SharedGpuEvent   = std::shared_ptr<GpuEvent>;
    using WeakedGpuEvent   = std::weak_ptr<GpuEvent>;
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

public:
    TBAG_CONSTEXPR static std::size_t maxCount() TBAG_NOEXCEPT
    { return static_cast<std::size_t>(TypeInfo<int>::maximum()); }

private:
    WeakedGpuStream _stream;
    WeakedGpuEvent  _event;

private:
    HostMemoryFlag  _flag;  ///< Host memory flags.
    bool            _async; ///< Asynchronous of memory operation.

private:
    TypeTable          _type;  ///< Base type.
    SyncedHead mutable _head;  ///< Synchronization state.
    SharedHostMemory   _host;  ///< Host memory.
    SharedGpuMemory    _gpu;   ///< Gpu(Device) memory.
    std::size_t        _count; ///< Count of element.

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
    inline TypeTable   type() const TBAG_NOEXCEPT { return _type; }

    inline std::size_t  sizeOfElem() const TBAG_NOEXCEPT { return type::getTypeSize(_type);  }
    inline std::size_t countOfElem() const TBAG_NOEXCEPT { return _count; }

    inline std::size_t size() const TBAG_NOEXCEPT { return sizeOfElem() * countOfElem(); }

    inline HostMemoryFlag getFlag() const TBAG_NOEXCEPT { return _flag; }

    inline bool isAsync() const TBAG_NOEXCEPT { return _async; }
    inline void setAsyncMode(bool flag = true) TBAG_NOEXCEPT { _async = flag; }

public:
    void setEvent(SharedGpuEvent const & event);
    void setEvent(WeakedGpuEvent const & event);

public:
    Err toHost() const;
    Err  toGpu() const;
    Err toSync() const;

public:
    void * getMutableHostData();
    void *  getMutableGpuData();

    template <typename T> T * castMutableHostData() { return (T*)getMutableHostData(); }
    template <typename T> T *  castMutableGpuData() { return (T*) getMutableGpuData(); }

    void const * getHostData() const;
    void const *  getGpuData() const;

    template <typename T> T const * castHostData() const { return (T*)getHostData(); }
    template <typename T> T const *  castGpuData() const { return (T*) getGpuData(); }

// Event by-pass methods.
public:
    Err      syncEvent() const;
    Err   elapsedEvent(float * millisec) const;
    float elapsedEvent() const;

// State checker.
public:
    bool validate() const;
    bool   exists() const;
    bool    empty() const;

protected:
    Err alloc(TypeTable type, std::size_t count);
    Err free();

public:
    Err resize(TypeTable type, std::size_t count);

    template <typename T>
    Err resize(std::size_t size)
    { return resize(TypeInfo<T>::table(), size); }

public:
    Err cloneFrom(SyncedMemory const & obj);
    Err cloneTo(SyncedMemory & obj) const;

public:
    template <typename Predicated>
    Err autoSync(Predicated predicated)
    {
        if (_event.expired()) {
            return Err::E_UNSUPOP;
        }
        if (empty()) {
            return Err::E_ILLSTATE;
        }

        Err code = predicated();
        if (isFailure(code)) {
            return code;
        }

        code = toSync();
        if (isFailure(code)) {
            return code;
        }
        return syncEvent();
    }

private:
    template <typename SourceType, typename DestinationType>
    static Err _assign(SourceType const * src, DestinationType * dest, std::size_t size)
    {
        for (; size > 0; ++src, ++dest, --size) {
             *dest = static_cast<DestinationType>(*src);
        }
        return Err::E_SUCCESS;
    }

public:
    template <typename DataType>
    Err assign(DataType const * data, std::size_t count)
    {
        static_assert(std::is_void<DataType>::value == false, "void type is not supported.");
        if (empty()) {
            return Err::E_ILLSTATE;
        }
        if (data == nullptr || _count < count) {
            return Err::E_ILLARGS;
        }

        // @formatter:off
        switch (_type) {
#define _TBAG_XX(n, s, t) case TypeTable::TT_##n: return _assign(data, (t*)getMutableHostData(), count);
        TBAG_TYPE_TABLE_MAP(_TBAG_XX)
#undef _TBAG_XX
        default: return Err::E_UNSUPOP;
        }
        // @formatter:on
    }

    template <typename DataType>
    Err assignSync(DataType const * data, std::size_t count)
    {
        return autoSync([this, data, count]() -> Err { return this->assign(data, count); });
    }

public:
    Err fillHost(AnyPod const & data);
    Err fillHostSync(AnyPod const & data);

public:
    Err fillGpu(AnyPod const & data);
    Err fillGpuSync(AnyPod const & data);
};

} // namespace details
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_SYNCEDMEMORY_HPP__

