/**
 * @file   OpenCLRaw.cpp
 * @brief  OpenCLRaw class implementation.
 * @author zer0
 * @date   2018-01-10
 */

#include <libtbag/gpu/opencl/OpenCLRaw.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/string/Environments.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu    {
namespace opencl {

TBAG_CONSTEXPR static char const * const TBAG_OPENCL_SOURCE_OF_ADD = R"(
__kernel void add(__global ${type} * v1, __global ${type} * v2, __global ${type} * result)
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
    string::Environments envs;
    envs.push("type", "float");
    return envs.convert(TBAG_OPENCL_SOURCE_OF_ADD);
}

std::string getOpenCLSourceOfAdd1d() TBAG_NOEXCEPT
{
    string::Environments envs;
    envs.push("type", "double");
    return envs.convert(TBAG_OPENCL_SOURCE_OF_ADD);
}

} // namespace opencl
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

