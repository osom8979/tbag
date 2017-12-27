/**
 * @file   OpenCLBackend.cpp
 * @brief  OpenCLBackend class implementation.
 * @author zer0
 * @date   2017-12-17
 */

#include <libtbag/gpu/backend/OpenCLBackend.hpp>
#include <libtbag/log/Log.hpp>

#if defined(USE_OPENCL)
# if defined(TBAG_PLATFORM_MACOS)
#  include <OpenCL/cl.h>
# else
#  include <CL/cl.h>
# endif
#endif

#include <array>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace backend {

GpuBackendType OpenCLBackend::getType() const TBAG_NOEXCEPT
{
    return GpuBackendType::GBT_OPENCL;
}

bool OpenCLBackend::isSupport() const TBAG_NOEXCEPT
{
    return isOpenCLSupport();
}

int OpenCLBackend::getDeviceCount() const
{
#if defined(USE_OPENCL)
    cl_uint num_platforms;
    if (::clGetPlatformIDs(0, nullptr, &num_platforms) == CL_SUCCESS) {
        return num_platforms;
    }
#endif
    return 0;
}

OpenCLBackend::Devices OpenCLBackend::getDeviceList() const
{
    Devices result;
    for (int i = 0; i < getDeviceCount(); ++i) {
        result.emplace_back(GpuBackendType::GBT_OPENCL, i + 1);
    }
    return result;
}

OpenCLBackend::StringMap OpenCLBackend::getPlatformInfo(GpuDevice const & device) const
{
    StringMap result;
#if defined(USE_OPENCL)
    cl_platform_id platform_id;
    cl_uint num_platforms;
    if (::clGetPlatformIDs((cl_uint)device.number, &platform_id, &num_platforms) != CL_SUCCESS) {
        return result;
    }

    auto platform_info = [&](cl_platform_info info, std::string const & key){
        std::size_t param_value_size_ret = 0;
        if (clGetPlatformInfo(platform_id, info, 0, nullptr, &param_value_size_ret) == CL_SUCCESS) {
            std::vector<char> buffer(param_value_size_ret, '\0');
            if (clGetPlatformInfo(platform_id, info, buffer.size(), buffer.data(), nullptr) == CL_SUCCESS) {
                result.insert(StringPair(key, buffer.data()));
            }
        }
    };

    platform_info(CL_PLATFORM_PROFILE   , PLATFORM_PROFILE);
    platform_info(CL_PLATFORM_VERSION   , PLATFORM_VERSION);
    platform_info(CL_PLATFORM_NAME      , PLATFORM_NAME);
    platform_info(CL_PLATFORM_VENDOR    , PLATFORM_VENDOR);
    platform_info(CL_PLATFORM_EXTENSIONS, PLATFORM_EXTENSIONS);
#endif
    return result;
}

} // namespace backend
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

