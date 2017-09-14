/**
 * @file   Stringify.hpp
 * @brief  Stringify preprocessor prototype.
 * @author zer0
 * @date   2017-09-14
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_DETAILS_STRINGIFY_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_DETAILS_STRINGIFY_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef TBAG_PP_STRINGIFY
#define TBAG_PP_STRINGIFY(x) TBAG_PP_STRINGIFY_IMP(x)
#endif

#ifndef TBAG_PP_STRINGIFY_IMP
#define TBAG_PP_STRINGIFY_IMP(m) #m
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_DETAILS_STRINGIFY_HPP__

