/**
 * @file   CudaUtils.cpp
 * @brief  CudaUtils class implementation.
 * @author zer0
 * @date   2017-08-01
 */

#include <libtbag/gpu/cuda/CudaUtils.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/log/Log.hpp>

#include <algorithm>

// ---------
// CPU CODE.
// ---------

int tbCudaAddByCpu(int const * lh, int const * rh, int * result, int size)
{
    for (int i = 0; i < size; ++i) {
        *(result + i) = *(lh + i) + *(rh + i);
    }
    return 0;
}

#if !defined(USE_CUDA)
int tbCudaAddByGpu(int const * lh, int const * rh, int * result, int size)
{ TBAG_INACCESSIBLE_BLOCK_ASSERT(); return 0; }
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu  {
namespace cuda {

// -------------
namespace impl {
// -------------

inline static BrewType upgradeCudaBrew(BrewType type) TBAG_NOEXCEPT
{
    if (type == BrewType::BT_UNKNOWN) {
        if (isCudaSupport()) {
            return BrewType::BT_GPU;
        } else {
            return BrewType::BT_CPU;
        }
    }
    return type;
}

// ----------------
} // namespace impl
// ----------------

std::vector<int> add(std::vector<int> const & lh, std::vector<int> const & rh, BrewType type)
{
    type = impl::upgradeCudaBrew(type);
    std::size_t const SIZE = std::min(lh.size(), rh.size());
    std::vector<int> result(SIZE);
    if (type == BrewType::BT_GPU) {
        tbCudaAddByGpu(&lh[0], &rh[0], &result[0], SIZE);
    } else {
        tbCudaAddByCpu(&lh[0], &rh[0], &result[0], SIZE);
    }
    return result;
}

} // namespace cuda
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

