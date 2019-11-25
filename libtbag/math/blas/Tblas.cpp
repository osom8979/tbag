/**
 * @file   Tblas.cpp
 * @brief  Tblas class implementation.
 * @author zer0
 * @date   2017-11-03
 */

#include <libtbag/math/blas/Tblas.hpp>
#include <libtbag/log/Log.hpp>

#include <openblas/cblas.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace math {
namespace blas {

void openblas_set_num_threads(int num_threads)
{
    ::openblas_set_num_threads(num_threads);
}

void goto_set_num_threads(int num_threads)
{
    ::goto_set_num_threads(num_threads);
}

int openblas_get_num_threads()
{
    return ::openblas_get_num_threads();
}

int openblas_get_num_procs()
{
    return ::openblas_get_num_procs();
}

char const * openblas_get_config()
{
    return ::openblas_get_config();
}

char const * openblas_get_corename()
{
    return ::openblas_get_corename();
}

int openblas_get_parallel()
{
    return ::openblas_get_parallel();
}


} // namespace blas
} // namespace math

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

