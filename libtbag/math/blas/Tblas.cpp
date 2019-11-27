/**
 * @file   Tblas.cpp
 * @brief  Tblas class implementation.
 * @author zer0
 * @date   2017-11-03
 */

#include <libtbag/math/blas/Tblas.hpp>
#include <libtbag/log/Log.hpp>

#include <openblas/cblas.h>

#if defined(TBAG_PLATFORM_WINDOWS)
# include <windows.h>
BOOL APIENTRY openblas_DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved);
#else
# include <libtbag/dummy/Win32.hpp>
using namespace ::libtbag::dummy::win32;
BOOL openblas_DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{ return TRUE; }
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace math {
namespace blas {

int openblas_dll_main(void * module, unsigned long reason, void * reserved)
{
    return openblas_DllMain((HMODULE)module, (DWORD)reason, (LPVOID)reserved);
}

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

