/**
 * @file   Gpu.cpp
 * @brief  Gpu class implementation.
 * @author zer0
 * @date   2017-12-17
 */

#include <libtbag/gpu/Gpu.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/debug/Assert.hpp>

#include <libtbag/gpu/backend/AccelBackend.hpp>
#include <libtbag/gpu/backend/CpuBackend.hpp>
#include <libtbag/gpu/backend/CudaBackend.hpp>
#include <libtbag/gpu/backend/OpenCLBackend.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu {

Gpu::Gpu(BackendType type) : TYPE(type)
{
    switch (type) {
    case BackendType::GBT_CPU:    _backend.reset(new backend::CpuBackend());    break;
    case BackendType::GBT_ACCEL:  _backend.reset(new backend::AccelBackend());  break;
    case BackendType::GBT_CUDA:   _backend.reset(new backend::CudaBackend());   break;
    case BackendType::GBT_OPENCL: _backend.reset(new backend::OpenCLBackend()); break;
    default: TBAG_INACCESSIBLE_BLOCK_ASSERT(); break;
    }
}

Gpu::~Gpu()
{
    // EMPTY.
}

bool Gpu::isSupport() const
{
    return _backend->isSupport();
}

int Gpu::getDeviceCount() const
{
    return _backend->getDeviceCount();
}

} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

