/**
 * @file   intel.hpp
 * @brief  intel class prototype.
 * @author zer0
 * @date   2017-12-05
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MACRO_ARCHITECTURE_INTEL_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MACRO_ARCHITECTURE_INTEL_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#if defined(__i386__) || (_M_IX86 == 600)
#define TBAG_ARCH_X86
#endif

#if defined(__x86_64__) || (_M_X64 == 100)
#define TBAG_ARCH_X86_64
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MACRO_ARCHITECTURE_INTEL_HPP__

