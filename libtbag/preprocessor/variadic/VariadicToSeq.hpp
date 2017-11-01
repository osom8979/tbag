/**
 * @file   VariadicToSeq.hpp
 * @brief  VariadicToSeq preprocessor prototype.
 * @author zer0
 * @date   2017-10-31
 *
 * @see <http://www.boost.org/doc/libs/1_63_0/libs/preprocessor/doc/index.html>
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_VARIADIC_VARIADICTOSEQ_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_VARIADIC_VARIADICTOSEQ_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/preprocessor/tuple/TupleToSeq.hpp>

#define TBAG_PP_VARIADIC_TO_SEQ(...) TBAG_PP_TUPLE_TO_SEQ((__VA_ARGS__))

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_VARIADIC_VARIADICTOSEQ_HPP__

