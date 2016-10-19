/**
 * @file   attributes.hpp
 * @brief  attributes utilities.
 * @author zer0
 * @date   2016-07-22
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MACRO_ATTRIBUTES_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MACRO_ATTRIBUTES_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

// OS.
#include <libtbag/macro/os/windows.hpp>
#include <libtbag/macro/os/cygwin.hpp>

// Compiler.
#include <libtbag/macro/compiler/gcc.hpp>
#include <libtbag/macro/compiler/clang.hpp>
#include <libtbag/macro/compiler/llvm.hpp>
#include <libtbag/macro/compiler/msvc.hpp>

#ifndef ATTRIBUTE_FORCE_INLINE
# if defined(__COMP_GNUC_CXX__) && (__COMP_GNUC_VERSION__ >= 30100)
#  define ATTRIBUTE_FORCE_INLINE __attribute__((always_inline)) inline
# elif defined(__COMP_MSVC__)
#  define ATTRIBUTE_FORCE_INLINE __forceinline
# else
#  define ATTRIBUTE_FORCE_INLINE inline
# endif
#endif

#ifndef ATTRIBUTE_DEPRECATED
# if defined(__COMP_GNUC_CXX__) && (__COMP_GNUC_VERSION__ >= 30100)
#  define ATTRIBUTE_DEPRECATED __attribute__((deprecated))
# elif defined(__COMP_MSVC__)
#  define ATTRIBUTE_DEPRECATED __declspec(deprecated)
# else
#  define ATTRIBUTE_DEPRECATED
# endif
#endif

#ifndef ATTRIBUTE_NORETURN
# if defined(__COMP_GNUC_CXX__) && (__COMP_GNUC_VERSION__ >= 20500)
#  define ATTRIBUTE_NORETURN __attribute__((noreturn))
# else
#  define ATTRIBUTE_NORETURN
# endif
#endif

#ifndef ATTRIBUTE_NO_WARNING_DEPRECATED
# if defined(__COMP_GNUC_CXX__) && (__COMP_GNUC_VERSION__ >= 40600)
#  define ATTRIBUTE_NO_WARNING_DEPRECATED(code)                     \
    _Pragma("GCC diagnostic push")                                  \
    _Pragma("GCC diagnostic ignored \"-Wdeprecated-declarations\"") \
    code;                                                           \
    _Pragma("GCC diagnostic pop")
# elif defined(__COMP_MSVC__)
#  define ATTRIBUTE_NO_WARNING_DEPRECATED(code) \
    __pragma(warning(push))                     \
    __pragma(warning(disable : 4996))           \
    code;                                       \
    __pragma(warning(pop))
# else
#  define ATTRIBUTE_NO_WARNING_DEPRECATED(code)
# endif
#endif

#if defined(TBAG_EXPORT_API)
# if defined(__OS_WINDOWS__)
#  define TBAG_EXPORTS __declspec(dllexport)
# elif defined(__COMP_GNUC__) && (__COMP_GNUC_VERSION__ >= 40000)
#  define TBAG_EXPORTS __attribute__((visibility("default")))
# else
#  define TBAG_EXPORTS
# endif
#else // defined(TBAG_EXPORT_API)
# if defined(__OS_WINDOWS__)
#  define TBAG_EXPORTS __declspec(dllimport)
# else
#  define TBAG_EXPORTS
# endif
#endif // defined(TBAG_EXPORT_API)

#if defined(TBAG_EXPORT_API) && defined(__COMP_GNUC__)
# define TBAG_CONSTRUCTOR __attribute__((constructor))
# define TBAG_DESTRUCTOR  __attribute__((destructor))
#else // defined(TBAG_EXPORT_API)
# define TBAG_CONSTRUCTOR
# define TBAG_DESTRUCTOR
#endif // defined(TBAG_EXPORT_API)

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MACRO_ATTRIBUTES_HPP__

