/**
 * @file   OddOrEven.hpp
 * @brief  OddOrEven preprocessor prototype.
 * @author zer0
 * @date   2017-11-02
 *
 * @see <http://www.boost.org/doc/libs/1_63_0/libs/preprocessor/doc/index.html>
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_NUMERIC_ODDOREVEN_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_NUMERIC_ODDOREVEN_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/preprocessor/logical/Boolean.hpp>
#include <libtbag/preprocessor/numeric/Odd.hpp>
#include <libtbag/preprocessor/numeric/Even.hpp>
#include <libtbag/preprocessor/control/If.hpp>

#define TBAG_PP_ODD_IF(c, t, f)     TBAG_PP_ODD_IF_I(c, t, f)
#define TBAG_PP_ODD_IF_I(c, t, f)   TBAG_PP_IF(TBAG_PP_BOOL(TBAG_PP_ODD(c)), t, f)

#define TBAG_PP_EVEN_IF(c, t, f)    TBAG_PP_EVEN_IF_I(c, t, f)
#define TBAG_PP_EVEN_IF_I(c, t, f)  TBAG_PP_IF(TBAG_PP_BOOL(TBAG_PP_EVEN(c)), t, f)

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_NUMERIC_ODDOREVEN_HPP__

