/**
 * @file   CudaBackend.cpp
 * @brief  CudaBackend class implementation.
 * @author zer0
 * @date   2017-08-01
 */

#include <libtbag/gpu/CudaBackend.hpp>
#include <libtbag/log/Log.hpp>

#if defined(USE_CUDA)
#include <cuda.h>
#include <cuda_runtime.h>
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu {

CudaBackend::CudaBackend()
{
    // EMPTY.
}

CudaBackend::~CudaBackend()
{
    // EMPTY.
}

BackendType CudaBackend::getType() const TBAG_NOEXCEPT
{
    return BackendType::BT_CUDA;
}

bool CudaBackend::isSupport() const TBAG_NOEXCEPT
{
    return isCudaSupport();
}

int CudaBackend::getDeviceCount() const
{
#if defined(USE_CUDA)
    int count = 0;
    ::cudaGetDeviceCount(&count);
    return count;
#endif
    return 0;
}

} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

