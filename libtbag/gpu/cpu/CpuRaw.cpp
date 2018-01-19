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

bool runCpuAdd(type::TypeTable type, void const * v1, void const * v2, void * r, int count)
{
    switch (type) {
    case type::TypeTable::TT_INT:    return __impl::addByCpu((int      const *)v1, (int      const *)v2, (int      *)r, count);
    case type::TypeTable::TT_UINT:   return __impl::addByCpu((unsigned const *)v1, (unsigned const *)v2, (unsigned *)r, count);
    case type::TypeTable::TT_FLOAT:  return __impl::addByCpu((float    const *)v1, (float    const *)v2, (float    *)r, count);
    case type::TypeTable::TT_DOUBLE: return __impl::addByCpu((double   const *)v1, (double   const *)v2, (double   *)r, count);
    default: break;
    }
    return false;
}

} // namespace cpu
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

