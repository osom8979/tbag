/**
 * @file   IsEmpty.hpp
 * @brief  IsEmpty preprocessor prototype.
 * @author zer0
 * @date   2017-11-02
 *
 * @see <http://www.boost.org/doc/libs/1_63_0/libs/preprocessor/doc/index.html>
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_FACILITIES_ISEMPTY_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_FACILITIES_ISEMPTY_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <libtbag/preprocessor/punctuation/IsBeginParens.hpp>

// --------
// DETAILS.
// --------

#if defined(TBAG_COMP_MSVC)
# pragma warning(once:4002)
# define TBAG_PP_DETAIL_IS_EMPTY_IIF_0(t, b) b
# define TBAG_PP_DETAIL_IS_EMPTY_IIF_1(t, b) t
#else
# define TBAG_PP_DETAIL_IS_EMPTY_IIF_0(t, ...) __VA_ARGS__
# define TBAG_PP_DETAIL_IS_EMPTY_IIF_1(t, ...) t
#endif

#if defined(TBAG_COMP_MSVC) && TBAG_COMP_MSVC_VERSION <= 1400
# define TBAG_PP_DETAIL_IS_EMPTY_PROCESS(param) TBAG_PP_IS_BEGIN_PARENS(TBAG_PP_DETAIL_IS_EMPTY_NON_FUNCTION_C param ())
#else
# define TBAG_PP_DETAIL_IS_EMPTY_PROCESS(...) TBAG_PP_IS_BEGIN_PARENS(TBAG_PP_DETAIL_IS_EMPTY_NON_FUNCTION_C __VA_ARGS__ ())
#endif

#define TBAG_PP_DETAIL_IS_EMPTY_PRIMITIVE_CAT(a, b) a ## b
#define TBAG_PP_DETAIL_IS_EMPTY_IIF(bit) TBAG_PP_DETAIL_IS_EMPTY_PRIMITIVE_CAT(TBAG_PP_DETAIL_IS_EMPTY_IIF_,bit)
#define TBAG_PP_DETAIL_IS_EMPTY_NON_FUNCTION_C(...) ()

// ---------
// IS_EMPTY.
// ---------

// NOT IMPLEMENTED.

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_FACILITIES_ISEMPTY_HPP__

