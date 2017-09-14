/**
 * @file   ExtendParams.hpp
 * @brief  ExtendParams preprocessor prototype.
 * @author zer0
 * @date   2017-09-14
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_DETAILS_EXTENDPARAMS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_DETAILS_EXTENDPARAMS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/preprocessor/details/Cat.hpp>
#include <libtbag/preprocessor/details/CommaIf.hpp>
#include <libtbag/preprocessor/details/Repeat.hpp>
#include <libtbag/preprocessor/details/Tuple.hpp>
#include <libtbag/preprocessor/details/Variadic.hpp>
#include <libtbag/preprocessor/details/VariadicToTuple.hpp>

#ifndef TBAG_PP_EXTEND_PARAMS_MACRO_IMPL
#define TBAG_PP_EXTEND_PARAMS_MACRO_IMPL(s, n, d) TBAG_PP_COMMA_IF(n) TBAG_PP_TUPLE_ELEM(s, n, d) TBAG_PP_CAT(v, n)
#endif

#ifndef TBAG_PP_EXTEND_PARAMS
#define TBAG_PP_EXTEND_PARAMS(...)             \
    TBAG_PP_REPEAT(                            \
        TBAG_PP_VARIADIC_SIZE(__VA_ARGS__),    \
        TBAG_PP_EXTEND_PARAMS_MACRO_IMPL,      \
        TBAG_PP_VARIADIC_TO_TUPLE(__VA_ARGS__) \
    )
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_DETAILS_EXTENDPARAMS_HPP__

