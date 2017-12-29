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
    if (code != CL_SUCCESS) {
        tDLogE("OpenCLBackend::getPlatformCount() OpenCL clGetPlatformIDs() error code: {}", code);
        return 0;
    }
    result = static_cast<int>(num_platforms);
#endif
    return result;
}

GpuPlatforms OpenCLBackend::getPlatformList() const
{
    GpuPlatforms result;
#if defined(USE_OPENCL)
    std::vector<cl_platform_id> platforms((std::size_t)getPlatformCount());
    cl_int code = ::clGetPlatformIDs((cl_uint)platforms.size(), platforms.data(), nullptr);
    if (code != CL_SUCCESS) {
        tDLogE("OpenCLBackend::getPlatformList() OpenCL clGetPlatformIDs() error code: {}", code);
        return result;
    }
    for (auto & id : platforms) {
        result.emplace_back(getType(), (std::size_t)id);
    }
#endif
    return result;
}

GpuPlatformInfo OpenCLBackend::getPlatformInfo(GpuPlatform const & platform) const
{
    GpuPlatformInfo info(platform);
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

    info.profile    = get_platform_info((cl_platform_id)platform.platform_number, CL_PLATFORM_PROFILE);
    info.version    = get_platform_info((cl_platform_id)platform.platform_number, CL_PLATFORM_VERSION);
    info.name       = get_platform_info((cl_platform_id)platform.platform_number, CL_PLATFORM_NAME);
    info.vendor     = get_platform_info((cl_platform_id)platform.platform_number, CL_PLATFORM_VENDOR);
    info.extensions = get_platform_info((cl_platform_id)platform.platform_number, CL_PLATFORM_EXTENSIONS);
#endif
    return info;
}

int OpenCLBackend::getDeviceCount(GpuPlatform const & platform) const
{
    int result = 0;
#if defined(USE_OPENCL)
    cl_uint num_devices;
    cl_int code = ::clGetDeviceIDs((cl_platform_id)platform.platform_number, CL_DEVICE_TYPE_ALL, 0, nullptr, &num_devices);
    if (code != CL_SUCCESS) {
        tDLogE("OpenCLBackend::getDeviceCount() OpenCL clGetDeviceIDs() error code: {}", code);
        return 0;
    }
    result = static_cast<int>(num_devices);
#endif
    return result;
}

GpuDevices OpenCLBackend::getDeviceList(GpuPlatform const & platform) const
{
    GpuDevices result;
#if defined(USE_OPENCL)
    std::vector<cl_device_id> devices((std::size_t)getDeviceCount(platform));
    cl_int code = ::clGetDeviceIDs((cl_platform_id)platform.platform_number, CL_DEVICE_TYPE_ALL,
                                   (cl_uint)devices.size(), devices.data(), nullptr);
    if (code != CL_SUCCESS) {
        tDLogE("OpenCLBackend::getDeviceList() OpenCL clGetDeviceIDs() error code: {}", code);
        return result;
    }
    for (auto & id : devices) {
        result.emplace_back(platform, (std::size_t)id);
    }
#endif
    return result;
}

GpuDeviceInfo OpenCLBackend::getDeviceInfo(GpuDevice const & device) const
{
    GpuDeviceInfo info(device);
#if defined(USE_OPENCL)
    auto get_device_info = [](cl_device_id id, cl_device_info info) -> std::string {
        size_t value_size = 0;
        if (::clGetDeviceInfo(id, info, 0, nullptr, &value_size) == CL_SUCCESS) {
            std::vector<char> buffer(value_size, '\0');
            if (::clGetDeviceInfo(id, info, buffer.size(), buffer.data(), nullptr) == CL_SUCCESS) {
                return std::string(buffer.data());
            }
        }
        return std::string();
    };

    info.name = get_device_info((cl_device_id)device.device_number, CL_DEVICE_NAME);
#endif
    return info;
}

} // namespace backend
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

