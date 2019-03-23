/**
 * @file   ClDevice.cpp
 * @brief  ClDevice class implementation.
 * @author zer0
 * @date   2019-02-22
 */

#include <libtbag/parallel/cl/ClDevice.hpp>
#include <libtbag/log/Log.hpp>

#if defined(USE_OPENCL)
# if defined(TBAG_PLATFORM_MACOS)
#  include <OpenCL/cl.h>
# else
#  include <CL/cl.h>
# endif
#else
# include <libtbag/dummy/Cl.hpp>
using namespace libtbag::dummy::cl;
#endif

#define TBAG_OPENCL_BACKEND_PROFILE

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace parallel {
namespace cl       {

TBAG_CONSTEXPR static bool isOpenCLBackendProfile() TBAG_NOEXCEPT
{
#if defined(TBAG_OPENCL_BACKEND_PROFILE)
    return true;
#else
    return false;
#endif
}

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
    cl_uint num_platforms;
    cl_int code = clGetPlatformIDs(0, nullptr, &num_platforms);
    if (code != CL_SUCCESS) {
        tDLogE("getPlatformCount() OpenCL clGetPlatformIDs() error code: {}", code);
        return 0;
    }
    result = static_cast<int>(num_platforms);
    return result;
}

ParallelPlatformIds getPlatformList()
{
    ParallelPlatformIds result;
    std::vector<cl_platform_id> platforms((std::size_t)getPlatformCount());
    cl_int code = clGetPlatformIDs((cl_uint)platforms.size(), platforms.data(), nullptr);
    if (code != CL_SUCCESS) {
        tDLogE("getPlatformList() OpenCL clGetPlatformIDs() error code: {}", code);
        return result;
    }
    for (auto & id : platforms) {
        result.emplace_back((ParallelPlatformId)id);
    }
    return result;
}

ParallelPlatformInfo getPlatformInfo(ParallelPlatformId platform)
{
    ParallelPlatformInfo info;
    auto get_platform_info = [](cl_platform_id id, cl_platform_info info) -> std::string {
        size_t value_size = 0;
        if (clGetPlatformInfo(id, info, 0, nullptr, &value_size) == CL_SUCCESS) {
            std::vector<char> buffer(value_size, '\0');
            if (clGetPlatformInfo(id, info, buffer.size(), buffer.data(), nullptr) == CL_SUCCESS) {
                return std::string(buffer.data());
            }
        }
        return std::string();
    };

    info.profile    = get_platform_info((cl_platform_id)platform, CL_PLATFORM_PROFILE);
    info.version    = get_platform_info((cl_platform_id)platform, CL_PLATFORM_VERSION);
    info.name       = get_platform_info((cl_platform_id)platform, CL_PLATFORM_NAME);
    info.vendor     = get_platform_info((cl_platform_id)platform, CL_PLATFORM_VENDOR);
    info.extensions = get_platform_info((cl_platform_id)platform, CL_PLATFORM_EXTENSIONS);
    return info;
}

int getDeviceCount(ParallelPlatformId platform)
{
    int result = 0;
    cl_uint num_devices;
    cl_int code = clGetDeviceIDs((cl_platform_id)platform, CL_DEVICE_TYPE_ALL, 0, nullptr, &num_devices);
    if (code != CL_SUCCESS) {
        tDLogE("getDeviceCount() OpenCL clGetDeviceIDs() error code: {}", code);
        return 0;
    }
    result = static_cast<int>(num_devices);
    return result;
}

ParallelDeviceIds getDeviceList(ParallelPlatformId platform)
{
    ParallelDeviceIds result;
    std::vector<cl_device_id> devices((std::size_t)getDeviceCount(platform));
    cl_int code = clGetDeviceIDs((cl_platform_id)platform, CL_DEVICE_TYPE_ALL,
                                 (cl_uint)devices.size(), devices.data(), nullptr);
    if (code != CL_SUCCESS) {
        tDLogE("getDeviceList() OpenCL clGetDeviceIDs() error code: {}", code);
        return result;
    }
    for (auto & id : devices) {
        result.emplace_back((ParallelDeviceId)id);
    }
    return result;
}

ParallelDeviceInfo getDeviceInfo(ParallelDeviceId device)
{
    ParallelDeviceInfo info;
    auto get_device_info_str = [](cl_device_id id, cl_device_info info) -> std::string {
        size_t value_size = 0;
        if (clGetDeviceInfo(id, info, 0, nullptr, &value_size) == CL_SUCCESS) {
            std::vector<char> buffer(value_size, '\0');
            if (clGetDeviceInfo(id, info, buffer.size(), buffer.data(), nullptr) == CL_SUCCESS) {
                return std::string(buffer.data());
            }
        }
        return std::string();
    };

    auto get_device_info_ulong = [](cl_device_id id, cl_device_info info) -> std::size_t {
        cl_ulong value;
        if (clGetDeviceInfo(id, info, sizeof(cl_ulong), &value, nullptr) == CL_SUCCESS) {
            return static_cast<std::size_t>(value);
        }
        return 0;
    };

    info.name           = get_device_info_str  ((cl_device_id)device, CL_DEVICE_NAME);
    info.device_version = get_device_info_str  ((cl_device_id)device, CL_DEVICE_VERSION);
    info.driver_version = get_device_info_str  ((cl_device_id)device, CL_DRIVER_VERSION);
    info.global_memory  = get_device_info_ulong((cl_device_id)device, CL_DEVICE_GLOBAL_MEM_SIZE);
    return info;
}

ParallelContextId createContext(ParallelDeviceId device)
{
    cl_int code;
    cl_context context = clCreateContext(nullptr, 1, (cl_device_id const *)&device, nullptr, nullptr, &code);
    if (code != CL_SUCCESS) {
        tDLogE("createContext() OpenCL clCreateContext() error code: {}", code);
        return UNKNOWN_ID;
    }
    return (ParallelContextId)context;
}

bool deleteContext(ParallelContextId context)
{
    cl_int code = clReleaseContext((cl_context)context);
    if (code != CL_SUCCESS) {
        tDLogE("deleteContext() OpenCL clReleaseContext() error code: {}", code);
        return false;
    }
    return true;
}

} // namespace cl
} // namespace parallel

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

