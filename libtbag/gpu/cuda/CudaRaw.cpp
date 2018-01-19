/**
 * @file   CudaRaw.cpp
 * @brief  CPU operation to use instead of CUDA.
 * @author zer0
 * @date   2017-08-01
 */

#include <libtbag/gpu/cuda/CudaRaw.hpp>
#include <libtbag/gpu/cuda/CudaRaw.h>
#include <libtbag/log/Log.hpp>

#if !defined(USE_CUDA)
tbBOOL tbCudaAdd1i(int       const * v1, int       const * v2, int      * r, int count, void * stream) { return TB_FALSE; }
tbBOOL tbCudaAdd1u(unsigned  const * v1, unsigned  const * v2, unsigned * r, int count, void * stream) { return TB_FALSE; }
tbBOOL tbCudaAdd1f(float     const * v1, float     const * v2, float    * r, int count, void * stream) { return TB_FALSE; }
tbBOOL tbCudaAdd1d(double    const * v1, double    const * v2, double   * r, int count, void * stream) { return TB_FALSE; }
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gpu  {
namespace cuda {

bool runCudaAdd(type::TypeTable type, void const * v1, void const * v2, void * r, int count, void * stream)
{
    switch (type) {
    case type::TypeTable::TT_INT:    return tbCudaAdd1i((int      const *)v1, (int      const *)v2, (int      *)r, count, stream) == TB_TRUE;
    case type::TypeTable::TT_UINT:   return tbCudaAdd1u((unsigned const *)v1, (unsigned const *)v2, (unsigned *)r, count, stream) == TB_TRUE;
    case type::TypeTable::TT_FLOAT:  return tbCudaAdd1f((float    const *)v1, (float    const *)v2, (float    *)r, count, stream) == TB_TRUE;
    case type::TypeTable::TT_DOUBLE: return tbCudaAdd1d((double   const *)v1, (double   const *)v2, (double   *)r, count, stream) == TB_TRUE;
    default: break;
    }
    return false;
}

} // namespace cuda
} // namespace gpu

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------
