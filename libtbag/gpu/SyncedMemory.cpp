/**
 * @file   SyncedMemory.cpp
 * @brief  SyncedMemory class implementation.
 * @author zer0
 * @date   2017-12-28
 */

#include <libtbag/gpu/SyncedMemory.hpp>
#include <libtbag/log/Log.hpp>

#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu {

//SyncedMemory::SyncedMemory(GpuBackendType type) : SyncedMemory(0, type)
//{
//    // EMPTY.
//}

//SyncedMemory::SyncedMemory(std::size_t size, GpuBackendType type)
//        : _backend(createGpuContext(type)), _head(SyncedHead::SH_UNINITIALIZED), _capacity(0), _size(0),
//          _own_cpu(true), _own_gpu(true), _cpu(nullptr), _gpu(nullptr)
//{
//    if (static_cast<bool>(_backend) == false) {
//        throw std::bad_alloc();
//    }
//    if (size > 0 && resize(size) == false) {
//        throw std::bad_alloc();
//    }
//}

//SyncedMemory::SyncedMemory(SyncedMemory const & obj) : SyncedMemory(obj.type())
//{
//    *this = obj;
//}
//
//SyncedMemory::SyncedMemory(SyncedMemory && obj) : SyncedMemory(obj.type())
//{
//    *this = std::move(obj);
//}
//
//SyncedMemory::~SyncedMemory()
//{
//    clear();
//}

SyncedMemory & SyncedMemory::operator =(SyncedMemory const & obj)
{
    cloneFrom(obj);
    return *this;
}

SyncedMemory & SyncedMemory::operator =(SyncedMemory && obj)
{
    swap(obj);
    return *this;
}

bool SyncedMemory::cloneFrom(SyncedMemory const & obj)
{
    if (this == &obj) {
        return true;
    }
    return true;
}

void SyncedMemory::swap(SyncedMemory & obj)
{
    if (this == &obj) {
        return;
    }
}

//GpuBackendType SyncedMemory::type() const TBAG_NOEXCEPT
//{
//    return _backend->getType();
//}

void SyncedMemory::clear()
{
    if (_cpu && _own_cpu) {
    }
    if (_gpu && _own_gpu) {
//        int initial_device;
//        cudaGetDevice(&initial_device);
//        if (gpu_device_ != -1) {
//            CUDA_CHECK(cudaSetDevice(gpu_device_));
//        }
//        CUDA_CHECK(cudaFree(gpu_ptr_));
//        cudaSetDevice(initial_device);
    }
    _head     = SyncedHead::SH_UNINITIALIZED;
    _capacity = 0;
    _size     = 0;
    _own_cpu  = true;
    _own_gpu  = true;
    _cpu      = nullptr;
    _gpu      = nullptr;
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

