/**
 * @file   types.hpp
 * @brief  common macro of types.
 * @author zer0
 * @date   2018-01-08
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MACRO_TYPES_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MACRO_TYPES_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef TB_NULL
# if __cplusplus >= 201103L
#  define TB_NULL nullptr
# else
#  define TB_NULL 0
# endif
#endif

#ifndef TB_BOOL
typedef int tbBOOL;
#define TB_BOOL tbBOOL
#endif

#ifndef TB_TRUE
#define TB_TRUE 1
#endif

#ifndef TB_FALSE
#define TB_FALSE 0
#endif

#ifndef TB_LOCALE_NAME_MAX_LENGTH
#define TB_LOCALE_NAME_MAX_LENGTH 85 // Win32 API.
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MACRO_TYPES_HPP__

