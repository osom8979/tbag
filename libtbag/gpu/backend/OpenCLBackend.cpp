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

bool OpenCLBackend::isHost() const TBAG_NOEXCEPT
{
    return false;
}

bool OpenCLBackend::isDevice() const TBAG_NOEXCEPT
{
    return true;
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

GpuPlatforms OpenCLBackend::getPlatformList() const
{
    GpuPlatforms result;
    for (int i = 0; i < getPlatformCount(); ++i) {
        result.emplace_back(getType(), i + 1);
    }
    return result;
}

GpuPlatformInfo OpenCLBackend::getPlatformInfo(GpuPlatform const & platform) const
{
    GpuPlatformInfo info(platform);
#if defined(USE_OPENCL)
    cl_platform_id platform_id;
    if (::clGetPlatformIDs((cl_uint)info.platform_number, &platform_id, nullptr) == CL_SUCCESS) {
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
        info.profile    = get_platform_info(platform_id, CL_PLATFORM_PROFILE);
        info.version    = get_platform_info(platform_id, CL_PLATFORM_VERSION);
        info.name       = get_platform_info(platform_id, CL_PLATFORM_NAME);
        info.vendor     = get_platform_info(platform_id, CL_PLATFORM_VENDOR);
        info.extensions = get_platform_info(platform_id, CL_PLATFORM_EXTENSIONS);
    }
#endif
    return info;
}

int OpenCLBackend::getDeviceCount(GpuPlatform const & platform) const
{
    return 1;
}

GpuDevices OpenCLBackend::getDeviceList(GpuPlatform const & platform) const
{
    return {GpuDevice(platform, 0)};
}

GpuDeviceInfo OpenCLBackend::getDeviceInfo(GpuDevice const & device) const
{
    GpuDeviceInfo info(device);
    return info;
}

} // namespace backend
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

