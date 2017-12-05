/**
 * @file   compiler.hpp
 * @brief  compiler class prototype.
 * @author zer0
 * @date   2016-12-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MACRO_COMPILER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MACRO_COMPILER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/macro/compiler/gcc.hpp>
#include <libtbag/macro/compiler/clang.hpp>
#include <libtbag/macro/compiler/llvm.hpp>
#include <libtbag/macro/compiler/msvc.hpp>

#if defined(TBAG_COMP_CLANG)
# define TBAG_ASM           __asm
# define TBAG_ASM_VOLATILE  __asm __volatile__
#elif defined(TBAG_COMP_MSVC)
# define TBAG_ASM           __asm
# define TBAG_ASM_VOLATILE  __asm
#else // GCC, ETC...
# define TBAG_ASM           __asm__
# define TBAG_ASM_VOLATILE  __asm__ __volatile__
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MACRO_COMPILER_HPP__

