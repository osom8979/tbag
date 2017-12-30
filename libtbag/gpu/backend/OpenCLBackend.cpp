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
    checkType(platform.type);
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

    info.profile    = get_platform_info((cl_platform_id)platform.platform_id, CL_PLATFORM_PROFILE);
    info.version    = get_platform_info((cl_platform_id)platform.platform_id, CL_PLATFORM_VERSION);
    info.name       = get_platform_info((cl_platform_id)platform.platform_id, CL_PLATFORM_NAME);
    info.vendor     = get_platform_info((cl_platform_id)platform.platform_id, CL_PLATFORM_VENDOR);
    info.extensions = get_platform_info((cl_platform_id)platform.platform_id, CL_PLATFORM_EXTENSIONS);
#endif
    return info;
}

int OpenCLBackend::getDeviceCount(GpuPlatform const & platform) const
{
    checkType(platform.type);
    int result = 0;
#if defined(USE_OPENCL)
    cl_uint num_devices;
    cl_int code = ::clGetDeviceIDs((cl_platform_id)platform.platform_id, CL_DEVICE_TYPE_ALL, 0, nullptr, &num_devices);
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
    checkType(platform.type);
    GpuDevices result;
#if defined(USE_OPENCL)
    std::vector<cl_device_id> devices((std::size_t)getDeviceCount(platform));
    cl_int code = ::clGetDeviceIDs((cl_platform_id)platform.platform_id, CL_DEVICE_TYPE_ALL,
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
    checkType(device.type);
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

    info.name = get_device_info((cl_device_id)device.device_id, CL_DEVICE_NAME);
#endif
    return info;
}

GpuContext OpenCLBackend::createContext(GpuDevice const & device) const
{
    checkType(device.type);
    GpuContext result(device);
#if defined(USE_OPENCL)
    cl_int code;
    cl_context context = ::clCreateContext(nullptr, 1, (cl_device_id const *)&device.device_id, nullptr, nullptr, &code);
    if (code == CL_SUCCESS) {
        result.context_id = (std::size_t)context;
    } else {
        tDLogE("OpenCLBackend::createContext() OpenCL clCreateContext() error code: {}", code);
    }
#endif
    return result;
}

bool OpenCLBackend::releaseContext(GpuContext & context) const
{
    checkType(context.type);
#if defined(USE_OPENCL)
    cl_int code = ::clReleaseContext((cl_context)context.context_id);
    context.context_id = UNKNOWN_GPU_ID;
    if (code != CL_SUCCESS) {
        tDLogE("OpenCLBackend::releaseContext() OpenCL clReleaseContext() error code: {}", code);
        return false;
    }
#endif
    return true;
}

GpuQueue OpenCLBackend::createQueue(GpuContext const & context) const
{
    checkType(context.type);
    GpuQueue result(context);
#if defined(USE_OPENCL)
    cl_int code;
    cl_command_queue queue = ::clCreateCommandQueue((cl_context)context.context_id,
                                                    (cl_device_id)context.device_id,
                                                    (cl_command_queue_properties)0,
                                                    &code);
    if (code == CL_SUCCESS) {
        result.queue_id = (GpuId)queue;
    } else {
        tDLogE("OpenCLBackend::createQueue() OpenCL clCreateCommandQueue() error code: {}", code);
    }
#endif
    return result;
}

bool OpenCLBackend::releaseQueue(GpuQueue & queue) const
{
    checkType(queue.type);
    if (queue.isUnknownQueue()) {
        tDLogE("OpenCLBackend::releaseQueue() Illegal queue.");
        return false;
    }
#if defined(USE_OPENCL)
    cl_int code = ::clReleaseCommandQueue((cl_command_queue)queue.queue_id);
    if (code != CL_SUCCESS) {
        tDLogE("OpenCLBackend::releaseQueue() OpenCL clReleaseCommandQueue() error code: {}", code);
        return false;
    }
#endif
    return true;
}

GpuMemory OpenCLBackend::malloc(GpuContext const & context, std::size_t size) const
{
    checkType(context.type);
    GpuMemory result(context);
#if defined(USE_OPENCL)
    cl_int code;
    cl_mem memory = ::clCreateBuffer((cl_context)context.context_id, CL_MEM_READ_WRITE, size, nullptr, &code);
    if (code == CL_SUCCESS) {
        result.data = (void*)memory;
        result.size = size;
    } else {
        tDLogE("OpenCLBackend::malloc() OpenCL clCreateBuffer() error code: {}", code);
    }
#endif
    return result;
}

bool OpenCLBackend::free(GpuMemory & memory) const
{
    checkType(memory.type);
    if (memory.existsMemory() == false) {
        tDLogE("OpenCLBackend::free() Illegal memory.");
        return false;
    }
#if defined(USE_OPENCL)
    cl_int code = ::clReleaseMemObject((cl_mem)memory.data);
    memory.data = nullptr;
    memory.size = 0;
    if (code != CL_SUCCESS) {
        tDLogE("OpenCLBackend::free() OpenCL clReleaseMemObject() error code: {}", code);
        return false;
    }
#endif
    return true;
}

HostMemory OpenCLBackend::mallocHost(GpuContext const & context, std::size_t size, HostMemoryFlag flag) const
{
    checkType(context.type);
    HostMemory result(context);
    return result;
}

bool OpenCLBackend::freeHost(HostMemory & memory) const
{
    checkType(memory.type);
    return true;
}

bool OpenCLBackend::enqueueWrite(GpuQueue & queue, GpuMemory & gpu_mem, HostMemory const & host_mem, std::size_t size) const
{
    checkType(queue.type);
    checkType(gpu_mem.type);
    checkType(host_mem.type);
    return true;
}

bool OpenCLBackend::enqueueRead(GpuQueue & queue, GpuMemory const & gpu_mem, HostMemory & host_mem, std::size_t size) const
{
    checkType(queue.type);
    checkType(gpu_mem.type);
    checkType(host_mem.type);
    return true;
}

} // namespace backend
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

