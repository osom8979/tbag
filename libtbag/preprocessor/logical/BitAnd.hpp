/**
 * @file   BitAnd.hpp
 * @brief  BitAnd class prototype.
 * @author zer0
 * @date   2019-07-23
 *
 * @see <http://www.boost.org/doc/libs/1_63_0/libs/preprocessor/doc/index.html>
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_LOGICAL_BITAND_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_LOGICAL_BITAND_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#define TBAG_PP_BITAND(x, y) TBAG_PP_BITAND_I(x, y)

#if defined(TBAG_COMP_MSVC)
# define TBAG_PP_BITAND_I(x, y) TBAG_PP_BITAND_ID(TBAG_PP_BITAND_ ## x ## y)
# define TBAG_PP_BITAND_ID(res) res
#else
# define TBAG_PP_BITAND_I(x, y) TBAG_PP_BITAND_ ## x ## y
#endif

#define TBAG_PP_BITAND_00 0
#define TBAG_PP_BITAND_01 0
#define TBAG_PP_BITAND_10 0
#define TBAG_PP_BITAND_11 1

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_LOGICAL_BITAND_HPP__

