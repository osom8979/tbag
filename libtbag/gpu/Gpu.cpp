/**
 * @file   Gpu.cpp
 * @brief  Gpu class implementation.
 * @author zer0
 * @date   2017-12-17
 */

#include <libtbag/gpu/Gpu.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu {

// -------------------
// Gpu implementation.
// -------------------

Gpu::Gpu() : _gpu()
{
    // EMPTY.
}

Gpu::Gpu(GpuType type, std::size_t platform_index, std::size_t device_index) : _gpu()
{
    if (init(type, platform_index, device_index) == false) {
        throw std::bad_alloc();
    }
}

Gpu::Gpu(SharedGpuContext const & gpu) : _gpu(gpu)
{
    // EMPTY.
}

Gpu::Gpu(WeakedGpuContext const & gpu) : _gpu(gpu)
{
    // EMPTY.
}

Gpu::Gpu(Gpu const & obj) : Gpu()
{
    (*this) = obj;
}

Gpu::Gpu(Gpu && obj) : Gpu()
{
    (*this) = std::move(obj);
}

Gpu::~Gpu()
{
    // EMPTY.
}

Gpu & Gpu::operator =(Gpu const & obj)
{
    if (this != &obj) {
        _gpu = obj._gpu;
    }
    return *this;
}

Gpu & Gpu::operator =(Gpu && obj)
{
    swap(obj);
    return *this;
}

void Gpu::swap(Gpu & obj)
{
    if (this != &obj) {
        _gpu.swap(obj._gpu);
    }
}

bool Gpu::init(GpuType type, std::size_t platform_index, std::size_t device_index)
{
    if (libtbag::gpu::isSupport(type) == false) {
        return false;
    }

    auto const PLATFORMS = libtbag::gpu::getPlatformList(type);
    if (PLATFORMS.size() > platform_index) {
        return false;
    }

    auto const DEVICES = libtbag::gpu::getDeviceList(PLATFORMS[platform_index]);
    if (DEVICES.empty() > device_index) {
        return false;
    }

    auto context = libtbag::gpu::createContext(DEVICES[device_index]);
    if (static_cast<bool>(context)) {
        _gpu = context;
        return true;
    }
    return false;
}

} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

