/**
 * @file   GpuProgram.hpp
 * @brief  GpuProgram class prototype.
 * @author zer0
 * @date   2018-01-16
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_GPUPROGRAM_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_GPUPROGRAM_HPP__

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
    GpuProgram(GpuContext const * c = nullptr, GpuId i = UNKNOWN_ID);
    GpuProgram(GpuProgram const & obj);
    GpuProgram(GpuProgram && obj);
    virtual ~GpuProgram();

public:
    GpuProgram & operator =(GpuProgram const & obj);
    GpuProgram & operator =(GpuProgram && obj);

public:
    void swap(GpuProgram & obj);
    inline friend void swap(GpuProgram & lh, GpuProgram & rh) { lh.swap(rh); }

public:
    Err create(std::string const & source);
    Err build();
    Err release();

public:
    static GpuProgram instance(GpuContext const * c, std::string const & source);
};

} // namespace details
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_DETAILS_GPUPROGRAM_HPP__

