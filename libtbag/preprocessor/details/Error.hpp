/**
 * @file   Error.hpp
 * @brief  Error preprocessor prototype.
 * @author zer0
 * @date   2017-09-15
 *
 * @see <http://www.boost.org/doc/libs/1_63_0/libs/preprocessor/doc/index.html>
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_DETAILS_ERROR_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_DETAILS_ERROR_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/preprocessor/details/Cat.hpp>

#define TBAG_PP_ERROR(code)   TBAG_PP_CAT(TBAG_PP_ERROR_, code)

#define TBAG_PP_ERROR_0x0000  TBAG_PP_ERROR(0x0000, TBAG_PP_INDEX_OUT_OF_BOUNDS)
#define TBAG_PP_ERROR_0x0001  TBAG_PP_ERROR(0x0001, TBAG_PP_WHILE_OVERFLOW)
#define TBAG_PP_ERROR_0x0002  TBAG_PP_ERROR(0x0002, TBAG_PP_FOR_OVERFLOW)
#define TBAG_PP_ERROR_0x0003  TBAG_PP_ERROR(0x0003, TBAG_PP_REPEAT_OVERFLOW)
#define TBAG_PP_ERROR_0x0004  TBAG_PP_ERROR(0x0004, TBAG_PP_LIST_FOLD_OVERFLOW)
#define TBAG_PP_ERROR_0x0005  TBAG_PP_ERROR(0x0005, TBAG_PP_SEQ_FOLD_OVERFLOW)
#define TBAG_PP_ERROR_0x0006  TBAG_PP_ERROR(0x0006, TBAG_PP_ARITHMETIC_OVERFLOW)
#define TBAG_PP_ERROR_0x0007  TBAG_PP_ERROR(0x0007, TBAG_PP_DIVISION_BY_ZERO)

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_DETAILS_ERROR_HPP__

