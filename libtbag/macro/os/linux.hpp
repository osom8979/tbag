/**
 * @file   linux.hpp
 * @brief  linux os config.
 * @author zer0
 * @date   2016-11-24
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MACRO_OS_LINUX_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MACRO_OS_LINUX_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#if defined(linux) || defined(__linux__) || defined(__linux)
#define __OS_LINUX__
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MACRO_OS_LINUX_HPP__

