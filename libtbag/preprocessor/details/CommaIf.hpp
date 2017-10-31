/**
 * @file   CommaIf.hpp
 * @brief  CommaIf preprocessor prototype.
 * @author zer0
 * @date   2017-09-14
 *
 * @see <http://www.boost.org/doc/libs/1_63_0/libs/preprocessor/doc/index.html>
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_DETAILS_COMMAIF_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_DETAILS_COMMAIF_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/preprocessor/logical/Boolean.hpp>
#include <libtbag/preprocessor/details/Comma.hpp>
#include <libtbag/preprocessor/details/Empty.hpp>

#define TBAG_PP_COMMA_IF(cond)  TBAG_PP_IF(cond, TBAG_PP_COMMA, TBAG_PP_EMPTY)()

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_DETAILS_COMMAIF_HPP__

