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
    if (clGetPlatformIDs(0, nullptr, &num_platforms) == CL_SUCCESS) {
        return num_platforms;
    }
#endif
    return 0;
}

OpenCLBackend::StringMap OpenCLBackend::getPlatformInfo(int device_index) const
{
    StringMap result;
#if defined(USE_OPENCL)
    cl_platform_id platform_id;
    cl_uint num_platforms;
    if (clGetPlatformIDs((cl_uint)device_index, &platform_id, &num_platforms) != CL_SUCCESS) {
        return result;
    }

    auto getPlatformInfo = [&](cl_platform_info info, std::string const & key){
        std::size_t param_value_size_ret = 0;
        if (clGetPlatformInfo(platform_id, info, 0, nullptr, &param_value_size_ret) == CL_SUCCESS) {
            std::vector<char> buffer(param_value_size_ret, '\0');
            if (clGetPlatformInfo(platform_id, info, buffer.size(), buffer.data(), nullptr) == CL_SUCCESS) {
                result.insert(StringMap::value_type(key, buffer.data()));
            }
        }
    };

    getPlatformInfo(CL_PLATFORM_PROFILE   , PLATFORM_PROFILE);
    getPlatformInfo(CL_PLATFORM_VERSION   , PLATFORM_VERSION);
    getPlatformInfo(CL_PLATFORM_NAME      , PLATFORM_NAME);
    getPlatformInfo(CL_PLATFORM_VENDOR    , PLATFORM_VENDOR);
    getPlatformInfo(CL_PLATFORM_EXTENSIONS, PLATFORM_EXTENSIONS);
#endif
    return result;
}

} // namespace backend
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

