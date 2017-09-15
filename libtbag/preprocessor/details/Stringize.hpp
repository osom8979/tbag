/**
 * @file   Stringize.hpp
 * @brief  Stringize preprocessor prototype.
 * @author zer0
 * @date   2017-09-15
 *
 * @see <http://www.boost.org/doc/libs/1_63_0/libs/preprocessor/doc/index.html>
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_DETAILS_STRINGIZE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_DETAILS_STRINGIZE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#if defined(TBAG_COMP_MSVC)
# define TBAG_PP_STRINGIZE(text)   TBAG_PP_STRINGIZE_A((text))
# define TBAG_PP_STRINGIZE_A(arg)  TBAG_PP_STRINGIZE_I arg
#else
# define TBAG_PP_STRINGIZE(text)   TBAG_PP_STRINGIZE_I(text)
#endif

#define TBAG_PP_STRINGIZE_I(text)  #text

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_DETAILS_STRINGIZE_HPP__

