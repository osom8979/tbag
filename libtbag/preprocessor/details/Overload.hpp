/**
 * @file   Overload.hpp
 * @brief  Overload preprocessor prototype.
 * @author zer0
 * @date   2017-09-15
 *
 * @see <http://www.boost.org/doc/libs/1_63_0/libs/preprocessor/doc/index.html>
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_DETAILS_OVERLOAD_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_DETAILS_OVERLOAD_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/preprocessor/details/Cat.hpp>
#include <libtbag/preprocessor/variadic/VariadicSize.hpp>

#define TBAG_PP_OVERLOAD(prefix, ...)  TBAG_PP_CAT(prefix, TBAG_PP_VARIADIC_SIZE(__VA_ARGS__))

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_DETAILS_OVERLOAD_HPP__

