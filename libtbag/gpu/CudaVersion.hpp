/**
 * @file   CudaVersion.hpp
 * @brief  CudaVersion class prototype.
 * @author zer0
 * @date   2017-07-31
 *
 * @see <https://developer.nvidia.com/cuda-gpus>
 * @see <https://en.wikipedia.org/wiki/CUDA>
 *
 * @remarks
 *  - CUDA SDK 6.5: Last Version with support for Compute Capability 1.x (Tesla)
 *  - CUDA SDK 7.5 support for Compute Capability 2.0 – 5.x (Fermi, Kepler, Maxwell)
 *  - CUDA SDK 8.0 support for Compute Capability 2.0 – 6.x (Fermi, Kepler, Maxwell, Pascal), last version with support for Compute Capability 2.x (Fermi)
 *  - CUDA SDK 9.0 support for Compute Capability 3.0 – 7.x (Kepler, Maxwell, Pascal, Volta)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GPU_CUDAVERSION_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GPU_CUDAVERSION_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu {

enum class NvidiaMicroArchitecture
{
    NMA_TESLA,
    NMA_FERMI,
    NMA_KEPLER,
    NMA_MAXWELL,
    NMA_PASCAL,
    NMA_VOLTA,
};

#ifndef TBAG_CUDA_VERSION_INFO_MAP
#define TBAG_CUDA_VERSION_INFO_MAP(_TBAG_XX) \
    /* Column information: {def, name, arch, major, minor, } */ \
    /* -- END -- */
#endif

} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GPU_CUDAVERSION_HPP__

