/**
 * @file   SyncedMemory.cpp
 * @brief  SyncedMemory class implementation.
 * @author zer0
 * @date   2018-01-26
 */

#include <libtbag/gpu/details/SyncedMemory.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/gpu/details/GpuContext.hpp>
#include <libtbag/gpu/details/GpuStream.hpp>
#include <libtbag/gpu/details/GpuEvent.hpp>
#include <libtbag/gpu/details/GpuMemory.hpp>

#include <cassert>
#include <cstring>
#include <utility>
#include <algorithm>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace details {

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
          _gpu(), _host(), _elem_size(0), _elem_count(0), _async(async)
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
        _stream     = obj._stream;
        _event      = obj._event;
        _type       = obj._type;
        _flag       = obj._flag;
        _head       = obj._head;
        _gpu        = obj._gpu;
        _host       = obj._host;
        _elem_size  = obj._elem_size;
        _elem_count = obj._elem_count;
        _async      = obj._async;
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
        std::swap(_flag, obj._flag);
        std::swap(_head, obj._head);
        _gpu .swap(obj._gpu);
        _host.swap(obj._host);
        std::swap(_elem_size,  obj._elem_size);
        std::swap(_elem_count, obj._elem_count);
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
        code = _gpu->copyAsync(*_host, size(), (_event.expired() ? nullptr : _event.lock().get()));
    } else {
        code = _gpu->copy(*_host, size(), (_event.expired() ? nullptr : _event.lock().get()));
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
        code = _host->copyAsync(*_gpu, size(), (_event.expired() ? nullptr : _event.lock().get()));
    } else {
        code = _host->copy(*_gpu, size(), (_event.expired() ? nullptr : _event.lock().get()));
    }

    if (isSuccess(code)) {
        _head = SyncedHead::SH_SYNCED;
    }
    return code;
}

Err SyncedMemory::toSync() const
{
    if (_head == SyncedHead::SH_HEAD_AT_HOST) {
        return toGpu();
    } else if (_head == SyncedHead::SH_HEAD_AT_GPU) {
        return toHost();
    } else {
        return Err::E_ILLSTATE;
    }
    return Err::E_ALREADY;
}

void * SyncedMemory::getMutableHostData()
{
    if (static_cast<bool>(_host)) {
        toHost();
        _head = SyncedHead::SH_HEAD_AT_HOST;
        return _host->data();
    }
    return nullptr;
}

void * SyncedMemory::getMutableGpuData()
{
    if (static_cast<bool>(_gpu)) {
        toGpu();
        _head = SyncedHead::SH_HEAD_AT_GPU;
        return _gpu->data();
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

void const * SyncedMemory::getGpuData() const
{
    if (static_cast<bool>(_gpu)) {
        toGpu();
        return _gpu->data();
    }
    return nullptr;
}

Err SyncedMemory::syncEvent() const
{
    return (_event.expired() ? Err::E_NULLPTR : _event.lock()->sync());
}

Err SyncedMemory::elapsedEvent(float * millisec) const
{
    return (_event.expired() ? Err::E_NULLPTR : _event.lock()->elapsed(millisec));
}

float SyncedMemory::elapsedEvent() const
{
    return (_event.expired() ? 0.0f : _event.lock()->elapsed());
}

bool SyncedMemory::exists() const
{
    return !_stream.expired() && _gpu && _host && size() > 0U;
}

bool SyncedMemory::empty() const
{
    return !exists();
}

Err SyncedMemory::alloc(std::size_t size, std::size_t count)
{
    if (size == 0 || count == 0) {
        return Err::E_ILLARGS;
    }

    if (_stream.expired()) {
        return Err::E_EXPIRED;
    }

    auto stream = _stream.lock();
    assert(static_cast<bool>(stream));

    if (_gpu || _host) {
        return Err::E_ILLSTATE;
    }

    try {
        _gpu.reset(new GpuMemory(*stream, size * count));
        _host.reset(new HostMemory(*stream, size * count, _flag));
    } catch (std::bad_alloc & e) {
        _gpu.reset();
        _host.reset();
        return Err::E_BADALLOC;
    }

    _elem_size  = size;
    _elem_count = count;
    return Err::E_SUCCESS;
}

Err SyncedMemory::free()
{
    if (_gpu) {
        _gpu.reset();
    }
    if (_host) {
        _host.reset();
    }
    _elem_size  = 0;
    _elem_count = 0;
    return Err::E_SUCCESS;
}

Err SyncedMemory::copyFrom(SyncedMemory const & obj)
{
    if (this == &obj) {
        return Err::E_SAMEOBJ;
    }
    if (obj.empty()) {
        return Err::E_ILLARGS;
    }
    if (size() < obj.size()) {
        return Err::E_SMALLBUF;
    }

    assert(obj.size() > 0);
    assert(obj._gpu);
    assert(obj._host);

    assert(size() >= obj.size());
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
        tDLogE("SyncedMemory::copyFrom() Copy error: gpu({}), host({})",
               getErrName(gpu_code), getErrName(host_code));
        return Err::E_ECOPY;
    }

    _head = obj._head;
    return Err::E_SUCCESS;
}

Err SyncedMemory::copyTo(SyncedMemory & obj) const
{
    if (this == &obj) {
        return Err::E_SAMEOBJ;
    }
    return obj.copyFrom(*this);
}

Err SyncedMemory::cloneFrom(SyncedMemory const & obj)
{
    if (this == &obj) {
        return Err::E_SAMEOBJ;
    }

    _stream = obj._stream;
    _event  = obj._event;
    _type   = obj._type;
    _flag   = obj._flag;
    _async  = obj._async;

    if (obj.empty()) {
        free();
        return Err::E_SUCCESS;
    }

    assert(obj.size() > 0);
    Err const CODE = resize(obj._elem_size, obj._elem_count);
    if (isFailure(CODE)) {
        tDLogE("SyncedMemory::cloneFrom() Resize error: {}", getErrName(CODE));
        return CODE;
    }

    return copyFrom(obj);
}

Err SyncedMemory::cloneTo(SyncedMemory & obj) const
{
    return obj.cloneFrom(*this);
}

Err SyncedMemory::resize(std::size_t size, std::size_t count)
{
    if (size == 0 || count == 0) {
        free();
        return Err::E_SUCCESS;
    }

    assert(size > 0);
    assert(count > 0);

    if (exists() && _elem_size == size && _elem_count == count) {
        return Err::E_ALREADY;
    }

    free();
    return alloc(size, count);
}

Err SyncedMemory::resize(TypeTable type, std::size_t count)
{
    _type = type;
    return resize(type::getTypeSize(type), count);
}

Err SyncedMemory::assign(void const * data, std::size_t size)
{
    if (this->size() < size) {
        return Err::E_OORANGE;
    }

    void * host_data = getMutableHostData();
    if (host_data == nullptr) {
        return Err::E_NULLPTR;
    }
    assert(_head == SyncedMemory::SyncedHead::SH_HEAD_AT_HOST);

    ::memcpy(host_data, data, size);
    return Err::E_SUCCESS;
}

Err SyncedMemory::assignSync(void const * data, std::size_t size)
{
    Err const CODE = assign(data, size);
    if (isFailure(CODE)) {
        return CODE;
    }
    return toSync();
}

} // namespace details
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

