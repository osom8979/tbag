/**
 * @file   OpenCLBackend.cpp
 * @brief  OpenCLBackend class implementation.
 * @author zer0
 * @date   2018-01-13
 */

#include <libtbag/gpu/opencl/OpenCLBackend.hpp>
#include <libtbag/log/Log.hpp>

#if defined(USE_OPENCL)
# if defined(TBAG_PLATFORM_MACOS)
#  include <OpenCL/cl.h>
# else
#  include <CL/cl.h>
# endif
#endif

//#define TBAG_OPENCL_BACKEND_DEBUGGING
#define TBAG_OPENCL_BACKEND_PROFILE

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu    {
namespace opencl {

// ---------------
namespace __impl {
// ---------------

TBAG_CONSTEXPR static bool isOpenCLBackendVerbose() TBAG_NOEXCEPT
{
#if defined(TBAG_OPENCL_BACKEND_DEBUGGING)
    return true;
#else
    return false;
#endif
}

TBAG_CONSTEXPR static bool isOpenCLBackendProfile() TBAG_NOEXCEPT
{
#if defined(TBAG_OPENCL_BACKEND_PROFILE)
    return true;
#else
    return false;
#endif
}

// ------------------
} // namespace __impl
// ------------------

bool isSupport() TBAG_NOEXCEPT
{
#if defined(USE_OPENCL)
    return true;
#else
    return false;
#endif
}


int getPlatformCount()
{
    int result = 0;
#if defined(USE_OPENCL)
    cl_uint num_platforms;
    cl_int code = ::clGetPlatformIDs(0, nullptr, &num_platforms);
    if (code != CL_SUCCESS) {
        tDLogE("getPlatformCount() OpenCL clGetPlatformIDs() error code: {}", code);
        return 0;
    }
    result = static_cast<int>(num_platforms);
#endif
    return result;
}

GpuPlatforms getPlatformList()
{
    GpuPlatforms result;
#if defined(USE_OPENCL)
    std::vector<cl_platform_id> platforms((std::size_t)getPlatformCount());
    cl_int code = ::clGetPlatformIDs((cl_uint)platforms.size(), platforms.data(), nullptr);
    if (code != CL_SUCCESS) {
        tDLogE("getPlatformList() OpenCL clGetPlatformIDs() error code: {}", code);
        return result;
    }
    for (auto & id : platforms) {
        result.emplace_back(GpuType::GT_OPENCL, (std::size_t)id);
    }
#endif
    return result;
}

GpuPlatformInfo getPlatformInfo(GpuPlatform const & platform)
{
    checkOpenCLGpuType(platform);
    GpuPlatformInfo info;
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

    info.profile    = get_platform_info((cl_platform_id)platform.PLATFORM, CL_PLATFORM_PROFILE);
    info.version    = get_platform_info((cl_platform_id)platform.PLATFORM, CL_PLATFORM_VERSION);
    info.name       = get_platform_info((cl_platform_id)platform.PLATFORM, CL_PLATFORM_NAME);
    info.vendor     = get_platform_info((cl_platform_id)platform.PLATFORM, CL_PLATFORM_VENDOR);
    info.extensions = get_platform_info((cl_platform_id)platform.PLATFORM, CL_PLATFORM_EXTENSIONS);
#endif
    return info;
}

int getDeviceCount(GpuPlatform const & platform)
{
    checkOpenCLGpuType(platform);
    int result = 0;
#if defined(USE_OPENCL)
    cl_uint num_devices;
    cl_int code = ::clGetDeviceIDs((cl_platform_id)platform.PLATFORM, CL_DEVICE_TYPE_ALL, 0, nullptr, &num_devices);
    if (code != CL_SUCCESS) {
        tDLogE("getDeviceCount() OpenCL clGetDeviceIDs() error code: {}", code);
        return 0;
    }
    result = static_cast<int>(num_devices);
#endif
    return result;
}

GpuDevices getDeviceList(GpuPlatform const & platform)
{
    checkOpenCLGpuType(platform);
    GpuDevices result;
#if defined(USE_OPENCL)
    std::vector<cl_device_id> devices((std::size_t)getDeviceCount(platform));
    cl_int code = ::clGetDeviceIDs((cl_platform_id)platform.PLATFORM, CL_DEVICE_TYPE_ALL,
                                   (cl_uint)devices.size(), devices.data(), nullptr);
    if (code != CL_SUCCESS) {
        tDLogE("getDeviceList() OpenCL clGetDeviceIDs() error code: {}", code);
        return result;
    }
    for (auto & id : devices) {
        result.emplace_back(platform, (std::size_t)id);
    }
#endif
    return result;
}

GpuDeviceInfo getDeviceInfo(GpuDevice const & device)
{
    checkOpenCLGpuType(device);
    GpuDeviceInfo info;
#if defined(USE_OPENCL)
    auto get_device_info_str = [](cl_device_id id, cl_device_info info) -> std::string {
        size_t value_size = 0;
        if (::clGetDeviceInfo(id, info, 0, nullptr, &value_size) == CL_SUCCESS) {
            std::vector<char> buffer(value_size, '\0');
            if (::clGetDeviceInfo(id, info, buffer.size(), buffer.data(), nullptr) == CL_SUCCESS) {
                return std::string(buffer.data());
            }
        }
        return std::string();
    };

    auto get_device_info_ulong = [](cl_device_id id, cl_device_info info) -> std::size_t {
        cl_ulong value;
        if (::clGetDeviceInfo(id, info, sizeof(cl_ulong), &value, nullptr) == CL_SUCCESS) {
            return static_cast<std::size_t>(value);
        }
        return 0;
    };

    info.name           = get_device_info_str  ((cl_device_id)device.DEVICE, CL_DEVICE_NAME);
    info.device_version = get_device_info_str  ((cl_device_id)device.DEVICE, CL_DEVICE_VERSION);
    info.driver_version = get_device_info_str  ((cl_device_id)device.DEVICE, CL_DRIVER_VERSION);
    info.global_memory  = get_device_info_ulong((cl_device_id)device.DEVICE, CL_DEVICE_GLOBAL_MEM_SIZE);
#endif
    return info;
}

SharedGpuContext createContext(GpuDevice const & device)
{
    checkOpenCLGpuType(device);
#if defined(USE_OPENCL)
    cl_int code;
    cl_context context = ::clCreateContext(nullptr, 1, (cl_device_id const *)&device.DEVICE, nullptr, nullptr, &code);
    if (code == CL_SUCCESS) {
        return SharedGpuContext(new OpenCLContext(device, (GpuId)context));
    } else {
        tDLogE("createContext() OpenCL clCreateContext() error code: {}", code);
    }
#endif
    return SharedGpuContext();
}

// -----------------------------
// OpenCLContext implementation.
// -----------------------------

OpenCLContext::OpenCLContext(GpuDevice const & d, GpuId c) : GpuContext(d, c)
{
    // EMPTY.
}

OpenCLContext::~OpenCLContext()
{
    // EMPTY.
}

// @formatter:off
bool OpenCLContext::isSupport() const TBAG_NOEXCEPT { return opencl::isSupport(); }
bool OpenCLContext::isHost   () const TBAG_NOEXCEPT { return true; }
bool OpenCLContext::isDevice () const TBAG_NOEXCEPT { return true; }
// @formatter:on

} // namespace opencl
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

