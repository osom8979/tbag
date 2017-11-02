/**
 * @file   IsBeginParens.hpp
 * @brief  IsBeginParens preprocessor prototype.
 * @author zer0
 * @date   2017-11-02
 *
 * @see <http://www.boost.org/doc/libs/1_63_0/libs/preprocessor/doc/index.html>
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_PUNCTUATION_ISBEGINPARENS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_PUNCTUATION_ISBEGINPARENS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <libtbag/preprocessor/facilities/Empty.hpp>

// --------
// DETAILS.
// --------

#if defined(TBAG_COMP_MSVC)
# define TBAG_PP_DETAIL_VD_IBP_CAT(a, b)         TBAG_PP_DETAIL_VD_IBP_CAT_I(a, b)
# define TBAG_PP_DETAIL_VD_IBP_CAT_I(a, b)       TBAG_PP_DETAIL_VD_IBP_CAT_II(a ## b)
# define TBAG_PP_DETAIL_VD_IBP_CAT_II(res)       res
# define TBAG_PP_DETAIL_IBP_SPLIT(i, ...)        TBAG_PP_DETAIL_VD_IBP_CAT(TBAG_PP_DETAIL_IBP_PRIMITIVE_CAT(TBAG_PP_DETAIL_IBP_SPLIT_,i)(__VA_ARGS__),TBAG_PP_EMPTY())
# define TBAG_PP_DETAIL_IBP_IS_VARIADIC_C(...)   1 1
#else
# define TBAG_PP_DETAIL_IBP_SPLIT(i, ...)        TBAG_PP_DETAIL_IBP_PRIMITIVE_CAT(TBAG_PP_DETAIL_IBP_SPLIT_,i)(__VA_ARGS__)
# define TBAG_PP_DETAIL_IBP_IS_VARIADIC_C(...)   1
#endif

#define TBAG_PP_DETAIL_IBP_SPLIT_0(a, ...)  a
#define TBAG_PP_DETAIL_IBP_SPLIT_1(a, ...)  __VA_ARGS__

#define TBAG_PP_DETAIL_IBP_CAT(a, ...)             TBAG_PP_DETAIL_IBP_PRIMITIVE_CAT(a,__VA_ARGS__)
#define TBAG_PP_DETAIL_IBP_PRIMITIVE_CAT(a, ...)   a ## __VA_ARGS__

#define TBAG_PP_DETAIL_IBP_IS_VARIADIC_R_1                                  1,
#define TBAG_PP_DETAIL_IBP_IS_VARIADIC_R_TBAG_PP_DETAIL_IBP_IS_VARIADIC_C   0,

// ----------------
// IS_BEGIN_PARENS.
// ----------------

#if defined(TBAG_COMP_MSVC) && TBAG_COMP_MSVC_VERSION <= 1400
# define TBAG_PP_IS_BEGIN_PARENS(param)             \
    TBAG_PP_DETAIL_IBP_SPLIT                        \
    (                                               \
        0,                                          \
        TBAG_PP_DETAIL_IBP_CAT                      \
        (                                           \
            TBAG_PP_DETAIL_IBP_IS_VARIADIC_R_,      \
            TBAG_PP_DETAIL_IBP_IS_VARIADIC_C param  \
        )                                           \
    )
#else
# define TBAG_PP_IS_BEGIN_PARENS(...)                       \
    TBAG_PP_DETAIL_IBP_SPLIT                                \
    (                                                       \
        0,                                                  \
        TBAG_PP_DETAIL_IBP_CAT                              \
        (                                                   \
            TBAG_PP_DETAIL_IBP_IS_VARIADIC_R_,              \
            TBAG_PP_DETAIL_IBP_IS_VARIADIC_C __VA_ARGS__    \
        )                                                   \
    )
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_PUNCTUATION_ISBEGINPARENS_HPP__

