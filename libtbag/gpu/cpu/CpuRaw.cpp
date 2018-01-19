/**
 * @file   CpuRaw.cpp
 * @brief  CpuRaw class implementation.
 * @author zer0
 * @date   2018-01-10
 */

#include <libtbag/gpu/cpu/CpuRaw.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu {
namespace cpu {

// ---------------
namespace __impl {
// ---------------

template <typename T>
static bool addByCpu(T const * v1, T const * v2, T * r, int count)
{
    for (; count > 0; ++v1, ++v2, ++r, --count) {
        *r = *v1 + *v2;
    }
    return true;
}

// --------------------
} // namespace __impl {
// --------------------

bool runCpuAdd1i(int const * v1, int const * v2, int * r, int count)
{ return __impl::addByCpu(v1, v2, r, count); }
bool runCpuAdd1u(unsigned const * v1, unsigned const * v2, unsigned * r, int count)
{ return __impl::addByCpu(v1, v2, r, count); }
bool runCpuAdd1f(float const * v1, float const * v2, float * r, int count)
{ return __impl::addByCpu(v1, v2, r, count); }
bool runCpuAdd1d(double const * v1, double const * v2, double * r, int count)
{ return __impl::addByCpu(v1, v2, r, count); }

} // namespace cpu
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

