/**
 * @file   OddEvenIf.hpp
 * @brief  OddEvenIf class prototype.
 * @author zer0
 * @date   2017-09-14
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_DETAILS_ODDEVENIF_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_DETAILS_ODDEVENIF_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef TBAG_PP_ODD_IF
#define TBAG_PP_ODD_IF(c, t, f) TBAG_PP_ODD_IF_IMPL(c, t, f)
#endif

#ifndef TBAG_PP_ODD_IF_IMPL
#define TBAG_PP_ODD_IF_IMPL(c, t, f) TBAG_PP_IF(TBAG_PP_BOOL(TBAG_PP_ODD(c)), t, f)
#endif

#ifndef TBAG_PP_EVEN_IF
#define TBAG_PP_EVEN_IF(c, t, f) TBAG_PP_EVEN_IF_IMPL(c, t, f)
#endif

#ifndef TBAG_PP_EVEN_IF_IMPL
#define TBAG_PP_EVEN_IF_IMPL(c, t, f) TBAG_PP_IF(TBAG_PP_BOOL(TBAG_PP_EVEN(c)), t, f)
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_DETAILS_ODDEVENIF_HPP__

