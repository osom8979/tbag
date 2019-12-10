/**
 * @file   function.hpp
 * @brief  function class prototype.
 * @author zer0
 * @date   2019-12-10
 *
 * @see <https://www.boost.org/doc/libs/1_70_0/boost/current_function.hpp>
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MACRO_FUNCTION_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MACRO_FUNCTION_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

inline void __tbag_current_function_helper()
{
#if defined(TBAG_DISABLE_CURRENT_FUNCTION)
# define TBAG_CURRENT_FUNCTION "(unknown)"
#elif defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__) || defined(__clang__)
# define TBAG_CURRENT_FUNCTION __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
# define TBAG_CURRENT_FUNCTION __PRETTY_FUNCTION__
#elif defined(__FUNCSIG__)
# define TBAG_CURRENT_FUNCTION __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
# define TBAG_CURRENT_FUNCTION __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
# define TBAG_CURRENT_FUNCTION __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
# define TBAG_CURRENT_FUNCTION __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
# define TBAG_CURRENT_FUNCTION __func__
#else
# define TBAG_CURRENT_FUNCTION "(unknown)"
#endif
}

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MACRO_FUNCTION_HPP__

