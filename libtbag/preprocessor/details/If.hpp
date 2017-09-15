/**
 * @file   If.hpp
 * @brief  If preprocessor prototype.
 * @author zer0
 * @date   2017-09-14
 *
 * @see <http://www.boost.org/doc/libs/1_63_0/libs/preprocessor/doc/index.html>
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_DETAILS_IF_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_DETAILS_IF_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/preprocessor/details/Boolean.hpp>

// --------
// IF Part.
// --------

#define TBAG_PP_IF(cond, t, f)  TBAG_PP_IIF(TBAG_PP_BOOL(cond), t, f)

// ---------
// IIF Part.
// ---------

#define TBAG_PP_IIF(bit, t, f)     TBAG_PP_IIF_I(bit, t, f)

#if defined(TBAG_COMP_MSVC)
# define TBAG_PP_IIF_I(bit, t, f)  TBAG_PP_IIF_II(TBAG_PP_IIF_ ## bit(t, f))
# define TBAG_PP_IIF_II(id) id
#else
# define TBAG_PP_IIF_I(bit, t, f)  TBAG_PP_IIF_ ## bit(t, f)
#endif

#define TBAG_PP_IIF_0(t, f)  f
#define TBAG_PP_IIF_1(t, f)  t

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_DETAILS_IF_HPP__

