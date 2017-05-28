/**
 * @file   libtbshare.cpp
 * @brief  libtbshare entry-point.
 * @author zer0
 * @date   2016-04-17
 * @date   2016-10-14 (Rename: libtbag_test -> libtbtest)
 * @date   2016-10-25 (Rename: libtbtest -> tbshare)
 * @date   2017-04-12 (Rename: tbshare -> libtbshare)
 */

#if defined(WIN32) || defined(_WIN32)
# include <windows.h>
# if defined(USE_LIBTEST_EXPORT)
#  define LIBTBAG_TEST_EXPORT __declspec(dllexport)
# else
#  define LIBTBAG_TEST_EXPORT __declspec(dllimport)
# endif /* USE_LIBTESTBAG_EXPORT */
#else /* defined(WIN32) || defined(_WIN32) */
# define LIBTBAG_TEST_EXPORT
#endif

#if defined(__cplusplus)
extern "C" {
#endif

/* Add function test. */
LIBTBAG_TEST_EXPORT int tbshare_add(int v1, int v2)
{
    return v1 + v2;
}

/* Add function test2. */
LIBTBAG_TEST_EXPORT void tbshare_add2(int v1, int v2, int * result)
{
    if (result != nullptr) {
        *result = v1 + v2;
    }
}

#if defined(__cplusplus)
} /* extern "C" */
#endif

