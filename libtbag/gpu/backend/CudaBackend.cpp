/**
 * @file   CudaBackend.cpp
 * @brief  CudaBackend class implementation.
 * @author zer0
 * @date   2017-12-17
 */

#include <libtbag/gpu/backend/CudaBackend.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/string/Format.hpp>

#if defined(USE_CUDA)
#include <cuda.h>
#include <cuda_runtime.h>
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace backend {

GpuBackendType CudaBackend::getType() const TBAG_NOEXCEPT
{
    return GpuBackendType::GBT_CUDA;
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

CudaBackend::StringMap CudaBackend::getPlatformInfo(int device_index) const
{
    StringMap result;
#if defined(USE_CUDA)
    cudaDeviceProp prop;
    cudaError_t code = ::cudaGetDeviceProperties(&prop, device_index);
    if (code != cudaSuccess) {
        tDLogE("CudaBackend::getPlatformInfo({}) CUDA Error: {}", device_index, ::cudaGetErrorString(code));
        return result;
    }

    result.insert(StringMap::value_type(PLATFORM_VERSION   , string::fformat("CUDA {}.{}", prop.major, prop.minor)));
    result.insert(StringMap::value_type(PLATFORM_NAME      , std::string(prop.name)));
    result.insert(StringMap::value_type(PLATFORM_VENDOR    , std::string("NVIDIA")));
#endif
    return result;
}

} // namespace backend
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

