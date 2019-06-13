#ifndef __INCLUDE_LMDB__LMDB_EXPORT_API_H__
#define __INCLUDE_LMDB__LMDB_EXPORT_API_H__

#if defined(__GNUC__)
#define __LMDB_GNUC_FULLVERSION \
    ( (__GNUC__            * 10000) \
    + (__GNUC_MINOR__      *   100) \
    + (__GNUC_PATCHLEVEL__ *     1) )
#else
#define __LMDB_GNUC_FULLVERSION 0
#endif

#if defined(WIN32) || defined(_WIN32) || \
    defined(WIN64) || defined(_WIN64) || \
    defined(WINCE) || defined(_WINCE) || \
    defined(__TOS_WIN__) || \
    defined(__WIN32__) || defined(__TOS_WIN__) || \
    defined(__WINDOWS__)
#define __LMDB_PLATFORM_WINDOWS
#endif

#if defined(LMDB_EXPORT_API)
# if defined(__LMDB_PLATFORM_WINDOWS)
#  define LMDB_API __declspec(dllexport)
# elif __LMDB_GNUC_FULLVERSION >= 40000
#  define LMDB_API __attribute__((visibility("default")))
# else
#  define LMDB_API
# endif
#else /* defined(LMDB_EXPORT_API) */
# if defined(__LMDB_PLATFORM_WINDOWS)
#  define LMDB_API __declspec(dllimport)
# else
#  define LMDB_API
# endif
#endif /* defined(LMDB_EXPORT_API) */

#endif /* __INCLUDE_LMDB__LMDB_EXPORT_API_H__ */
