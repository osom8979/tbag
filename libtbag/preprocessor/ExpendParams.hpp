/**
 * @file   ExpendParams.hpp
 * @brief  ExpendParams preprocessor prototype.
 * @author zer0
 * @date   2017-11-02
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_EXPENDPARAMS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_EXPENDPARAMS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/preprocessor/control/If.hpp>
#include <libtbag/preprocessor/details/Cat.hpp>
#include <libtbag/preprocessor/punctuation/CommaIf.hpp>
#include <libtbag/preprocessor/repetition/Repeat2.hpp>
#include <libtbag/preprocessor/tuple/TupleElem.hpp>
#include <libtbag/preprocessor/variadic/VariadicSize.hpp>
#include <libtbag/preprocessor/variadic/VariadicToTuple.hpp>

#define TBAG_PP_EXPEND_TYPES_MACRO(z, c, n, d)   TBAG_PP_COMMA_IF(n) TBAG_PP_TUPLE_ELEM(c, n, d)
#define TBAG_PP_EXPEND_VALUES_MACRO(z, c, n, d)  TBAG_PP_COMMA_IF(n) TBAG_PP_CAT(v, n)
#define TBAG_PP_EXPEND_PARAMS_MACRO(z, c, n, d)  TBAG_PP_COMMA_IF(n) TBAG_PP_TUPLE_ELEM(c, n, d) TBAG_PP_CAT(v, n)

#define TBAG_PP_EXPEND_TYPES(...)              \
    TBAG_PP_REPEAT2(                           \
        TBAG_PP_VARIADIC_SIZE(__VA_ARGS__),    \
        TBAG_PP_EXPEND_TYPES_MACRO,            \
        TBAG_PP_VARIADIC_TO_TUPLE(__VA_ARGS__) \
    )

#define TBAG_PP_EXPEND_VALUES(...)             \
    TBAG_PP_REPEAT2(                           \
        TBAG_PP_VARIADIC_SIZE(__VA_ARGS__),    \
        TBAG_PP_EXPEND_VALUES_MACRO,           \
        TBAG_PP_VARIADIC_TO_TUPLE(__VA_ARGS__) \
    )

#define TBAG_PP_EXPEND_PARAMS(...)             \
    TBAG_PP_REPEAT2(                           \
        TBAG_PP_VARIADIC_SIZE(__VA_ARGS__),    \
        TBAG_PP_EXPEND_PARAMS_MACRO,           \
        TBAG_PP_VARIADIC_TO_TUPLE(__VA_ARGS__) \
    )

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_EXPENDPARAMS_HPP__

