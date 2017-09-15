/**
 * @file   ExpendParams.hpp
 * @brief  ExpendParams preprocessor prototype.
 * @author zer0
 * @date   2017-09-15
 *
 * @see <http://www.boost.org/doc/libs/1_63_0/libs/preprocessor/doc/index.html>
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_DETAILS_EXPENDPARAMS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_DETAILS_EXPENDPARAMS_HPP__

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

#define TBAG_PP_EXPEND_PARAMS(...)             \
    TBAG_PP_REPEAT(                            \
        TBAG_PP_VARIADIC_SIZE(__VA_ARGS__),    \
        TBAG_PP_EXPEND_PARAMS_MACRO_IMPL,      \
        TBAG_PP_VARIADIC_TO_TUPLE(__VA_ARGS__) \
    )

#define TBAG_PP_EXPEND_PARAMS_MACRO_IMPL(z, c, n, d)  TBAG_PP_COMMA_IF(n) TBAG_PP_TUPLE_ELEM(c, n, d) TBAG_PP_CAT(v, n)

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_DETAILS_EXPENDPARAMS_HPP__

