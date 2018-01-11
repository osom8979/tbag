/**
 * @file   OpenCLBackend.cpp
 * @brief  OpenCLBackend class implementation.
 * @author zer0
 * @date   2017-12-17
 */

#include <libtbag/gpu/backend/OpenCLBackend.hpp>
#include <libtbag/gpu/opencl/OpenCLRaw.hpp>
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

//#define TBAG_OPENCL_BACKEND_DEBUGGING
#define TBAG_OPENCL_BACKEND_PROFILE

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace backend {

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

static bool write(GpuStream & stream, GpuMemory & gpu_mem, HostMemory const & host_mem,
           std::size_t size, bool blocking, GpuEvent * event = nullptr)
{
#if defined(USE_OPENCL)
    cl_int code = ::clEnqueueWriteBuffer((cl_command_queue)stream.stream_id, (cl_mem)gpu_mem.data,
                                         (blocking ? CL_TRUE : CL_FALSE),
                                         0, host_mem.size, host_mem.data, 0, nullptr,
                                         (cl_event*)(event == nullptr ? nullptr : &event->start));
    if (code == CL_SUCCESS) {
        return true;
    } else {
        tDLogE("OpenCLBackend/__impl::write({}) OpenCL clEnqueueWriteBuffer() error code: {}", blocking, code);
    }
#endif
    return false;
}

static bool read(GpuStream & stream, GpuMemory const & gpu_mem, HostMemory & host_mem,
          std::size_t size, bool blocking, GpuEvent * event = nullptr)
{
#if defined(USE_OPENCL)
    cl_int code = ::clEnqueueReadBuffer((cl_command_queue)stream.stream_id, (cl_mem)gpu_mem.data,
                                        (blocking ? CL_TRUE : CL_FALSE),
                                        0, host_mem.size, host_mem.data, 0, nullptr,
                                        (cl_event*)(event == nullptr ? nullptr : &event->start));
    if (code == CL_SUCCESS) {
        return true;
    } else {
        tDLogE("OpenCLBackend/__impl::read({}) OpenCL clEnqueueReadBuffer() error code: {}", blocking, code);
    }
#endif
    return false;
}

template <typename KernelType>
static bool setKernelMemories(KernelType kernel, GpuMemories const & mems)
{
    if (mems.empty()) {
        return false;
    }

#if defined(USE_OPENCL)
    cl_uint const SIZE = (cl_uint)mems.size();
    for (cl_uint i = 0; i < SIZE; ++i) {
        cl_int code = ::clSetKernelArg(kernel, i, mems[i].size, mems[i].data);
        if (code != CL_SUCCESS) {
            tDLogE("OpenCLBackend/__impl::setKernelMemories() OpenCL clSetKernelArg() error code: {}", code);
            return false;
        }
    }
    return true;
#else
    return false;
#endif
}

template <typename KernelType, typename ... Args>
static bool setKernelArguments(KernelType kernel, Args && ... mems)
{
    return setKernelMemories(kernel, {std::forward<Args>(mems) ...});
}

// ------------------
} // namespace __impl
// ------------------

OpenCLBackend::OpenCLBackend()
{
    // EMPTY.
}

OpenCLBackend::~OpenCLBackend()
{
    // EMPTY.
}

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

    info.name           = get_device_info_str  ((cl_device_id)device.device_id, CL_DEVICE_NAME);
    info.device_version = get_device_info_str  ((cl_device_id)device.device_id, CL_DEVICE_VERSION);
    info.driver_version = get_device_info_str  ((cl_device_id)device.device_id, CL_DRIVER_VERSION);
    info.global_memory  = get_device_info_ulong((cl_device_id)device.device_id, CL_DEVICE_GLOBAL_MEM_SIZE);
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
    context.context_id = id::UNKNOWN_ID;
    if (code != CL_SUCCESS) {
        tDLogE("OpenCLBackend::releaseContext() OpenCL clReleaseContext() error code: {}", code);
        return false;
    }
#endif
    return true;
}

bool OpenCLBackend::isStreamSupported(GpuDevice const & device) const
{
    return true;
}

GpuStream OpenCLBackend::createStream(GpuContext const & context) const
{
    checkType(context.type);
    GpuStream result(context);
#if defined(USE_OPENCL)
    cl_int code;
    cl_command_queue_properties properties = 0;
    if (__impl::isOpenCLBackendProfile()) {
        properties |= CL_QUEUE_PROFILING_ENABLE;
    }
    cl_command_queue stream = ::clCreateCommandQueue((cl_context)context.context_id,
                                                    (cl_device_id)context.device_id,
                                                    properties, &code);
    if (code == CL_SUCCESS) {
        result.stream_id = (id::Id)stream;
    } else {
        tDLogE("OpenCLBackend::createStream() OpenCL clCreateCommandQueue() error code: {}", code);
    }
#endif
    return result;
}

bool OpenCLBackend::releaseStream(GpuStream & stream) const
{
    checkType(stream.type);
    if (stream.isUnknownStream()) {
        tDLogE("OpenCLBackend::releaseStream() Illegal stream.");
        return false;
    }
#if defined(USE_OPENCL)
    cl_int code = ::clReleaseCommandQueue((cl_command_queue)stream.stream_id);
    stream.stream_id = id::UNKNOWN_ID;
    if (code != CL_SUCCESS) {
        tDLogE("OpenCLBackend::releaseStream() OpenCL clReleaseCommandQueue() error code: {}", code);
        return false;
    }
#endif
    return true;
}

GpuEvent OpenCLBackend::createEvent(GpuStream const & stream) const
{
    GpuEvent result(stream);
    result.start = 0;
    result.stop  = 0;
    return result;
}

bool OpenCLBackend::syncEvent(GpuEvent const & event) const
{
#if defined(USE_OPENCL)
    cl_int code = ::clWaitForEvents(1, (cl_event const *)&event.start);
    if (code == CL_SUCCESS) {
        return true;
    } else {
        tDLogE("OpenCLBackend::syncEvent() OpenCL clWaitForEvents() error code: {}", code);
    }
#endif
    return false;
}

bool OpenCLBackend::elapsedEvent(GpuEvent & event, float * millisec) const
{
#if defined(USE_OPENCL)
    cl_ulong start_nano, stop_nano;
    cl_int start_code = ::clGetEventProfilingInfo((cl_event)event.start, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start_nano, nullptr);
    cl_int  stop_code = ::clGetEventProfilingInfo((cl_event)event.start, CL_PROFILING_COMMAND_END  , sizeof(cl_ulong),  &stop_nano, nullptr);
    if (start_code == CL_SUCCESS && stop_code == CL_SUCCESS) {
        if (millisec != nullptr) {
            *millisec = (stop_nano - start_nano) * 1.0e-6f;
        }
        return true;
    } else {
        tDLogE("OpenCLBackend::elapsedEvent() OpenCL clGetEventProfilingInfo() error code: start({}), stop({})",
               start_code, stop_code);
    }
#endif
    return false;
}

bool OpenCLBackend::releaseEvent(GpuEvent & event) const
{
    event.start = id::UNKNOWN_ID;
    event.stop  = id::UNKNOWN_ID;
    return true;
}

GpuMemory OpenCLBackend::malloc(GpuContext const & context, std::size_t size) const
{
    checkType(context.type);
    GpuMemory result(context);
#if defined(USE_OPENCL)
    cl_int code;
    cl_mem memory = ::clCreateBuffer((cl_context)context.context_id, CL_MEM_READ_WRITE, size, nullptr, &code);
    if (code != CL_SUCCESS) {
        tDLogE("OpenCLBackend::malloc() OpenCL clCreateBuffer() error code: {}", code);
        return result;
    }
    result.data = (void*)memory;
    result.capacity = size;
    result.size = size;
    tDLogIfD(__impl::isOpenCLBackendVerbose(), "OpenCLBackend::malloc() OpenCL clCreateBuffer() MEM:{} SIZE:{}",
             result.data, result.size);
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
    tDLogIfD(__impl::isOpenCLBackendVerbose(), "OpenCLBackend::free() OpenCL clReleaseMemObject() MEM:{} SIZE:{}",
             memory.data, memory.size);
    cl_int code;
    code = ::clReleaseMemObject((cl_mem)memory.data);
    memory.data = nullptr;
    memory.capacity = 0;
    memory.size = 0;
    if (code == CL_SUCCESS) {
        return true;
    } else {
        tDLogE("OpenCLBackend::free() OpenCL clReleaseMemObject() error code: {}", code);
    }
#endif
    return false;
}

HostMemory OpenCLBackend::mallocHost(GpuContext const & context, std::size_t size, HostMemoryFlag flag) const
{
    checkType(context.type);
    HostMemory result(context);
    if (HostMemoryFlag::HMF_DEFAULT != flag) {
        return result;
    }

    result.data = ::malloc(size);
    result.capacity = size;
    result.size = size;
    result.flag = flag;
    tDLogIfD(__impl::isOpenCLBackendVerbose(), "OpenCLBackend::mallocHost() OpenCL clCreateBuffer() MEM:{} SIZE:{}",
             result.data, result.size);
    return result;
}

bool OpenCLBackend::freeHost(HostMemory & memory) const
{
    checkType(memory.type);
    if (memory.existsMemory() == false) {
        tDLogE("OpenCLBackend::freeHost() Illegal memory.");
        return false;
    }

    tDLogIfD(__impl::isOpenCLBackendVerbose(), "OpenCLBackend::freeHost() OpenCL clReleaseMemObject() MEM:{} SIZE:{}",
             memory.data, memory.size);
    ::free(memory.data);
    memory.data = nullptr;
    memory.capacity = 0;
    memory.size = 0;
    memory.flag = HostMemoryFlag::HMF_UNINITIALIZED;
    return true;
}

bool OpenCLBackend::write(GpuStream & stream, GpuMemory & gpu_mem, HostMemory const & host_mem, std::size_t size, GpuEvent * event) const
{
    checkType(stream.type);
    checkType(gpu_mem.type);
    checkType(host_mem.type);
    if (gpu_mem.size < size || host_mem.size < size) {
        tDLogE("OpenCLBackend::write() Invalid size error: gpu({}), host({}), size({})",
               gpu_mem.size, host_mem.size, size);
        return false;
    }
    return __impl::write(stream, gpu_mem, host_mem, size, true, event);
}

bool OpenCLBackend::read(GpuStream & stream, GpuMemory const & gpu_mem, HostMemory & host_mem, std::size_t size, GpuEvent * event) const
{
    checkType(stream.type);
    checkType(gpu_mem.type);
    checkType(host_mem.type);
    if (gpu_mem.size < size || host_mem.size < size) {
        tDLogE("OpenCLBackend::read() Invalid size error: gpu({}), host({}), size({})",
               gpu_mem.size, host_mem.size, size);
        return false;
    }
    return __impl::read(stream, gpu_mem, host_mem, size, true, event);
}

bool OpenCLBackend::enqueueWrite(GpuStream & stream, GpuMemory & gpu_mem, HostMemory const & host_mem, std::size_t size, GpuEvent * event) const
{
    checkType(stream.type);
    checkType(gpu_mem.type);
    checkType(host_mem.type);
    if (gpu_mem.size < size || host_mem.size < size) {
        tDLogE("OpenCLBackend::enqueueWrite() Invalid size error: gpu({}), host({}), size({})",
               gpu_mem.size, host_mem.size, size);
        return false;
    }
    return __impl::write(stream, gpu_mem, host_mem, size, false, event);
}

bool OpenCLBackend::enqueueRead(GpuStream & stream, GpuMemory const & gpu_mem, HostMemory & host_mem, std::size_t size, GpuEvent * event) const
{
    checkType(stream.type);
    checkType(gpu_mem.type);
    checkType(host_mem.type);
    if (gpu_mem.size < size || host_mem.size < size) {
        tDLogE("OpenCLBackend::enqueueRead() Invalid size error: gpu({}), host({}), size({})",
               gpu_mem.size, host_mem.size, size);
        return false;
    }
    return __impl::read(stream, gpu_mem, host_mem, size, false, event);
}

bool OpenCLBackend::flush(GpuStream & stream) const
{
#if defined(USE_OPENCL)
    cl_int code = ::clFlush((cl_command_queue)stream.stream_id);
    if (code == CL_SUCCESS) {
        return true;
    } else {
        tDLogE("OpenCLBackend::flush() OpenCL clFlush() error code: {}", code);
    }
#endif
    return false;
}

bool OpenCLBackend::finish(GpuStream & stream) const
{
#if defined(USE_OPENCL)
    cl_int code = ::clFinish((cl_command_queue)stream.stream_id);
    if (code == CL_SUCCESS) {
        return true;
    } else {
        tDLogE("OpenCLBackend::finish() OpenCL clFinish() error code: {}", code);
    }
#endif
    return false;
}

GpuProgram OpenCLBackend::createProgram(GpuContext const & context, std::string const & source) const
{
    checkType(context.type);
    GpuProgram result(context);
#if defined(USE_OPENCL)
    char const * c_source = source.c_str();
    cl_int code;
    cl_program program = ::clCreateProgramWithSource((cl_context)context.context_id, 1, (char const **)&c_source, nullptr, &code);
    if (code == CL_SUCCESS) {
        result.program_id = (id::Id)program;
    } else {
        tDLogE("OpenCLBackend::createProgram() OpenCL clCreateProgramWithSource() error code: {}", code);
    }
#endif
    return result;
}

bool OpenCLBackend::buildProgram(GpuProgram & program) const
{
    checkType(program.type);
#if defined(USE_OPENCL)
    cl_int code = ::clBuildProgram((cl_program)program.program_id, 1, (cl_device_id const *)&program.device_id,
                                   nullptr, nullptr, nullptr);
    if (code == CL_SUCCESS) {
        return true;
    } else {
        tDLogE("OpenCLBackend::buildProgram() OpenCL clBuildProgram() error code: {}", code);
    }
#endif
    return false;
}

bool OpenCLBackend::releaseProgram(GpuProgram & program) const
{
    checkType(program.type);
#if defined(USE_OPENCL)
    cl_int code = ::clReleaseProgram((cl_program)program.program_id);
    program.program_id = id::UNKNOWN_ID;
    if (code == CL_SUCCESS) {
        return true;
    } else {
        tDLogE("OpenCLBackend::releaseProgram() OpenCL clReleaseProgram() error code: {}", code);
    }
#endif
    return false;
}

GpuKernel OpenCLBackend::createKernel(GpuProgram const & program, std::string const & kernel_symbol) const
{
    checkType(program.type);
    GpuKernel result(program);
#if defined(USE_OPENCL)
    cl_int code;
    cl_kernel kernel = ::clCreateKernel((cl_program)program.program_id, kernel_symbol.c_str(), &code);
    if (code == CL_SUCCESS) {
        result.kernel_id = (id::Id)kernel;
    } else {
        tDLogE("OpenCLBackend::createKernel() OpenCL clCreateKernel() error code: {}", code);
    }
#endif
    return result;
}

bool OpenCLBackend::releaseKernel(GpuKernel & kernel) const
{
    checkType(kernel.type);
#if defined(USE_OPENCL)
    cl_int code = ::clReleaseKernel((cl_kernel)kernel.kernel_id);
    kernel.kernel_id = id::UNKNOWN_ID;
    if (code == CL_SUCCESS) {
        return true;
    } else {
        tDLogE("OpenCLBackend::releaseKernel() OpenCL clReleaseKernel() error code: {}", code);
    }
#endif
    return false;
}

bool OpenCLBackend::runAdd(GpuStream & stream, GpuMemory const & v1, GpuMemory const & v2, GpuMemory & result,
                           type::TypeTable type, int count, GpuEvent * event) const
{
    checkType(stream.type);
    checkType(v1.type);
    checkType(v2.type);
    checkType(result.type);

#if defined(USE_OPENCL)
    auto program = createProgram(stream, opencl::getOpenCLSourceOfAdd1f());
    buildProgram(program);
    auto kernel = createKernel(program, "add");
    std::size_t global_work_size[1] = { static_cast<std::size_t>(count) };

    ::clSetKernelArg((cl_kernel)kernel.kernel_id, 0, sizeof(cl_mem), &v1.data);
    ::clSetKernelArg((cl_kernel)kernel.kernel_id, 1, sizeof(cl_mem), &v2.data);
    ::clSetKernelArg((cl_kernel)kernel.kernel_id, 2, sizeof(cl_mem), &result.data);
    cl_int code = ::clEnqueueNDRangeKernel((cl_command_queue)stream.stream_id, (cl_kernel)kernel.kernel_id,
                                           1, nullptr, global_work_size, nullptr, 0, nullptr,
                                           (cl_event*)(event == nullptr ? nullptr : &event->start));

    releaseKernel(kernel);
    releaseProgram(program);

    if (code == CL_SUCCESS) {
        return true;
    } else {
        tDLogE("OpenCLBackend::runAdd() OpenCL clEnqueueNDRangeKernel() error code: {}", code);
    }
#endif
    return false;
}

} // namespace backend
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

