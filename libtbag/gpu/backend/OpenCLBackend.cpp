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

int OpenCLBackend::getPlatformCount() const
{
    int result = 0;
#if defined(USE_OPENCL)
    cl_uint num_platforms;
    cl_int code = ::clGetPlatformIDs(0, nullptr, &num_platforms);
    if (code == CL_SUCCESS) {
        result = static_cast<int>(num_platforms);
    } else {
        tDLogE("OpenCLBackend::getDeviceCount() OpenCL error code: {}", code);
        return 0;
    }
#endif
    return result;
}

OpenCLBackend::Platforms OpenCLBackend::getPlatformList() const
{
    Platforms result;
    for (int i = 0; i < getDeviceCount(); ++i) {
        GpuPlatform plat(getType(), i + 1);
#if defined(USE_OPENCL)
        auto get_platform_info = [](cl_platform_id id, cl_platform_info info) -> std::string {
            size_t value_size = 0;
            if (::clGetPlatformInfo(id, info, 0, nullptr, &value_size) == CL_SUCCESS) {
                std::vector<char> buffer(value_size, '\0');
                if (::clGetPlatformInfo(id, info, buffer.size(), buffer.data(), nullptr) == CL_SUCCESS) {
                    return std::string(buffer.data());
                }
            }
            return std::string();
        };
        cl_platform_id platform_id;
        if (::clGetPlatformIDs((cl_uint)plat.number, &platform_id, nullptr) == CL_SUCCESS) {
            plat.profile    = get_platform_info(platform_id, CL_PLATFORM_PROFILE);
            plat.version    = get_platform_info(platform_id, CL_PLATFORM_VERSION);
            plat.name       = get_platform_info(platform_id, CL_PLATFORM_NAME);
            plat.vendor     = get_platform_info(platform_id, CL_PLATFORM_VENDOR);
            plat.extensions = get_platform_info(platform_id, CL_PLATFORM_EXTENSIONS);
        }
#endif
        result.push_back(plat);
    }
    return result;
}

int OpenCLBackend::getDeviceCount() const
{
    return 1;
}

OpenCLBackend::Devices OpenCLBackend::getDeviceList() const
{
    Devices result;
    GpuDevice dev(getType(), 0);
    result.push_back(dev);
    return result;
}

} // namespace backend
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

