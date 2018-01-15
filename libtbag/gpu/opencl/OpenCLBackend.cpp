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
#else
# include <libtbag/dummy/Cl.hpp>
using namespace libtbag::dummy::cl;
#endif

#define TBAG_OPENCL_BACKEND_PROFILE

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu    {
namespace opencl {

// ---------------
namespace __impl {
// ---------------

TBAG_CONSTEXPR static bool isOpenCLBackendProfile() TBAG_NOEXCEPT
{
#if defined(TBAG_OPENCL_BACKEND_PROFILE)
    return true;
#else
    return false;
#endif
}

template <typename KernelType>
static bool setKernelMemories(KernelType kernel, GpuMemories const & mems)
{
    if (mems.empty()) {
        return false;
    }

    auto const SIZE = (cl_uint)mems.size();
    for (cl_uint i = 0; i < SIZE; ++i) {
        cl_int code = clSetKernelArg(kernel, i, mems[i].size, mems[i].data);
        if (code != CL_SUCCESS) {
            tDLogE("setKernelMemories() OpenCL clSetKernelArg() error code: {}", code);
            return false;
        }
    }
    return true;
}

template <typename KernelType, typename ... Args>
static bool setKernelArguments(KernelType kernel, Args && ... mems)
{
    return setKernelMemories(kernel, {std::forward<Args>(mems) ...});
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
    cl_uint num_platforms;
    cl_int code = clGetPlatformIDs(0, nullptr, &num_platforms);
    if (code != CL_SUCCESS) {
        tDLogE("getPlatformCount() OpenCL clGetPlatformIDs() error code: {}", code);
        return 0;
    }
    result = static_cast<int>(num_platforms);
    return result;
}

GpuPlatforms getPlatformList()
{
    GpuPlatforms result;
    std::vector<cl_platform_id> platforms((std::size_t)getPlatformCount());
    cl_int code = clGetPlatformIDs((cl_uint)platforms.size(), platforms.data(), nullptr);
    if (code != CL_SUCCESS) {
        tDLogE("getPlatformList() OpenCL clGetPlatformIDs() error code: {}", code);
        return result;
    }
    for (auto & id : platforms) {
        result.emplace_back(GpuType::GT_OPENCL, (std::size_t)id);
    }
    return result;
}

GpuPlatformInfo getPlatformInfo(GpuPlatform const & platform)
{
    checkOpenCLGpuType(platform);
    GpuPlatformInfo info;
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

    info.profile    = get_platform_info((cl_platform_id)platform.PLATFORM, CL_PLATFORM_PROFILE);
    info.version    = get_platform_info((cl_platform_id)platform.PLATFORM, CL_PLATFORM_VERSION);
    info.name       = get_platform_info((cl_platform_id)platform.PLATFORM, CL_PLATFORM_NAME);
    info.vendor     = get_platform_info((cl_platform_id)platform.PLATFORM, CL_PLATFORM_VENDOR);
    info.extensions = get_platform_info((cl_platform_id)platform.PLATFORM, CL_PLATFORM_EXTENSIONS);
    return info;
}

int getDeviceCount(GpuPlatform const & platform)
{
    checkOpenCLGpuType(platform);
    int result = 0;
    cl_uint num_devices;
    cl_int code = clGetDeviceIDs((cl_platform_id)platform.PLATFORM, CL_DEVICE_TYPE_ALL, 0, nullptr, &num_devices);
    if (code != CL_SUCCESS) {
        tDLogE("getDeviceCount() OpenCL clGetDeviceIDs() error code: {}", code);
        return 0;
    }
    result = static_cast<int>(num_devices);
    return result;
}

GpuDevices getDeviceList(GpuPlatform const & platform)
{
    checkOpenCLGpuType(platform);
    GpuDevices result;
    std::vector<cl_device_id> devices((std::size_t)opencl::getDeviceCount(platform));
    cl_int code = clGetDeviceIDs((cl_platform_id)platform.PLATFORM, CL_DEVICE_TYPE_ALL,
                                 (cl_uint)devices.size(), devices.data(), nullptr);
    if (code != CL_SUCCESS) {
        tDLogE("getDeviceList() OpenCL clGetDeviceIDs() error code: {}", code);
        return result;
    }
    for (auto & id : devices) {
        result.emplace_back(platform, (std::size_t)id);
    }
    return result;
}

GpuDeviceInfo getDeviceInfo(GpuDevice const & device)
{
    checkOpenCLGpuType(device);
    GpuDeviceInfo info;
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

    info.name           = get_device_info_str  ((cl_device_id)device.DEVICE, CL_DEVICE_NAME);
    info.device_version = get_device_info_str  ((cl_device_id)device.DEVICE, CL_DEVICE_VERSION);
    info.driver_version = get_device_info_str  ((cl_device_id)device.DEVICE, CL_DRIVER_VERSION);
    info.global_memory  = get_device_info_ulong((cl_device_id)device.DEVICE, CL_DEVICE_GLOBAL_MEM_SIZE);
    return info;
}

SharedGpuContext createContext(GpuDevice const & device)
{
    checkOpenCLGpuType(device);
    cl_int code;
    cl_context context = clCreateContext(nullptr, 1, (cl_device_id const *)&device.DEVICE, nullptr, nullptr, &code);
    if (code == CL_SUCCESS) {
        return SharedGpuContext(new OpenCLContext(device, (GpuId)context), [](OpenCLContext * context){
            cl_int code = clReleaseContext((cl_context)context->CONTEXT);
            if (code != CL_SUCCESS) {
                tDLogE("createContext() OpenCL clReleaseContext() error code: {}", code);
            }
        });
    } else {
        tDLogE("createContext() OpenCL clCreateContext() error code: {}", code);
    }
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

bool OpenCLContext::_write(GpuStream & stream, GpuMemory & gpu_mem, HostMemory const & host_mem,
                           std::size_t size, bool blocking, GpuEvent * event) const
{
    cl_int code = clEnqueueWriteBuffer(stream.castId<cl_command_queue>(), (cl_mem)gpu_mem.data,
                                       (blocking ? CL_TRUE : CL_FALSE),
                                       0, host_mem.size, host_mem.data, 0, nullptr,
                                       (cl_event*)(event == nullptr ? nullptr : &event->start));
    if (code == CL_SUCCESS) {
        return true;
    } else {
        tDLogE("OpenCLContext::_write({}) OpenCL clEnqueueWriteBuffer() error code: {}", blocking, code);
    }
    return false;
}

bool OpenCLContext::_read(GpuStream & stream, GpuMemory const & gpu_mem, HostMemory & host_mem,
                          std::size_t size, bool blocking, GpuEvent * event) const
{
    cl_int code = clEnqueueReadBuffer(stream.castId<cl_command_queue>(), (cl_mem)gpu_mem.data,
                                      (blocking ? CL_TRUE : CL_FALSE),
                                      0, host_mem.size, host_mem.data, 0, nullptr,
                                      (cl_event*)(event == nullptr ? nullptr : &event->start));
    if (code == CL_SUCCESS) {
        return true;
    } else {
        tDLogE("OpenCLContext::_read({}) OpenCL clEnqueueReadBuffer() error code: {}", blocking, code);
    }
    return false;
}

// @formatter:off
bool OpenCLContext::isSupport() const TBAG_NOEXCEPT { return opencl::isSupport(); }
bool OpenCLContext::isHost   () const TBAG_NOEXCEPT { return true; }
bool OpenCLContext::isDevice () const TBAG_NOEXCEPT { return true; }
bool OpenCLContext::isStream () const TBAG_NOEXCEPT { return true; }
// @formatter:on

bool OpenCLContext::createStream(GpuStream & stream) const
{
    if (stream.getContextPtr() != this) {
        tDLogE("OpenCLContext::createStream() Illegal stream argument.");
        return false;
    }

    cl_int code;
    cl_command_queue_properties properties = 0;
    if (__impl::isOpenCLBackendProfile()) {
        properties |= CL_QUEUE_PROFILING_ENABLE;
    }
    cl_command_queue native_stream = clCreateCommandQueue((cl_context)CONTEXT,
                                                          (cl_device_id)DEVICE,
                                                          properties, &code);
    if (code == CL_SUCCESS) {
        stream.setId(native_stream);
        return true;
    } else {
        tDLogE("OpenCLContext::createStream() OpenCL clCreateCommandQueue() error code: {}", code);
    }
    return false;
}

bool OpenCLContext::releaseStream(GpuStream & stream) const
{
    if (stream.validate(this) == false) {
        tDLogE("OpenCLContext::releaseStream() Illegal stream argument.");
        return false;
    }

    cl_int code = clReleaseCommandQueue(stream.castId<cl_command_queue>());
    stream.clearId();
    if (code == CL_SUCCESS) {
        return true;
    } else {
        tDLogE("OpenCLContext::releaseStream() OpenCL clReleaseCommandQueue() error code: {}", code);
    }
    return false;
}

bool OpenCLContext::createEvent(GpuStream const & stream, GpuEvent & event) const
{
    if (stream.validate(this) == false) {
        tDLogE("OpenCLContext::createEvent() Illegal stream argument.");
        return false;
    }
    if (event.CONTEXT != this) {
        tDLogE("OpenCLContext::createEvent() Illegal event argument.");
        return false;
    }
    event.start = 0;
    event.stop  = 0;
    return true;
}

bool OpenCLContext::syncEvent(GpuEvent const & event) const
{
    if (event.CONTEXT != this || event.exists() == false) {
        tDLogE("OpenCLContext::syncEvent() Illegal event argument.");
        return false;
    }

    cl_int code = clWaitForEvents(1, (cl_event const *)&event.start);
    if (code == CL_SUCCESS) {
        return true;
    } else {
        tDLogE("OpenCLContext::syncEvent() OpenCL clWaitForEvents() error code: {}", code);
    }
    return false;
}

bool OpenCLContext::elapsedEvent(GpuEvent & event, float * millisec) const
{
    if (event.CONTEXT != this || event.exists() == false) {
        tDLogE("OpenCLContext::elapsedEvent() Illegal event argument.");
        return false;
    }

    cl_ulong start_nano, stop_nano;
    cl_int start_code = clGetEventProfilingInfo((cl_event)event.start, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start_nano, nullptr);
    cl_int  stop_code = clGetEventProfilingInfo((cl_event)event.start, CL_PROFILING_COMMAND_END  , sizeof(cl_ulong),  &stop_nano, nullptr);
    if (start_code == CL_SUCCESS && stop_code == CL_SUCCESS) {
        if (millisec != nullptr) {
            *millisec = (stop_nano - start_nano) * 1.0e-6f;
        }
        return true;
    } else {
        tDLogE("OpenCLContext::elapsedEvent() OpenCL clGetEventProfilingInfo() error code: start({}), stop({})",
               start_code, stop_code);
    }
    return false;
}

bool OpenCLContext::releaseEvent(GpuEvent & event) const
{
    if (event.CONTEXT != this || event.exists() == false) {
        tDLogE("OpenCLContext::releaseEvent() Illegal event argument.");
        return false;
    }

    event.start = UNKNOWN_ID;
    event.stop  = UNKNOWN_ID;
    return true;
}

bool OpenCLContext::createProgram(std::string const & source, GpuProgram & program) const
{
    if (source.empty()) {
        tDLogE("OpenCLContext::createProgram() Empty source.");
        return false;
    }
    if (program.CONTEXT != this) {
        tDLogE("OpenCLContext::createProgram() Illegal program argument.");
        return false;
    }

    char const * c_source = source.c_str();
    cl_int code;
    cl_program native_program = clCreateProgramWithSource((cl_context)CONTEXT, 1, &c_source, nullptr, &code);
    if (code == CL_SUCCESS) {
        program.program = (GpuId)native_program;
        return true;
    } else {
        tDLogE("OpenCLContext::createProgram() OpenCL clCreateProgramWithSource() error code: {}", code);
    }
    return false;
}

bool OpenCLContext::buildProgram(GpuProgram & program) const
{
    if (program.CONTEXT != this || program.exists() == false) {
        tDLogE("OpenCLContext::buildProgram() Illegal program argument.");
        return false;
    }

    cl_int code = clBuildProgram((cl_program)program.program, 1, (cl_device_id const *)&DEVICE,
                                 nullptr, nullptr, nullptr);
    if (code == CL_SUCCESS) {
        return true;
    } else {
        tDLogE("OpenCLContext::buildProgram() OpenCL clBuildProgram() error code: {}", code);
    }
    return false;
}

bool OpenCLContext::releaseProgram(GpuProgram & program) const
{
    if (program.CONTEXT != this || program.exists() == false) {
        tDLogE("OpenCLContext::releaseProgram() Illegal program argument.");
        return false;
    }

    cl_int code = clReleaseProgram((cl_program)program.program);
    program.program = UNKNOWN_ID;
    if (code == CL_SUCCESS) {
        return true;
    } else {
        tDLogE("OpenCLContext::releaseProgram() OpenCL clReleaseProgram() error code: {}", code);
    }
    return false;
}

bool OpenCLContext::createKernel(GpuProgram const & program, std::string const & kernel_symbol, GpuKernel & kernel) const
{
    if (program.CONTEXT != this || program.exists() == false) {
        tDLogE("OpenCLContext::createKernel() Illegal program arguments.");
        return false;
    }
    if (kernel_symbol.empty()) {
        tDLogE("OpenCLContext::createKernel() Empty kernel symbol.");
        return false;
    }
    if (kernel.CONTEXT != this) {
        tDLogE("OpenCLContext::createKernel() Illegal kernel arguments.");
        return false;
    }

    cl_int code;
    cl_kernel native_kernel = clCreateKernel((cl_program)program.program, kernel_symbol.c_str(), &code);
    if (code == CL_SUCCESS) {
        kernel.kernel = (GpuId)native_kernel;
        return true;
    } else {
        tDLogE("OpenCLContext::createKernel() OpenCL clCreateKernel() error code: {}", code);
    }
    return false;
}

bool OpenCLContext::releaseKernel(GpuKernel & kernel) const
{
    if (kernel.CONTEXT != this || kernel.exists() == false) {
        tDLogE("OpenCLContext::releaseKernel() Illegal kernel context.");
        return false;
    }

    cl_int code = clReleaseKernel((cl_kernel)kernel.kernel);
    kernel.kernel = UNKNOWN_ID;
    if (code == CL_SUCCESS) {
        return true;
    } else {
        tDLogE("OpenCLContext::releaseKernel() OpenCL clReleaseKernel() error code: {}", code);
    }
    return false;
}

bool OpenCLContext::malloc(GpuMemory & memory, std::size_t size) const
{
    if (memory.CONTEXT != this) {
        tDLogE("OpenCLContext::malloc() Illegal memory arguments.");
        return false;
    }

    cl_int code;
    cl_mem native_memory = clCreateBuffer((cl_context)CONTEXT, CL_MEM_READ_WRITE, size, nullptr, &code);
    if (code == CL_SUCCESS) {
        memory.data     = (void*)native_memory;
        memory.capacity = size;
        memory.size     = size;
        tDLogIfD(isGpuVerbose(), "OpenCLContext::malloc({}) OpenCL clCreateBuffer() MEM:{} CAP:{} SIZE:{}",
                 size, memory.data, memory.capacity, memory.size);
        return true;
    } else {
        tDLogE("OpenCLContext::malloc({}) OpenCL clCreateBuffer() error code: {}", size, code);
    }
    return false;
}

bool OpenCLContext::free(GpuMemory & memory) const
{
    if (memory.CONTEXT != this || memory.exists() == false) {
        tDLogE("OpenCLContext::free() Illegal memory.");
        return false;
    }

    tDLogIfD(isGpuVerbose(), "OpenCLContext::free() OpenCL clReleaseMemObject() MEM:{} SIZE:{}",
             memory.data, memory.size);

    cl_int code = clReleaseMemObject((cl_mem)memory.data);
    memory.data     = nullptr;
    memory.capacity = 0;
    memory.size     = 0;
    if (code == CL_SUCCESS) {
        return true;
    } else {
        tDLogE("OpenCLContext::free() OpenCL clReleaseMemObject() error code: {}", code);
    }
    return false;
}

bool OpenCLContext::write(GpuStream & stream, GpuMemory & gpu_mem, HostMemory const & host_mem, std::size_t size, GpuEvent * event) const
{
    if (checkMemory(stream, gpu_mem, host_mem, size) == false) {
        tDLogE("OpenCLContext::write() Illegal arguments.");
        return false;
    }
    return _write(stream, gpu_mem, host_mem, size, true, event);
}

bool OpenCLContext::read(GpuStream & stream, GpuMemory const & gpu_mem, HostMemory & host_mem, std::size_t size, GpuEvent * event) const
{
    if (checkMemory(stream, gpu_mem, host_mem, size) == false) {
        tDLogE("OpenCLContext::read() Illegal arguments.");
        return false;
    }
    return _read(stream, gpu_mem, host_mem, size, true, event);
}

bool OpenCLContext::writeAsync(GpuStream & stream, GpuMemory & gpu_mem, HostMemory const & host_mem, std::size_t size, GpuEvent * event) const
{
    if (checkMemory(stream, gpu_mem, host_mem, size) == false) {
        tDLogE("OpenCLContext::writeAsync() Illegal arguments.");
        return false;
    }
    return _write(stream, gpu_mem, host_mem, size, false, event);
}

bool OpenCLContext::readAsync(GpuStream & stream, GpuMemory const & gpu_mem, HostMemory & host_mem, std::size_t size, GpuEvent * event) const
{
    if (checkMemory(stream, gpu_mem, host_mem, size) == false) {
        tDLogE("OpenCLContext::readAsync() Illegal arguments.");
        return false;
    }
    return _read(stream, gpu_mem, host_mem, size, false, event);
}

bool OpenCLContext::flush(GpuStream & stream) const
{
    if (stream.validate(this) == false) {
        tDLogE("OpenCLContext::flush() Illegal stream argument.");
        return false;
    }

    cl_int code = clFlush(stream.castId<cl_command_queue>());
    if (code == CL_SUCCESS) {
        return true;
    } else {
        tDLogE("OpenCLContext::flush() OpenCL clFlush() error code: {}", code);
    }
    return false;
}

bool OpenCLContext::finish(GpuStream & stream) const
{
    if (stream.validate(this) == false) {
        tDLogE("OpenCLContext::finish() Illegal stream argument.");
        return false;
    }

    cl_int code = clFinish(stream.castId<cl_command_queue>());
    if (code == CL_SUCCESS) {
        return true;
    } else {
        tDLogE("OpenCLContext::finish() OpenCL clFinish() error code: {}", code);
    }
    return false;
}

} // namespace opencl
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

