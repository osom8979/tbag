/**
 * @file   predef.hpp
 * @brief  pre-define header.
 * @author zer0
 * @date   2016-04-18
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PREDEF_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PREDEF_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

// Operating System.
#include <libtbag/macro/os/android.hpp>
#include <libtbag/macro/os/apple.hpp>
#include <libtbag/macro/os/cygwin.hpp>
#include <libtbag/macro/os/linux.hpp>
#include <libtbag/macro/os/unix.hpp>
#include <libtbag/macro/os/windows.hpp>

// Compiler.
#include <libtbag/macro/compiler/gcc.hpp>
#include <libtbag/macro/compiler/clang.hpp>
#include <libtbag/macro/compiler/llvm.hpp>
#include <libtbag/macro/compiler/msvc.hpp>

// Language.
#include <libtbag/macro/language/c.hpp>
#include <libtbag/macro/language/cpp.hpp>
#include <libtbag/macro/language/objc.hpp>

// Common.
#include <libtbag/macro/attributes.hpp>
#include <libtbag/macro/features.hpp>
#include <libtbag/macro/utils.hpp>

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PREDEF_HPP__

