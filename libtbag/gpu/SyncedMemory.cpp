/**
 * @file   SyncedMemory.cpp
 * @brief  SyncedMemory class implementation.
 * @author zer0
 * @date   2017-12-28
 */

#include <libtbag/gpu/SyncedMemory.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/debug/Assert.hpp>

#include <cassert>
#include <utility>
#include <algorithm>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu {

SyncedMemory::SyncedMemory() : SyncedMemory(WeakedGpuStream())
{
    // EMPTY.
}

SyncedMemory::SyncedMemory(SharedGpuStream const & stream, SharedGpuEvent const & event, bool async)
        : SyncedMemory(WeakedGpuStream(stream), WeakedGpuEvent(event), async)
{
    // EMPTY.
}

SyncedMemory::SyncedMemory(WeakedGpuStream const & stream, WeakedGpuEvent const & event, bool async)
        : _stream(stream), _event(event), _type(TypeTable::TT_UNKNOWN),
          _flag(HostMemoryFlag::HMF_DEFAULT), _head(SyncedHead::SH_UNINITIALIZED),
          _gpu(), _host(), _size(0), _async(async)
{
    if (auto shared_stream = stream.lock()) {
        if (shared_stream->atContext().getType() == GpuType::GT_CUDA) {
            _flag = HostMemoryFlag::HMF_PINNED;
        }
    }
}

SyncedMemory::SyncedMemory(SyncedMemory const & obj) : SyncedMemory()
{
    (*this) = obj;
}

SyncedMemory::SyncedMemory(SyncedMemory && obj) : SyncedMemory()
{
    (*this) = std::move(obj);
}

SyncedMemory::~SyncedMemory()
{
    // EMPTY.
}

SyncedMemory & SyncedMemory::operator =(SyncedMemory const & obj)
{
    if (this != &obj) {
        // @formatter:off
        _stream = obj._stream;
        _event  = obj._event;
        _type   = obj._type;
        _head   = obj._head;
        _gpu    = obj._gpu;
        _host   = obj._host;
        _size   = obj._size;
        _async  = obj._async;
        // @formatter:on
    }
    return *this;
}

SyncedMemory & SyncedMemory::operator =(SyncedMemory && obj)
{
    swap(obj);
    return *this;
}

void SyncedMemory::swap(SyncedMemory & obj)
{
    if (this != &obj) {
        // @formatter:off
        _stream.swap(obj._stream);
        _event .swap(obj._event);
        std::swap(_type, obj._type);
        std::swap(_head, obj._head);
        _gpu .swap(obj._gpu);
        _host.swap(obj._host);
        std::swap(_size , obj._size);
        std::swap(_async, obj._async);
        // @formatter:on
    }
}

void SyncedMemory::setEvent(SharedGpuEvent const & event)
{
    _event = event;
}

void SyncedMemory::setEvent(WeakedGpuEvent const & event)
{
    _event = event;
}

Err SyncedMemory::toHost() const
{
    if (_head != SyncedHead::SH_HEAD_AT_GPU) {
        return Err::E_SUCCESS; // SKIP.
    }
    assert(_head == SyncedHead::SH_HEAD_AT_GPU);

    Err code = Err::E_UNKNOWN;
    if (_async) {
        code = _gpu->copyAsync(*_host, _size, (_event.expired() ? nullptr : _event.lock().get()));
    } else {
        code = _gpu->copy(*_host, _size, (_event.expired() ? nullptr : _event.lock().get()));
    }

    if (isSuccess(code)) {
        _head = SyncedHead::SH_SYNCED;
    }
    return code;
}

Err SyncedMemory::toGpu() const
{
    if (_head != SyncedHead::SH_HEAD_AT_HOST) {
        return Err::E_SUCCESS; // SKIP.
    }
    assert(_head == SyncedHead::SH_HEAD_AT_HOST);

    Err code = Err::E_UNKNOWN;
    if (_async) {
        code = _host->copyAsync(*_gpu, _size, (_event.expired() ? nullptr : _event.lock().get()));
    } else {
        code = _host->copy(*_gpu, _size, (_event.expired() ? nullptr : _event.lock().get()));
    }

    if (isSuccess(code)) {
        _head = SyncedHead::SH_SYNCED;
    }
    return code;
}

void * SyncedMemory::getHostData()
{
    if (static_cast<bool>(_host)) {
        toHost();
        _head = SyncedHead::SH_HEAD_AT_HOST;
        return _host->data();
    }
    return nullptr;
}

void const * SyncedMemory::getHostData() const
{
    if (static_cast<bool>(_host)) {
        toHost();
        return _host->data();
    }
    return nullptr;
}

void * SyncedMemory::getGpuData()
{
    if (static_cast<bool>(_gpu)) {
        toGpu();
        _head = SyncedHead::SH_HEAD_AT_GPU;
        return _gpu->data();
    }
    return nullptr;
}

void const * SyncedMemory::getGpuData() const
{
    if (static_cast<bool>(_gpu)) {
        toGpu();
        return _gpu->data();
    }
    return nullptr;
}

Err SyncedMemory::sync() const
{
    return (_event.expired() ? Err::E_NULLPTR : _event.lock()->sync());
}

Err SyncedMemory::elapsed(float * millisec) const
{
    return (_event.expired() ? Err::E_NULLPTR : _event.lock()->elapsed(millisec));
}

float SyncedMemory::elapsed() const
{
    return (_event.expired() ? 0.0f : _event.lock()->elapsed());
}

bool SyncedMemory::exists() const
{
    return !_stream.expired() && _gpu && _host && _size > 0U;
}

bool SyncedMemory::empty() const
{
    return !exists();
}

Err SyncedMemory::alloc(std::size_t size)
{
    if (_stream.expired()) {
        return Err::E_EXPIRED;
    }

    auto stream = _stream.lock();
    assert(static_cast<bool>(stream));

    if (_gpu || _host) {
        return Err::E_ILLSTATE;
    }

    try {
        _gpu.reset(new GpuMemory(*stream, size));
        _host.reset(new HostMemory(*stream, size, _flag));
    } catch (std::bad_alloc & e) {
        _gpu.reset();
        _host.reset();
        return Err::E_BADALLOC;
    }

    _size = size;
    return Err::E_SUCCESS;
}

Err SyncedMemory::free()
{
    if (static_cast<bool>(_gpu)) {
        _gpu.reset();
    } else {
        tDLogW("SyncedMemory::free() GPU memory is already released.");
    }

    if (static_cast<bool>(_host)) {
        _host.reset();
    } else {
        tDLogW("SyncedMemory::free() Host memory is already released.");
    }

    _size = 0;
    return Err::E_SUCCESS;
}

Err SyncedMemory::resize(std::size_t size)
{
    if (size == 0) {
        free();
        return Err::E_SUCCESS;
    }

    assert(size > 0);
    if (exists() && _size == size) {
        return Err::E_ALREADY;
    }

    free();
    return alloc(size);
}

Err SyncedMemory::cloneFrom(SyncedMemory const & obj)
{
    if (this == &obj) {
        return Err::E_SAMEOBJ;
    }

    _stream = obj._stream;
    _event  = obj._event;
    _type   = obj._type;
    _head   = obj._head;
    _async  = obj._async;

    if (obj.empty()) {
        resize(0);
        return Err::E_SUCCESS;
    }

    assert(obj.size() > 0);
    Err const CODE = resize(obj.size());
    if (isFailure(CODE)) {
        tDLogE("SyncedMemory::cloneFrom() Resize error: {}", getErrName(CODE));
        return CODE;
    }

    assert(obj._gpu);
    assert(obj._host);
    assert(_gpu);
    assert(_host);

    GpuEvent * event = nullptr;

    // Prevents the object from expiring before using the event.
    SharedGpuEvent event_lock;
    if (_event.expired() == false) {
        event_lock = _event.lock();
        event = event_lock.get();
    }

    Err  gpu_code = Err::E_UNKNOWN;
    Err host_code = Err::E_UNKNOWN;

    if (_async) {
        gpu_code  = obj._gpu->copy(*_gpu, obj.size(), event);
        host_code = obj._host->copy(*_host, obj.size(), event);
    } else {
        gpu_code  = obj._gpu->copyAsync(*_gpu, obj.size(), event);
        host_code = obj._host->copyAsync(*_host, obj.size(), event);
    }

    if (isFailure(gpu_code) || isFailure(host_code)) {
        tDLogE("SyncedMemory::cloneFrom() Copy error: gpu({}), host({})",
               getErrName(gpu_code), getErrName(host_code));
        return Err::E_ECOPY;
    }
    return Err::E_SUCCESS;
}

Err SyncedMemory::cloneTo(SyncedMemory & obj)
{
    return obj.cloneFrom(*this);
}

} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

