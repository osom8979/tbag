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
    cl_uint platforms;
    if (clGetPlatformIDs(0, nullptr, &platforms) == CL_SUCCESS) {
        return platforms;
    }
#endif
    return 0;
}

} // namespace backend
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

