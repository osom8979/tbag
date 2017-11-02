/**
 * @file   VariadicIsEmpty.hpp
 * @brief  VariadicIsEmpty preprocessor prototype.
 * @author zer0
 * @date   2017-11-02
 *
 * @see <http://www.boost.org/doc/libs/1_63_0/libs/preprocessor/doc/index.html>
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_VARIADIC_VARIADICISEMPTY_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_VARIADIC_VARIADICISEMPTY_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/preprocessor/punctuation/IsBeginParens.hpp>
#include <libtbag/preprocessor/facilities/IsEmpty.hpp>

#if defined(TBAG_COMP_MSVC) && TBAG_COMP_MSVC_VERSION <= 1400
# define TBAG_PP_VARIADIC_IS_EMPTY(param)                     \
    TBAG_PP_DETAIL_IS_EMPTY_IIF                               \
    (TBAG_PP_IS_BEGIN_PARENS(param))                          \
    (TBAG_PP_IS_EMPTY_ZERO, TBAG_PP_DETAIL_IS_EMPTY_PROCESS)  \
    (param)
#else
# define TBAG_PP_VARIADIC_IS_EMPTY(...)                       \
    TBAG_PP_DETAIL_IS_EMPTY_IIF                               \
    (TBAG_PP_IS_BEGIN_PARENS(__VA_ARGS__))                    \
    (TBAG_PP_IS_EMPTY_ZERO, TBAG_PP_DETAIL_IS_EMPTY_PROCESS)  \
    (__VA_ARGS__)
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_VARIADIC_VARIADICISEMPTY_HPP__

