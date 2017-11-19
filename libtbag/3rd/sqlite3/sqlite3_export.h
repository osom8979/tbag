#ifndef __INCLUDE_SQLITE3__SQLITE_EXPORT_API_H__
#define __INCLUDE_SQLITE3__SQLITE_EXPORT_API_H__

#if defined(__GNUC__)
#define __SQLITE_GNUC_FULLVERSION \
    ( (__GNUC__            * 10000) \
    + (__GNUC_MINOR__      *   100) \
    + (__GNUC_PATCHLEVEL__ *     1) )
#else
#define __SQLITE_GNUC_FULLVERSION 0
#endif

#if defined(WIN32) || defined(_WIN32) || \
    defined(WIN64) || defined(_WIN64) || \
    defined(WINCE) || defined(_WINCE) || \
    defined(__TOS_WIN__) || \
    defined(__WIN32__) || defined(__TOS_WIN__) || \
    defined(__WINDOWS__)
#define __SQLITE_PLATFORM_WINDOWS
#endif

#if defined(SQLITE_EXPORT_API)
# if defined(__SQLITE_PLATFORM_WINDOWS)
#  define SQLITE_API __declspec(dllexport)
# elif __SQLITE_GNUC_FULLVERSION >= 40000
#  define SQLITE_API __attribute__((visibility("default")))
# else
#  define SQLITE_API
# endif
#else /* defined(SQLITE_EXPORT_API) */
# if defined(__SQLITE_PLATFORM_WINDOWS)
#  define SQLITE_API __declspec(dllimport)
# else
#  define SQLITE_API
# endif
#endif /* defined(SQLITE_EXPORT_API) */

#endif /* __INCLUDE_SQLITE3__SQLITE_EXPORT_API_H__ */
