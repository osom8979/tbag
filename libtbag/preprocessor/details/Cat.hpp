/**
 * @file   Cat.hpp
 * @brief  Cat preprocessor prototype.
 * @author zer0
 * @date   2017-09-14
 *
 * @see <http://www.boost.org/doc/libs/1_63_0/libs/preprocessor/doc/index.html>
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_DETAILS_CAT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_DETAILS_CAT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#define TBAG_PP_CAT(a, b)  TBAG_PP_CAT_I(a, b)

#if defined(TBAG_COMP_MSVC)
# define TBAG_PP_CAT_I(a, b)     TBAG_PP_CAT_II(~, a ## b)
# define TBAG_PP_CAT_II(p, res)  res
#else
# define TBAG_PP_CAT_I(a, b)     a ## b
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_DETAILS_CAT_HPP__

