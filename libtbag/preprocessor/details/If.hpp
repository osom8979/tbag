/**
 * @file   If.hpp
 * @brief  If class prototype.
 * @author zer0
 * @date   2017-09-14
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_DETAILS_IF_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_DETAILS_IF_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/preprocessor/details/Boolean.hpp>

#ifndef TBAG_PP_IF
#define TBAG_PP_IF(cond, t, f) TBAG_PP_IF_IMPL(TBAG_PP_BOOL(cond), t, f)
#endif

#ifndef TBAG_PP_IF_IMPL
#define TBAG_PP_IF_IMPL(bit, t, f) TBAG_PP_IF_IMPL2(bit, t, f)
#endif

#ifndef TBAG_PP_IF_IMPL2
#define TBAG_PP_IF_IMPL2(bit, t, f) TBAG_PP_IF_IMPL_##bit(t, f)
#endif

#define TBAG_PP_IF_IMPL_0(t, f) f
#define TBAG_PP_IF_IMPL_1(t, f) t

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_DETAILS_IF_HPP__

