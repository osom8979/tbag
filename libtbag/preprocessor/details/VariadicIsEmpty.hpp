/**
 * @file   VariadicIsEmpty.hpp
 * @brief  VariadicIsEmpty class prototype.
 * @author zer0
 * @date   2017-10-27
 *
 * @see <https://gustedt.wordpress.com/2010/06/08/detect-empty-macro-arguments/>
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_DETAILS_VARIADICISEMPTY_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_DETAILS_VARIADICISEMPTY_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#define TBAG_PP_VARIADIC_IS_EMPTY_ARGS64(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63, ...) e63
#define TBAG_PP_VIE_HAS_COMMA(...) TBAG_PP_VARIADIC_IS_EMPTY_ARGS64(__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0)
#define TBAG_PP_VIE_TRIGGER_PARENTHESIS(...) ,

#define TBAG_PP_VARIADIC_IS_EMPTY(...)                                                                      \
    TBAG_PP_VIE_I(                                                                                          \
        /* test if there is just one argument, eventually an empty one */                                   \
        TBAG_PP_VIE_HAS_COMMA(__VA_ARGS__),                                                                 \
        /* test if TBAG_PP_VIE_TRIGGER_PARENTHESIS together with the argument adds a comma */               \
        TBAG_PP_VIE_HAS_COMMA(TBAG_PP_VIE_TRIGGER_PARENTHESIS __VA_ARGS__),                                 \
        /* test if the argument together with a parenthesis adds a comma */                                 \
        TBAG_PP_VIE_HAS_COMMA(__VA_ARGS__ (/*empty*/)),                                                     \
        /* test if placing it between TBAG_PP_VIE_TRIGGER_PARENTHESIS and the parenthesis adds a comma */   \
        TBAG_PP_VIE_HAS_COMMA(TBAG_PP_VIE_TRIGGER_PARENTHESIS __VA_ARGS__ (/*empty*/))                      \
    )

#define TBAG_PP_VIE_CAT5(e0, e1, e2, e3, e4) e0 ## e1 ## e2 ## e3 ## e4
#define TBAG_PP_VIE_I(e0, e1, e2, e3) TBAG_PP_VIE_HAS_COMMA(TBAG_PP_VIE_CAT5(TBAG_PP_VIE_CASE_, e0, e1, e2, e3))
#define TBAG_PP_VIE_CASE_0001 ,

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_DETAILS_VARIADICISEMPTY_HPP__

