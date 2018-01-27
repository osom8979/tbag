/**
 * @file   OpenCLContext.cpp
 * @brief  OpenCLContext class implementation.
 * @author zer0
 * @date   2018-01-16
 */

#include <libtbag/gpu/opencl/OpenCLContext.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/string/Format.hpp>
#include <libtbag/string/Environments.hpp>

#include <cassert>
#include <vector>
#include <string>

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

//TBAG_CONSTEXPR static char const * const OPENCL_DEFAULT_BUILD_OPTION = "-x clc++"; // AMD ONLY.
TBAG_CONSTEXPR   static char const * const OPENCL_DEFAULT_BUILD_OPTION = "";

TBAG_CONSTEXPR static bool isOpenCLBackendProfile() TBAG_NOEXCEPT
{
#if defined(TBAG_OPENCL_BACKEND_PROFILE)
    return true;
#else
    return false;
#endif
}

template <typename T>
static std::string getOpenCLSource(std::string const & source)
{
    return string::Environments(string::fformat("type={}", GpuMemoryTypeSuffix<T>::getPrefix())).convert(source);
}

template <typename T>
static std::string getOpenCLSymbol(std::string const & name)
{
    return string::fformat("{}_{}", name, GpuMemoryTypeSuffix<T>::getPrefix());
}

static std::string getBuildLog(cl_program program, cl_device_id device)
{
    std::size_t log_buffer_size = 0;
    std::size_t opt_buffer_size = 0;
    cl_int log_code = clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG    , 0, nullptr, &log_buffer_size);
    cl_int opt_code = clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_OPTIONS, 0, nullptr, &opt_buffer_size);

    if (log_code != CL_SUCCESS || opt_code != CL_SUCCESS) {
        tDLogW("printBuildLog() OpenCL clGetProgramBuildInfo(nullptr) error: log({}), options({})",
               log_code, opt_code);
        return std::string();
    }

    std::vector<char> log_buffer(log_buffer_size, 0);
    std::vector<char> opt_buffer(opt_buffer_size, 0);

    log_code = clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG    , log_buffer.size(), log_buffer.data(), nullptr);
    opt_code = clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_OPTIONS, opt_buffer.size(), opt_buffer.data(), nullptr);

    if (log_code != CL_SUCCESS || opt_code != CL_SUCCESS) {
        tDLogW("printBuildLog() OpenCL clGetProgramBuildInfo() error: log({}), options({})",
               log_code, opt_code);
        return std::string();
    }

    return string::fformat("ProgramId({}), DeviceId({}), Options({})\n{}",
                           (std::size_t)program, (std::size_t)device,
                           std::string(opt_buffer.data()), std::string(log_buffer.data()));
}

template <typename KernelType>
static bool setKernelMemories(KernelType kernel, GpuMemories const & mems)
{
    if (mems.empty()) {
        return false;
    }

    auto const SIZE = (cl_uint)mems.size();
    for (cl_uint i = 0; i < SIZE; ++i) {
        cl_int code = clSetKernelArg(kernel, i, mems[i].size(), mems[i].data());
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

    info.profile    = get_platform_info((cl_platform_id)platform.PLATFORM_ID, CL_PLATFORM_PROFILE);
    info.version    = get_platform_info((cl_platform_id)platform.PLATFORM_ID, CL_PLATFORM_VERSION);
    info.name       = get_platform_info((cl_platform_id)platform.PLATFORM_ID, CL_PLATFORM_NAME);
    info.vendor     = get_platform_info((cl_platform_id)platform.PLATFORM_ID, CL_PLATFORM_VENDOR);
    info.extensions = get_platform_info((cl_platform_id)platform.PLATFORM_ID, CL_PLATFORM_EXTENSIONS);
    return info;
}

int getDeviceCount(GpuPlatform const & platform)
{
    checkOpenCLGpuType(platform);
    int result = 0;
    cl_uint num_devices;
    cl_int code = clGetDeviceIDs((cl_platform_id)platform.PLATFORM_ID, CL_DEVICE_TYPE_ALL, 0, nullptr, &num_devices);
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
    cl_int code = clGetDeviceIDs((cl_platform_id)platform.PLATFORM_ID, CL_DEVICE_TYPE_ALL,
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

    info.name           = get_device_info_str  ((cl_device_id)device.DEVICE_ID, CL_DEVICE_NAME);
    info.device_version = get_device_info_str  ((cl_device_id)device.DEVICE_ID, CL_DEVICE_VERSION);
    info.driver_version = get_device_info_str  ((cl_device_id)device.DEVICE_ID, CL_DRIVER_VERSION);
    info.global_memory  = get_device_info_ulong((cl_device_id)device.DEVICE_ID, CL_DEVICE_GLOBAL_MEM_SIZE);
    return info;
}

SharedGpuContext createContext(GpuDevice const & device)
{
    checkOpenCLGpuType(device);
    cl_int code;
    cl_context context = clCreateContext(nullptr, 1, (cl_device_id const *)&device.DEVICE_ID, nullptr, nullptr, &code);
    if (code == CL_SUCCESS) {
        return SharedGpuContext(new OpenCLContext(device, (GpuId)context), [](OpenCLContext * context){
            cl_int code = clReleaseContext((cl_context)context->CONTEXT_ID);
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

Err OpenCLContext::_write(GpuStream const & stream, GpuMemory & gpu_mem, HostMemory const & host_mem,
                          std::size_t size, bool blocking, GpuEvent * event) const
{
    cl_int code = clEnqueueWriteBuffer(stream.castId<cl_command_queue>(), gpu_mem.cast<cl_mem>(),
                                       (blocking ? CL_TRUE : CL_FALSE),
                                       0, host_mem.size(), host_mem.data(), 0, nullptr,
                                       (cl_event*)(event == nullptr ? nullptr : &event->atId()));
    if (code != CL_SUCCESS) {
        tDLogE("OpenCLContext::_write({}) OpenCL clEnqueueWriteBuffer() error code: {}",
               (blocking ? "BLOCKING" : "NON-BLOCKING"), code);
        return Err::E_OPENCL;
    }
    return Err::E_SUCCESS;
}

Err OpenCLContext::_read(GpuStream const & stream, GpuMemory const & gpu_mem, HostMemory & host_mem,
                         std::size_t size, bool blocking, GpuEvent * event) const
{
    cl_int code = clEnqueueReadBuffer(stream.castId<cl_command_queue>(), gpu_mem.cast<cl_mem>(),
                                      (blocking ? CL_TRUE : CL_FALSE),
                                      0, host_mem.size(), host_mem.data(), 0, nullptr,
                                      (cl_event*)(event == nullptr ? nullptr : &event->atId()));
    if (code != CL_SUCCESS) {
        tDLogE("OpenCLContext::_read({}) OpenCL clEnqueueReadBuffer() error code: {}",
               (blocking ? "BLOCKING" : "NON-BLOCKING"), code);
        return Err::E_OPENCL;
    }
    return Err::E_SUCCESS;
}

// @formatter:off
bool OpenCLContext::isSupport() const TBAG_NOEXCEPT { return opencl::isSupport(); }
bool OpenCLContext::isHost   () const TBAG_NOEXCEPT { return true; }
bool OpenCLContext::isDevice () const TBAG_NOEXCEPT { return true; }
bool OpenCLContext::isStream () const TBAG_NOEXCEPT { return true; }
// @formatter:on

Err OpenCLContext::createStream(GpuStream & stream) const
{
    if (stream.isSameContext(*this) == false) {
        return Err::E_ILLARGS;
    }

    cl_int code;
    cl_command_queue_properties properties = 0;
    if (__impl::isOpenCLBackendProfile()) {
        properties |= CL_QUEUE_PROFILING_ENABLE;
    }
    cl_command_queue native_stream = clCreateCommandQueue((cl_context)CONTEXT_ID,
                                                          (cl_device_id)DEVICE_ID,
                                                          properties, &code);
    if (code != CL_SUCCESS) {
        tDLogE("OpenCLContext::createStream() OpenCL clCreateCommandQueue() error code: {}", code);
        return Err::E_OPENCL;
    }

    stream.setId(native_stream);
    tDLogIfD(isGpuVerbose(), "OpenCLContext({})::createStream({})", getContextId(), stream.getId());
    return Err::E_SUCCESS;
}

Err OpenCLContext::releaseStream(GpuStream & stream) const
{
    if (stream.validate(*this) == false) {
        return Err::E_ILLARGS;
    }

    tDLogIfD(isGpuVerbose(), "OpenCLContext({})::releaseStream({})", getContextId(), stream.getId());
    cl_int code = clReleaseCommandQueue(stream.castId<cl_command_queue>());
    stream.clearId();
    if (code != CL_SUCCESS) {
        tDLogE("OpenCLContext::releaseStream() OpenCL clReleaseCommandQueue() error code: {}", code);
        return Err::E_OPENCL;
    }
    return Err::E_SUCCESS;
}

Err OpenCLContext::createEvent(GpuStream const & stream, GpuEvent & event) const
{
    if (stream.validate(*this) == false || event.isSameContext(*this) == false) {
        return Err::E_ILLARGS;
    }
    event.setStart(0);
    event.setStop(0);
    tDLogIfD(isGpuVerbose(), "OpenCLContext({})::createEvent(s:{}, e:{})",
             getContextId(), stream.getId(), event.getId());
    return Err::E_SUCCESS;
}

Err OpenCLContext::syncEvent(GpuEvent const & event) const
{
    if (event.validate(*this) == false) {
        return Err::E_ILLARGS;
    }

    cl_int code = clWaitForEvents(1, (cl_event const *)&event.atId());
    if (code != CL_SUCCESS) {
        tDLogE("OpenCLContext::syncEvent() OpenCL clWaitForEvents() error code: {}", code);
        return Err::E_OPENCL;
    }
    return Err::E_SUCCESS;
}

Err OpenCLContext::elapsedEvent(GpuEvent & event, float * millisec) const
{
    if (event.validate(*this) == false) {
        return Err::E_ILLARGS;
    }

    cl_ulong start_nano, stop_nano;
    cl_int start_code = clGetEventProfilingInfo(event.castId<cl_event>(), CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start_nano, nullptr);
    cl_int  stop_code = clGetEventProfilingInfo(event.castId<cl_event>(), CL_PROFILING_COMMAND_END  , sizeof(cl_ulong),  &stop_nano, nullptr);

    if (start_code != CL_SUCCESS || stop_code != CL_SUCCESS) {
        tDLogE("OpenCLContext::elapsedEvent() OpenCL clGetEventProfilingInfo() error code: start({}), stop({})",
               start_code, stop_code);
        return Err::E_OPENCL;
    }

    if (millisec != nullptr) {
        *millisec = (stop_nano - start_nano) * 1.0e-6f;
    }
    return Err::E_SUCCESS;
}

Err OpenCLContext::releaseEvent(GpuEvent & event) const
{
    if (event.validate(*this) == false) {
        return Err::E_ILLARGS;
    }
    tDLogIfD(isGpuVerbose(), "OpenCLContext({})::releaseEvent({})", getContextId(), event.getId());
    event.clearIds();
    return Err::E_SUCCESS;
}

Err OpenCLContext::createProgram(std::string const & source, GpuProgram & program) const
{
    if (source.empty() || program.isSameContext(*this) == false) {
        return Err::E_ILLARGS;
    }

    char const * c_source = source.c_str();
    cl_int code;
    cl_program native_program = clCreateProgramWithSource((cl_context)CONTEXT_ID, 1, &c_source, nullptr, &code);
    if (code != CL_SUCCESS) {
        tDLogE("OpenCLContext::createProgram() OpenCL clCreateProgramWithSource() error code: {}", code);
        return Err::E_OPENCL;
    }

    program.setId(native_program);
    tDLogIfD(isGpuVerbose(), "OpenCLContext({})::createProgram({})", getContextId(), program.getId());
    return Err::E_SUCCESS;
}

Err OpenCLContext::buildProgram(GpuProgram & program) const
{
    if (program.validate(*this) == false) {
        return Err::E_ILLARGS;
    }

    cl_int code = clBuildProgram(program.castId<cl_program>(), 1, (cl_device_id const *)&DEVICE_ID,
                                 __impl::OPENCL_DEFAULT_BUILD_OPTION, nullptr, nullptr);
    if (code != CL_SUCCESS) {
        tDLogE("OpenCLContext::buildProgram() OpenCL clBuildProgram() error code: {}", code);
        tDLogW("Build Log: {}", __impl::getBuildLog(program.castId<cl_program>(), castDeviceId<cl_device_id>()));
        return Err::E_OPENCL;
    }
    return Err::E_SUCCESS;
}

Err OpenCLContext::releaseProgram(GpuProgram & program) const
{
    if (program.validate(*this) == false) {
        return Err::E_ILLARGS;
    }

    tDLogIfD(isGpuVerbose(), "OpenCLContext({})::releaseProgram({})", getContextId(), program.getId());
    cl_int code = clReleaseProgram(program.castId<cl_program>());
    program.clearId();
    if (code != CL_SUCCESS) {
        tDLogE("OpenCLContext::releaseProgram() OpenCL clReleaseProgram() error code: {}", code);
        return Err::E_OPENCL;
    }
    return Err::E_SUCCESS;
}

Err OpenCLContext::createKernel(GpuProgram const & program, std::string const & kernel_symbol, GpuKernel & kernel) const
{
    if (program.validate(*this) == false || kernel_symbol.empty() || kernel.isSameContext(*this) == false) {
        return Err::E_ILLARGS;
    }

    cl_int code;
    cl_kernel native_kernel = clCreateKernel(program.castId<cl_program>(), kernel_symbol.c_str(), &code);
    if (code != CL_SUCCESS) {
        tDLogE("OpenCLContext::createKernel() OpenCL clCreateKernel() error code: {}", code);
        return Err::E_OPENCL;
    }

    kernel.setId(native_kernel);
    tDLogIfD(isGpuVerbose(), "OpenCLContext({})::createKernel(p:{}, k:{}) Symbol: {}",
             getContextId(), program.getId(), kernel.getId(), kernel_symbol);
    return Err::E_SUCCESS;
}

Err OpenCLContext::releaseKernel(GpuKernel & kernel) const
{
    if (kernel.validate(*this) == false) {
        return Err::E_ILLARGS;
    }

    tDLogIfD(isGpuVerbose(), "OpenCLContext({})::releaseKernel({})", getContextId(), kernel.getId());
    cl_int code = clReleaseKernel(kernel.castId<cl_kernel>());
    kernel.clearId();
    if (code != CL_SUCCESS) {
        tDLogE("OpenCLContext::releaseKernel() OpenCL clReleaseKernel() error code: {}", code);
        return Err::E_OPENCL;
    }
    return Err::E_SUCCESS;
}

Err OpenCLContext::malloc(GpuMemory & mem, std::size_t size) const
{
    if (mem.isSameContext(*this) == false) {
        return Err::E_ILLARGS;
    }

    cl_int code;
    cl_mem native_memory = clCreateBuffer((cl_context)CONTEXT_ID, CL_MEM_READ_WRITE, size, nullptr, &code);
    if (code != CL_SUCCESS) {
        tDLogE("OpenCLContext::malloc({}) OpenCL clCreateBuffer() error code: {}", size, code);
        return Err::E_OPENCL;
    }

    mem.set((void*)native_memory, size, size);
    tDLogIfD(isGpuVerbose(), "OpenCLContext({})::malloc({}) OpenCL clCreateBuffer() MEM:{} CAP:{} SIZE:{}",
             getContextId(), size, mem.data(), mem.capacity(), mem.size());
    return Err::E_SUCCESS;
}

Err OpenCLContext::free(GpuMemory & mem) const
{
    if (mem.validate(*this) == false) {
        return Err::E_ILLARGS;
    }

    tDLogIfD(isGpuVerbose(), "OpenCLContext({})::free() OpenCL clReleaseMemObject() MEM:{} CAP:{} SIZE:{}",
             getContextId(), mem.data(), mem.capacity(), mem.size());

    cl_int code = clReleaseMemObject(mem.cast<cl_mem>());
    mem.clear();
    if (code != CL_SUCCESS) {
        tDLogE("OpenCLContext::free() OpenCL clReleaseMemObject() error code: {}", code);
        return Err::E_OPENCL;
    }
    return Err::E_SUCCESS;
}

Err OpenCLContext::write(GpuStream const & stream, GpuMemory & gpu_mem, HostMemory const & host_mem, std::size_t size, GpuEvent * event) const
{
    if (validateMemory(stream, gpu_mem, host_mem, size) == false) {
        return Err::E_ILLARGS;
    }
    return _write(stream, gpu_mem, host_mem, size, true, event);
}

Err OpenCLContext::read(GpuStream const & stream, GpuMemory const & gpu_mem, HostMemory & host_mem, std::size_t size, GpuEvent * event) const
{
    if (validateMemory(stream, gpu_mem, host_mem, size) == false) {
        return Err::E_ILLARGS;
    }
    return _read(stream, gpu_mem, host_mem, size, true, event);
}

Err OpenCLContext::writeAsync(GpuStream const & stream, GpuMemory & gpu_mem, HostMemory const & host_mem, std::size_t size, GpuEvent * event) const
{
    if (validateMemory(stream, gpu_mem, host_mem, size) == false) {
        return Err::E_ILLARGS;
    }
    return _write(stream, gpu_mem, host_mem, size, false, event);
}

Err OpenCLContext::readAsync(GpuStream const & stream, GpuMemory const & gpu_mem, HostMemory & host_mem, std::size_t size, GpuEvent * event) const
{
    if (validateMemory(stream, gpu_mem, host_mem, size) == false) {
        return Err::E_ILLARGS;
    }
    return _read(stream, gpu_mem, host_mem, size, false, event);
}

Err OpenCLContext::copy(GpuStream const & stream, GpuMemory const & src, GpuMemory & dest, std::size_t size, GpuEvent * event) const
{
    if (validateMemory(stream, src, dest, size) == false) {
        return Err::E_ILLARGS;
    }

    cl_int code = clEnqueueCopyBuffer(stream.castId<cl_command_queue>(), src.cast<cl_mem>(), dest.cast<cl_mem>(),
                                      0, 0, size, 0, nullptr, (cl_event*)(event == nullptr ? nullptr : &event->atId()));
    if (code != CL_SUCCESS) {
        tDLogE("OpenCLContext::copy() OpenCL clEnqueueCopyBuffer() error code: {}", code);
        return Err::E_OPENCL;
    }

    cl_int finish_code = clFinish(stream.castId<cl_command_queue>());
    if (finish_code != CL_SUCCESS) {
        tDLogE("OpenCLContext::copy() OpenCL clFinish() error code: {}", finish_code);
        return Err::E_OPENCL;
    }

    return Err::E_SUCCESS;
}

Err OpenCLContext::copyAsync(GpuStream const & stream, GpuMemory const & src, GpuMemory & dest, std::size_t size, GpuEvent * event) const
{
    if (validateMemory(stream, src, dest, size) == false) {
        return Err::E_ILLARGS;
    }

    cl_int code = clEnqueueCopyBuffer(stream.castId<cl_command_queue>(), src.cast<cl_mem>(), dest.cast<cl_mem>(),
                                      0, 0, size, 0, nullptr, (cl_event*)(event == nullptr ? nullptr : &event->atId()));
    if (code != CL_SUCCESS) {
        tDLogE("OpenCLContext::copyAsync() OpenCL clEnqueueCopyBuffer() error code: {}", code);
        return Err::E_OPENCL;
    }
    return Err::E_SUCCESS;
}

Err OpenCLContext::flush(GpuStream const & stream) const
{
    if (stream.validate(*this) == false) {
        return Err::E_ILLARGS;
    }

    cl_int code = clFlush(stream.castId<cl_command_queue>());
    if (code != CL_SUCCESS) {
        tDLogE("OpenCLContext::flush() OpenCL clFlush() error code: {}", code);
        return Err::E_OPENCL;
    }
    return Err::E_SUCCESS;
}

Err OpenCLContext::finish(GpuStream const & stream) const
{
    if (stream.validate(*this) == false) {
        return Err::E_ILLARGS;
    }

    cl_int code = clFinish(stream.castId<cl_command_queue>());
    if (code != CL_SUCCESS) {
        tDLogE("OpenCLContext::finish() OpenCL clFinish() error code: {}", code);
        return Err::E_OPENCL;
    }
    return Err::E_SUCCESS;
}

TBAG_CONSTEXPR static char const * const _TBAG_OPENCL_SOURCE_FILL = R"(
__kernel void fill_${type}(__global ${type} * out, ${type} data)
{
    uint w = get_global_size(0);
    uint y = get_global_id(1);
    uint x = get_global_id(0);
    uint i = y * w + x;
    out[i] = data;
}
)";

template <typename T>
static Err fillByOpenCL(GpuContext const & context, GpuStream const & stream, SharedGpuKernel & kernel,
                       T * out, T data, int count, GpuEvent * event)
{
    if (stream.validate(context) == false) {
        return Err::E_ILLARGS;
    }

    if (static_cast<bool>(kernel) == false) {
        SharedGpuProgram program(GpuProgram::newInstance(context, __impl::getOpenCLSource<T>(_TBAG_OPENCL_SOURCE_FILL)));
        Err const BUILD_RESULT = program->build();
        if (isFailure(BUILD_RESULT)) {
            return BUILD_RESULT;
        }

        try {
            kernel.reset(GpuKernel::newInstance(*program, __impl::getOpenCLSymbol<T>("fill")));
        } catch (std::bad_alloc & e) {
            return Err::E_BADALLOC;
        }
    }

    assert(static_cast<bool>(kernel));

    cl_int c0 = clSetKernelArg(kernel->castId<cl_kernel>(), 0, sizeof(cl_mem), &out);
    cl_int c1 = clSetKernelArg(kernel->castId<cl_kernel>(), 1, sizeof(T), &data);

    if (c0 != CL_SUCCESS || c1 != CL_SUCCESS) {
        tDLogE("OpenCLContext::fillByOpenCL() OpenCL clSetKernelArg() error: c0({}), c1({})", c0, c1);
        return Err::E_OPENCL;
    }

    std::size_t global_work_size[1] = { static_cast<std::size_t>(count) };
    cl_int code = clEnqueueNDRangeKernel(stream.castId<cl_command_queue>(), kernel->castId<cl_kernel>(),
                                         1, nullptr, global_work_size, nullptr, 0, nullptr,
                                         (cl_event*)(event == nullptr ? nullptr : &event->atId()));
    if (code != CL_SUCCESS) {
        tDLogE("OpenCLContext::fillByOpenCL() OpenCL clEnqueueNDRangeKernel() error code: {}", code);
        return Err::E_OPENCL;
    }
    return Err::E_SUCCESS;
}

// @formatter:off
Err OpenCLContext::fill(GpuStream const & stream, int * out, int data, int count, GpuEvent * event) const
{ return fillByOpenCL(*this, stream, _fill.i, out, data, count, event); }
Err OpenCLContext::fill(GpuStream const & stream, unsigned * out, unsigned data, int count, GpuEvent * event) const
{ return fillByOpenCL(*this, stream, _fill.u, out, data, count, event); }
Err OpenCLContext::fill(GpuStream const & stream, float * out, float data, int count, GpuEvent * event) const
{ return fillByOpenCL(*this, stream, _fill.f, out, data, count, event); }
Err OpenCLContext::fill(GpuStream const & stream, double * out, double data, int count, GpuEvent * event) const
{ return fillByOpenCL(*this, stream, _fill.d, out, data, count, event); }
// @formatter:on

TBAG_CONSTEXPR static char const * const _TBAG_OPENCL_SOURCE_ADD = R"(
__kernel void add_${type}(__global ${type} * in1, __global ${type} * in2, __global ${type} * out)
{
    uint w = get_global_size(0);
    uint y = get_global_id(1);
    uint x = get_global_id(0);
    uint i = y * w + x;
    out[i] = in1[i] + in2[i];
}
)";

template <typename T>
static Err addByOpenCL(GpuContext const & context, GpuStream const & stream, SharedGpuKernel & kernel,
                       T const * in1, T const * in2, T * out, int count, GpuEvent * event)
{
    if (stream.validate(context) == false) {
        return Err::E_ILLARGS;
    }

    if (static_cast<bool>(kernel) == false) {
        SharedGpuProgram program(GpuProgram::newInstance(context, __impl::getOpenCLSource<T>(_TBAG_OPENCL_SOURCE_ADD)));
        Err const BUILD_RESULT = program->build();
        if (isFailure(BUILD_RESULT)) {
            return BUILD_RESULT;
        }

        try {
            kernel.reset(GpuKernel::newInstance(*program, __impl::getOpenCLSymbol<T>("add")));
        } catch (std::bad_alloc & e) {
            return Err::E_BADALLOC;
        }
    }

    assert(static_cast<bool>(kernel));

    cl_int c0 = clSetKernelArg(kernel->castId<cl_kernel>(), 0, sizeof(cl_mem), in1);
    cl_int c1 = clSetKernelArg(kernel->castId<cl_kernel>(), 1, sizeof(cl_mem), in2);
    cl_int c2 = clSetKernelArg(kernel->castId<cl_kernel>(), 2, sizeof(cl_mem), out);

    if (c0 != CL_SUCCESS || c1 != CL_SUCCESS || c2 != CL_SUCCESS) {
        tDLogE("OpenCLContext::addByOpenCL() OpenCL clSetKernelArg() error: c0({}), c1({}), c2({})", c0, c1, c2);
        return Err::E_OPENCL;
    }

    std::size_t global_work_size[1] = { static_cast<std::size_t>(count) };
    cl_int code = clEnqueueNDRangeKernel(stream.castId<cl_command_queue>(), kernel->castId<cl_kernel>(),
                                         1, nullptr, global_work_size, nullptr, 0, nullptr,
                                         (cl_event*)(event == nullptr ? nullptr : &event->atId()));
    if (code != CL_SUCCESS) {
        tDLogE("OpenCLContext::addByOpenCL() OpenCL clEnqueueNDRangeKernel() error code: {}", code);
        return Err::E_OPENCL;
    }
    return Err::E_SUCCESS;
}

// @formatter:off
Err OpenCLContext::add(GpuStream const & stream, int const * in1, int const * in2, int * out, int count, GpuEvent * event) const
{ return addByOpenCL(*this, stream, _add.i, in1, in2, out, count, event); }
Err OpenCLContext::add(GpuStream const & stream, unsigned const * in1, unsigned const * in2, unsigned * out, int count, GpuEvent * event) const
{ return addByOpenCL(*this, stream, _add.u, in1, in2, out, count, event); }
Err OpenCLContext::add(GpuStream const & stream, float const * in1, float const * in2, float * out, int count, GpuEvent * event) const
{ return addByOpenCL(*this, stream, _add.f, in1, in2, out, count, event); }
Err OpenCLContext::add(GpuStream const & stream, double const * in1, double const * in2, double * out, int count, GpuEvent * event) const
{ return addByOpenCL(*this, stream, _add.d, in1, in2, out, count, event); }
// @formatter:on

} // namespace opencl
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

