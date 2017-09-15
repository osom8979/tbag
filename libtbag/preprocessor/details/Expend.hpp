/**
 * @file   Expend.hpp
 * @brief  Expend preprocessor prototype.
 * @author zer0
 * @date   2017-09-15
 *
 * @see <http://www.boost.org/doc/libs/1_63_0/libs/preprocessor/doc/index.html>
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_DETAILS_EXPEND_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_DETAILS_EXPEND_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#define TBAG_PP_EXPAND(x)    TBAG_PP_EXPAND_I(x)
#define TBAG_PP_EXPAND_I(x)  x

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_DETAILS_EXPEND_HPP__

