/**
 * @file   SyncedMemory.cpp
 * @brief  SyncedMemory class implementation.
 * @author zer0
 * @date   2017-12-28
 */

#include <libtbag/gpu/SyncedMemory.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/debug/Assert.hpp>

#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu {

SyncedMemory::SyncedMemory() : SyncedMemory(WeakedGpuStream())
{
    // EMPTY.
}

SyncedMemory::SyncedMemory(SharedGpuStream const & stream)
        : SyncedMemory(WeakedGpuStream(stream))
{
    // EMPTY.
}

SyncedMemory::SyncedMemory(WeakedGpuStream const & stream)
        : _stream(stream), _type(TypeTable::TT_UNKNOWN), _head(SyncedHead::SH_UNINITIALIZED),
          _gpu(), _host(), _size(0)
{
    // EMPTY.
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
    clear();
}

SyncedMemory & SyncedMemory::operator =(SyncedMemory const & obj)
{
    if (this != &obj) {
        _stream = obj._stream;
        _type   = obj._type;
        _head   = obj._head;
        _gpu    = obj._gpu;
        _host   = obj._host;
        _size   = obj._size;
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
        _stream.swap(obj._stream);
        std::swap(_type, obj._type);
        std::swap(_head, obj._head);
        _gpu.swap(obj._gpu);
        _host.swap(obj._host);
        std::swap(_size, obj._size);
    }
}

void SyncedMemory::toHost() const
{
    if (_head != SyncedHead::SH_HEAD_AT_GPU) {
        return; // SKIP.
    }
    assert(_head == SyncedHead::SH_HEAD_AT_GPU);
    _head = SyncedHead::SH_SYNCED;
}

void SyncedMemory::toGpu() const
{
    if (_head != SyncedHead::SH_HEAD_AT_HOST) {
        return; // SKIP.
    }
    assert(_head == SyncedHead::SH_HEAD_AT_HOST);
    _head = SyncedHead::SH_SYNCED;
}

void * SyncedMemory::getHostData()
{
    if (static_cast<bool>(_host)) {
        toHost();
        _head = SyncedHead::SH_HEAD_AT_HOST;
        return _host->data;
    }
    return nullptr;
}

void const * SyncedMemory::getHostData() const
{
    if (static_cast<bool>(_host)) {
        toHost();
        return _host->data;
    }
    return nullptr;
}

void * SyncedMemory::getGpuData()
{
    if (static_cast<bool>(_gpu)) {
        toGpu();
        _head = SyncedHead::SH_HEAD_AT_GPU;
        return _gpu->data;
    }
    return nullptr;
}

void const * SyncedMemory::getGpuData() const
{
    if (static_cast<bool>(_gpu)) {
        toGpu();
        return _gpu->data;
    }
    return nullptr;
}

bool SyncedMemory::cloneFrom(SyncedMemory const & obj)
{
    return false;
}

bool SyncedMemory::cloneTo(SyncedMemory & obj)
{
    return false;
}

void SyncedMemory::clear()
{
}

bool SyncedMemory::realloc(std::size_t size)
{
    return false;
}

bool SyncedMemory::resize(std::size_t size)
{
    return false;
}

} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

