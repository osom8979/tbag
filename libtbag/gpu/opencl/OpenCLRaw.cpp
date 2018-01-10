/**
 * @file   OpenCLRaw.cpp
 * @brief  OpenCLRaw class implementation.
 * @author zer0
 * @date   2018-01-10
 */

#include <libtbag/gpu/opencl/OpenCLRaw.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/string/Format.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu    {
namespace opencl {

TBAG_CONSTEXPR static char const * const TBAG_OPENCL_SOURCE_OF_ADD = R"(
__kernel void add(__global {1} * v1, __global {1} * v2, __global {1} * result)
{
    uint w = get_global_size(0);
    uint y = get_global_id(1);
    uint x = get_global_id(0);
    uint i = y * w + x;
    result[i] = v1[i] + v2[i];
}
)";

std::string getOpenCLSourceOfAdd1f() TBAG_NOEXCEPT
{
    return string::fformat(TBAG_OPENCL_SOURCE_OF_ADD, "float");
}

std::string getOpenCLSourceOfAdd1d() TBAG_NOEXCEPT
{
    return string::fformat(TBAG_OPENCL_SOURCE_OF_ADD, "double");
}

} // namespace opencl
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

