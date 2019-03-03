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

#include <libtbag/macro/platform.hpp>
#include <libtbag/macro/compiler.hpp>

#ifndef TBAG_ATTRIBUTE_FORCE_INLINE
# if defined(TBAG_COMP_GNUC_CXX) && (TBAG_COMP_GNUC_VERSION >= 30100)
#  define TBAG_ATTRIBUTE_FORCE_INLINE __attribute__((always_inline)) inline
# elif defined(TBAG_COMP_MSVC)
#  define TBAG_ATTRIBUTE_FORCE_INLINE __forceinline
# else
#  define TBAG_ATTRIBUTE_FORCE_INLINE inline
# endif
#endif

#ifndef TBAG_ATTRIBUTE_DEPRECATED
# if defined(TBAG_COMP_GNUC_CXX) && (TBAG_COMP_GNUC_VERSION >= 30100)
#  define TBAG_ATTRIBUTE_DEPRECATED __attribute__((deprecated))
# elif defined(TBAG_COMP_MSVC)
#  define TBAG_ATTRIBUTE_DEPRECATED __declspec(deprecated)
# else
#  define TBAG_ATTRIBUTE_DEPRECATED
# endif
#endif

#ifndef TBAG_ATTRIBUTE_NORETURN
# if defined(TBAG_COMP_GNUC_CXX) && (TBAG_COMP_GNUC_VERSION >= 20500)
#  define TBAG_ATTRIBUTE_NORETURN __attribute__((noreturn))
# else
#  define TBAG_ATTRIBUTE_NORETURN
# endif
#endif

#ifndef TBAG_ATTRIBUTE_ALIGN
# if defined(TBAG_COMP_GNUC_CXX)
#  define TBAG_ATTRIBUTE_ALIGN(n) __attribute__((__aligned__(n)))
# elif defined(TBAG_COMP_MSVC)
#  define TBAG_ATTRIBUTE_ALIGN(n) __declspec(align(n))
# else
#  define TBAG_ATTRIBUTE_ALIGN(n)
# endif
# define TBAG_ALIGNMENT_SIZE_1BYTE        1
# define TBAG_ALIGNMENT_SIZE_2BYTE        2
# define TBAG_ALIGNMENT_SIZE_4BYTE        4
# define TBAG_ALIGNMENT_SIZE_8BYTE        8
# define TBAG_ALIGNMENT_SIZE_16BYTE      16
# define TBAG_ALIGNMENT_SIZE_32BYTE      32
# define TBAG_ALIGNMENT_SIZE_64BYTE      64
# define TBAG_ALIGNMENT_SIZE_128BYTE    128
# define TBAG_ALIGNMENT_SIZE_256BYTE    256
# define TBAG_ALIGNMENT_SIZE_512BYTE    512
# define TBAG_ALIGNMENT_SIZE_1024BYTE  1024
# define TBAG_ALIGNMENT_SIZE_2048BYTE  2048
# define TBAG_ALIGNMENT_SIZE_4096BYTE  4096
# define TBAG_ALIGNMENT_DEFAULT_SIMD_M128_SIZE   TBAG_ALIGNMENT_SIZE_16BYTE
# define TBAG_ALIGNMENT_DEFAULT_CACHE_LINE_SIZE  TBAG_ALIGNMENT_SIZE_64BYTE
# define TBAG_ALIGNMENT_DEFAULT_PAGE_SIZE        TBAG_ALIGNMENT_SIZE_4096BYTE
#endif

#if defined(TBAG_COMP_MSVC)
#define TBAG_ALIGNED_STRUCT_BEGIN(alignment) \
    __pragma(pack(1)); struct __declspec(align(alignment))
  #define TBAG_ALIGNED_STRUCT_END(name, size) \
    __pragma(pack()); static_assert(sizeof(name) == size, "compiler breaks packing rules")
#elif defined(TBAG_COMP_GNUC) || defined(TBAG_COMP_CLANG)
# define TBAG_ALIGNED_STRUCT_BEGIN(alignment) \
    _Pragma("pack(1)") struct __attribute__((aligned(alignment)))
# define TBAG_ALIGNED_STRUCT_END(name, size) \
    _Pragma("pack()") static_assert(sizeof(name) == size, "compiler breaks packing rules")
#else
# warning Unknown compiler, please define structure alignment macros
# define TBAG_ALIGNED_STRUCT_BEGIN(alignment) struct
# define TBAG_ALIGNED_STRUCT_END(name, size)
#endif

#ifndef TBAG_ATTRIBUTE_PUSH_NO_WARNING_DEPRECATED
# if defined(TBAG_COMP_GNUC_CXX) /*&& (TBAG_COMP_GNUC_VERSION >= 40600)*/
#  define TBAG_ATTRIBUTE_PUSH_NO_WARNING_DEPRECATED                 \
    _Pragma("GCC diagnostic push")                                  \
    _Pragma("GCC diagnostic ignored \"-Wdeprecated-declarations\"")
# elif defined(TBAG_COMP_MSVC)
#  define TBAG_ATTRIBUTE_PUSH_NO_WARNING_DEPRECATED \
    __pragma(warning(push))                         \
    __pragma(warning(disable : 4996))
# else
#  define TBAG_ATTRIBUTE_PUSH_NO_WARNING_DEPRECATED
# endif
#endif

#ifndef TBAG_ATTRIBUTE_PUSH_NO_WARNING_REORDER
# if defined(TBAG_COMP_CLANG)
#  define TBAG_ATTRIBUTE_PUSH_NO_WARNING_REORDER    \
    _Pragma("GCC diagnostic push")                  \
    _Pragma("GCC diagnostic ignored \"-Wreorder\"")
# elif defined(TBAG_COMP_GNUC_CXX)
#  define TBAG_ATTRIBUTE_PUSH_NO_WARNING_REORDER
# elif defined(TBAG_COMP_MSVC)
#  define TBAG_ATTRIBUTE_PUSH_NO_WARNING_REORDER
# else
#  define TBAG_ATTRIBUTE_PUSH_NO_WARNING_REORDER
# endif
#endif

#ifndef TBAG_ATTRIBUTE_DIAGNOSTIC_POP
# if defined(TBAG_COMP_GNUC_CXX)
#  define TBAG_ATTRIBUTE_DIAGNOSTIC_POP  _Pragma("GCC diagnostic pop")
# elif defined(TBAG_COMP_MSVC)
#  define TBAG_ATTRIBUTE_DIAGNOSTIC_POP  __pragma(warning(pop))
# else
#  define TBAG_ATTRIBUTE_DIAGNOSTIC_POP
# endif
#endif

#ifndef TBAG_PUSH_MACRO
# if defined(TBAG_COMP_GNUC_CXX)
#  define TBAG_PUSH_MACRO(name) _Pragma("push_macro(\"" #name "\")")
# elif defined(TBAG_COMP_MSVC)
#  define TBAG_PUSH_MACRO(name) __pragma(push_macro("\"" #name "\""))
# else
#  define TBAG_PUSH_MACRO(name)
# endif
#endif

#ifndef TBAG_POP_MACRO
# if defined(TBAG_COMP_GNUC_CXX)
#  define TBAG_POP_MACRO(name) _Pragma("pop_macro(\"" #name "\")")
# elif defined(TBAG_COMP_MSVC)
#  define TBAG_POP_MACRO(name) __pragma(pop_macro("\"" #name "\""))
# else
#  define TBAG_POP_MACRO(name)
# endif
#endif

#if defined(TBAG_EXPORT_API)
# if defined(TBAG_PLATFORM_WINDOWS)
#  define TBAG_API __declspec(dllexport)
# elif defined(TBAG_COMP_GNUC) && (TBAG_COMP_GNUC_VERSION >= 40000)
#  define TBAG_API __attribute__((visibility("default")))
# else
#  define TBAG_API
# endif
#else // defined(TBAG_EXPORT_API)
# if defined(TBAG_PLATFORM_WINDOWS)
#  define TBAG_API __declspec(dllimport)
# else
#  define TBAG_API
# endif
#endif // defined(TBAG_EXPORT_API)

#if defined(TBAG_EXPORT_API) && defined(TBAG_COMP_GNUC)
# define TBAG_CONSTRUCTOR __attribute__((constructor))
# define TBAG_DESTRUCTOR  __attribute__((destructor))
#else // defined(TBAG_EXPORT_API)
# define TBAG_CONSTRUCTOR
# define TBAG_DESTRUCTOR
#endif // defined(TBAG_EXPORT_API)

#ifndef TBAG_FALLTHROUGH
# if defined(TBAG_LANG_STDCPP_17)
#  define TBAG_FALLTHROUGH [[fallthrough]]
# else
#  define TBAG_FALLTHROUGH
# endif
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MACRO_ATTRIBUTES_HPP__

