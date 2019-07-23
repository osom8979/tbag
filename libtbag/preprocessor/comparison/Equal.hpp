/**
 * @file   Equal.hpp
 * @brief  Equal class prototype.
 * @author zer0
 * @date   2019-07-23
 *
 * @see <http://www.boost.org/doc/libs/1_63_0/libs/preprocessor/doc/index.html>
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_COMPARISON_EQUAL_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_COMPARISON_EQUAL_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/preprocessor/comparison/NotEqual.hpp>
#include <libtbag/preprocessor/logical/Complement.hpp>

#define TBAG_PP_EQUAL(x, y) TBAG_PP_COMPL(TBAG_PP_NOT_EQUAL(x, y))

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_COMPARISON_EQUAL_HPP__

