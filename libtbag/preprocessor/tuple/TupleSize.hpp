/**
 * @file   TupleSize.hpp
 * @brief  TupleSize preprocessor prototype.
 * @author zer0
 * @date   2017-11-01
 *
 * @see <http://www.boost.org/doc/libs/1_63_0/libs/preprocessor/doc/index.html>
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_TUPLE_TUPLESIZE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_TUPLE_TUPLESIZE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <libtbag/preprocessor/details/Cat.hpp>
#include <libtbag/preprocessor/variadic/VariadicSize.hpp>

#if defined(TBAG_COMP_MSVC)
# define TBAG_PP_TUPLE_SIZE(tuple) TBAG_PP_CAT(TBAG_PP_VARIADIC_SIZE tuple,)
#else
# define TBAG_PP_TUPLE_SIZE(tuple) TBAG_PP_VARIADIC_SIZE tuple
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_TUPLE_TUPLESIZE_HPP__

