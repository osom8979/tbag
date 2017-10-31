/**
 * @file   Complement.hpp
 * @brief  Complement preprocessor prototype.
 * @author zer0
 * @date   2017-10-31
 *
 * @see <http://www.boost.org/doc/libs/1_63_0/libs/preprocessor/doc/index.html>
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_LOGICAL_COMPLEMENT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_LOGICAL_COMPLEMENT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#define TBAG_PP_COMPL(x) TBAG_PP_COMPL_I(x)

#if defined(TBAG_COMP_MSVC)
# define TBAG_PP_COMPL_I(x) TBAG_PP_COMPL_ID(TBAG_PP_COMPL_ ## x)
# define TBAG_PP_COMPL_ID(id) id
#else
# define TBAG_PP_COMPL_I(x) TBAG_PP_COMPL_ ## x
#endif

#define TBAG_PP_COMPL_0 1
#define TBAG_PP_COMPL_1 0

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_LOGICAL_COMPLEMENT_HPP__

