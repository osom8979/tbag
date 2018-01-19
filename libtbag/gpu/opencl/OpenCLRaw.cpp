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

// ---------------
namespace __impl {
// ---------------

TBAG_CONSTEXPR static char const * const TBAG_OPENCL_PREFIX_OF_ADD = "add";
TBAG_CONSTEXPR static char const * const TBAG_OPENCL_SOURCE_OF_ADD = R"(
__kernel void add${suffix}(__global ${type} * v1, __global ${type} * v2, __global ${type} * r, __global int size)
{
    uint w = get_global_size(0);
    uint y = get_global_id(1);
    uint x = get_global_id(0);
    uint i = y * w + x;
    r[i] = v1[i] + v2[i];
}
)";

TBAG_CONSTEXPR static char const * const TBAG_TYPE_TO_1I = "suffix=1i type=int";
TBAG_CONSTEXPR static char const * const TBAG_TYPE_TO_1U = "suffix=1u type=uint";
TBAG_CONSTEXPR static char const * const TBAG_TYPE_TO_1F = "suffix=1f type=float";
TBAG_CONSTEXPR static char const * const TBAG_TYPE_TO_1D = "suffix=1d type=double";

static std::string getOpenCLSource(char const * source, char const * env)
{ return string::Environments(env).convert(source); }

static std::string getOpenCL1i(char const * source) { return getOpenCLSource(source, TBAG_TYPE_TO_1I); }
static std::string getOpenCL1u(char const * source) { return getOpenCLSource(source, TBAG_TYPE_TO_1U); }
static std::string getOpenCL1f(char const * source) { return getOpenCLSource(source, TBAG_TYPE_TO_1F); }
static std::string getOpenCL1d(char const * source) { return getOpenCLSource(source, TBAG_TYPE_TO_1D); }

// ------------------
} // namespace __impl
// ------------------

using namespace libtbag::gpu::opencl::__impl;

std::string getOpenCLName1i(std::string const & prefix) { return getOpenCL1i(prefix.c_str()); }
std::string getOpenCLName1u(std::string const & prefix) { return getOpenCL1u(prefix.c_str()); }
std::string getOpenCLName1f(std::string const & prefix) { return getOpenCL1f(prefix.c_str()); }
std::string getOpenCLName1d(std::string const & prefix) { return getOpenCL1d(prefix.c_str()); }

std::string getOpenCLPrefixOfAdd  () { return TBAG_OPENCL_PREFIX_OF_ADD; }
std::string getOpenCLSourceOfAdd1i() { return getOpenCL1i(TBAG_OPENCL_SOURCE_OF_ADD); }
std::string getOpenCLSourceOfAdd1u() { return getOpenCL1u(TBAG_OPENCL_SOURCE_OF_ADD); }
std::string getOpenCLSourceOfAdd1f() { return getOpenCL1f(TBAG_OPENCL_SOURCE_OF_ADD); }
std::string getOpenCLSourceOfAdd1d() { return getOpenCL1d(TBAG_OPENCL_SOURCE_OF_ADD); }

} // namespace opencl
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

