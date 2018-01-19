/**
 * @file   GpuKernel.hpp
 * @brief  GpuKernel class prototype.
 * @author zer0
 * @date   2018-01-16
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_GPUKERNEL_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_GPUKERNEL_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/gpu/details/GpuCommon.hpp>

#include <string>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace details {

/**
 * GpuProgram class prototype.
 *
 * @author zer0
 * @date   2018-01-14
 * @date   2018-01-15 (struct -> class)
 */
class TBAG_API GpuProgram : public GpuIdWrapper
{
public:
    GpuProgram(GpuContext const & context, std::string const & source);
    ~GpuProgram();

public:
    Err build();
};

/**
 * GpuKernel class prototype.
 *
 * @author zer0
 * @date   2018-01-14
 * @date   2018-01-15 (struct -> class)
 */
class TBAG_API GpuKernel : public GpuIdWrapper
{
private:
    std::string _name;

public:
    GpuKernel(GpuProgram const & program, std::string const & kernel_symbol);
    ~GpuKernel();

public:
    inline std::string name() const { return _name; }
};

using SharedGpuKernel = std::shared_ptr<GpuKernel>;
using WeakedGpuKernel = std::weak_ptr<GpuKernel>;

} // namespace details
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_GPUKERNEL_HPP__

