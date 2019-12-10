/**
 * @file   debug_stamp.hpp
 * @brief  debug_stamp class prototype.
 * @author zer0
 * @date   2019-12-10
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MACRO_DEBUG_STAMP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MACRO_DEBUG_STAMP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/macro/utils.hpp>
#include <libtbag/macro/function.hpp>

#ifndef TBAG_DEBUG_STAMP
#define TBAG_DEBUG_STAMP __LOCATION__
#endif

#ifndef TBAG_DEBUG_STAMP_SPACE
#define TBAG_DEBUG_STAMP_SPACE __LOCATION__ " "
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MACRO_DEBUG_STAMP_HPP__

