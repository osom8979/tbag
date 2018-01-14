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

TBAG_CONSTEXPR static char const * const TBAG_OPENCL_NAME_OF_ADD = "add";
TBAG_CONSTEXPR static char const * const TBAG_OPENCL_SOURCE_OF_ADD = R"(
__kernel void add(__global ${type} * v1, __global ${type} * v2, __global ${type} * result, __global uint size)
{
    uint w = get_global_size(0);
    uint y = get_global_id(1);
    uint x = get_global_id(0);
    uint i = y * w + x;
    result[i] = v1[i] + v2[i];
}
)";

TBAG_CONSTEXPR static char const * const TBAG_TYPE_TO_INT    = "type=int";
TBAG_CONSTEXPR static char const * const TBAG_TYPE_TO_UINT   = "type=uint";
TBAG_CONSTEXPR static char const * const TBAG_TYPE_TO_FLOAT  = "type=float";
TBAG_CONSTEXPR static char const * const TBAG_TYPE_TO_DOUBLE = "type=double";

static std::string getOpenCLSource(char const * source, char const * env)
{ return string::Environments(env).convert(source); }
static std::string getOpenCLIntSource(char const * source)
{ return getOpenCLSource(source, TBAG_TYPE_TO_INT); }
static std::string getOpenCLUintSource(char const * source)
{ return getOpenCLSource(source, TBAG_TYPE_TO_UINT); }
static std::string getOpenCLFloatSource(char const * source)
{ return getOpenCLSource(source, TBAG_TYPE_TO_FLOAT); }
static std::string getOpenCLDoubleSource(char const * source)
{ return getOpenCLSource(source, TBAG_TYPE_TO_DOUBLE); }

// ------------------
} // namespace __impl
// ------------------

using namespace libtbag::gpu::opencl::__impl;

std::string getOpenCLSourceOfAddName() TBAG_NOEXCEPT { return TBAG_OPENCL_NAME_OF_ADD; }
std::string getOpenCLSourceOfAdd1i  () TBAG_NOEXCEPT { return getOpenCLIntSource   (TBAG_OPENCL_SOURCE_OF_ADD); }
std::string getOpenCLSourceOfAdd1u  () TBAG_NOEXCEPT { return getOpenCLUintSource  (TBAG_OPENCL_SOURCE_OF_ADD); }
std::string getOpenCLSourceOfAdd1f  () TBAG_NOEXCEPT { return getOpenCLFloatSource (TBAG_OPENCL_SOURCE_OF_ADD); }
std::string getOpenCLSourceOfAdd1d  () TBAG_NOEXCEPT { return getOpenCLDoubleSource(TBAG_OPENCL_SOURCE_OF_ADD); }

} // namespace opencl
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

