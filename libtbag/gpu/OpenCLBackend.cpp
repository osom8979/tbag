/**
 * @file   OpenCLBackend.cpp
 * @brief  OpenCLBackend class implementation.
 * @author zer0
 * @date   2017-04-18
 */

#include <libtbag/gpu/OpenCLBackend.hpp>
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

namespace gpu {

OpenCLBackend::OpenCLBackend()
{
    // EMPTY.
}

OpenCLBackend::~OpenCLBackend()
{
    // EMPTY.
}

bool OpenCLBackend::isSupport() const TBAG_NOEXCEPT
{
#if defined(USE_OPENCL)
    return true;
#else
    return false;
#endif
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

} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

