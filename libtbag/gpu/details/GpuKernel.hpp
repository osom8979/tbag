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
#include <libtbag/Err.hpp>
#include <libtbag/gpu/details/GpuCommon.hpp>
#include <libtbag/gpu/details/GpuIdWrapper.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace details {

class GpuProgram;

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
    GpuKernel(GpuContext const * c = nullptr, GpuId i = UNKNOWN_ID);
    GpuKernel(GpuKernel const & obj);
    GpuKernel(GpuKernel && obj);
    virtual ~GpuKernel();

public:
    GpuKernel & operator =(GpuKernel const & obj);
    GpuKernel & operator =(GpuKernel && obj);

public:
    void swap(GpuKernel & obj);
    inline friend void swap(GpuKernel & lh, GpuKernel & rh) { lh.swap(rh); }

public:
    inline std::string getName() const { return _name; }

public:
    Err create(GpuProgram const & program, std::string const & kernel_symbol);
    Err release();

public:
    static GpuKernel instance(GpuProgram const & program, std::string const & kernel_symbol);
};

} // namespace details
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_GPUKERNEL_HPP__

