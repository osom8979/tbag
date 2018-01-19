/**
 * @file   OpenCLRaw.hpp
 * @brief  OpenCLRaw class prototype.
 * @author zer0
 * @date   2018-01-10
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_OPENCL_OPENCLRAW_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_OPENCL_OPENCLRAW_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu    {
namespace opencl {

TBAG_API std::string getOpenCLName1i(std::string const & prefix);
TBAG_API std::string getOpenCLName1u(std::string const & prefix);
TBAG_API std::string getOpenCLName1f(std::string const & prefix);
TBAG_API std::string getOpenCLName1d(std::string const & prefix);

TBAG_API std::string getOpenCLPrefixOfAdd  ();
TBAG_API std::string getOpenCLSourceOfAdd1i();
TBAG_API std::string getOpenCLSourceOfAdd1u();
TBAG_API std::string getOpenCLSourceOfAdd1f();
TBAG_API std::string getOpenCLSourceOfAdd1d();

} // namespace opencl
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_OPENCL_OPENCLRAW_HPP__

