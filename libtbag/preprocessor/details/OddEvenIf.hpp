/**
 * @file   OddEvenIf.hpp
 * @brief  OddEvenIf preprocessor prototype.
 * @author zer0
 * @date   2017-09-14
 *
 * @see <http://www.boost.org/doc/libs/1_63_0/libs/preprocessor/doc/index.html>
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_DETAILS_ODDEVENIF_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_DETAILS_ODDEVENIF_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/preprocessor/details/Boolean.hpp>
#include <libtbag/preprocessor/details/Odd.hpp>
#include <libtbag/preprocessor/details/Even.hpp>
#include <libtbag/preprocessor/details/If.hpp>

#define TBAG_PP_ODD_IF(c, t, f)     TBAG_PP_ODD_IF_I(c, t, f)
#define TBAG_PP_ODD_IF_I(c, t, f)   TBAG_PP_IF(TBAG_PP_BOOL(TBAG_PP_ODD(c)), t, f)

#define TBAG_PP_EVEN_IF(c, t, f)    TBAG_PP_EVEN_IF_I(c, t, f)
#define TBAG_PP_EVEN_IF_I(c, t, f)  TBAG_PP_IF(TBAG_PP_BOOL(TBAG_PP_EVEN(c)), t, f)

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_DETAILS_ODDEVENIF_HPP__

