/**
 * @file   AddKernel.hpp
 * @brief  AddKernel class prototype.
 * @author zer0
 * @date   2018-01-19
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_KERNELS_ADDKERNEL_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_KERNELS_ADDKERNEL_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/gpu/kernels/BaseKernel.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu     {
namespace kernels {

/**
 * AddKernel class prototype.
 *
 * @author zer0
 * @date   2018-01-19
 */
class TBAG_API AddKernel : public BaseKernel
{
public:
    AddKernel(GpuStream const & stream);
    virtual ~AddKernel();

public:
    Err run(SyncedMemory const & v1, SyncedMemory const & v2, SyncedMemory & r, int count);
};

} // namespace kernels
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_KERNELS_ADDKERNEL_HPP__

