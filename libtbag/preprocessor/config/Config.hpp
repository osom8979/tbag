/**
 * @file   Config.hpp
 * @brief  Config class prototype.
 * @author zer0
 * @date   2019-07-23
 *
 * @see <http://www.boost.org/doc/libs/1_63_0/libs/preprocessor/doc/index.html>
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_CONFIG_CONFIG_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_CONFIG_CONFIG_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#define TBAG_PP_CONFIG_STRICT() 0x0001
#define TBAG_PP_CONFIG_IDEAL()  0x0002
#define TBAG_PP_CONFIG_MSVC()   0x0004
#define TBAG_PP_CONFIG_MWCC()   0x0008
#define TBAG_PP_CONFIG_BCC()    0x0010
#define TBAG_PP_CONFIG_EDG()    0x0020
#define TBAG_PP_CONFIG_DMC()    0x0040

#ifndef TBAG_PP_CONFIG_FLAGS
# if defined(__GCCXML__) || defined(__WAVE__) || defined(__MWERKS__) && __MWERKS__ >= 0x3200
#  define TBAG_PP_CONFIG_FLAGS() (TBAG_PP_CONFIG_STRICT())
# elif defined(__EDG__) || defined(__EDG_VERSION__)
#  if defined(_MSC_VER) && !defined(__clang__) && (defined(__INTELLISENSE__) || __EDG_VERSION__ >= 308)
#   if !defined(_MSVC_TRADITIONAL) || _MSVC_TRADITIONAL
#     define TBAG_PP_CONFIG_FLAGS() (TBAG_PP_CONFIG_MSVC())
#   else
#     define TBAG_PP_CONFIG_FLAGS() (TBAG_PP_CONFIG_STRICT())
#   endif
#  else
#   define TBAG_PP_CONFIG_FLAGS() (TBAG_PP_CONFIG_EDG() | TBAG_PP_CONFIG_STRICT())
#  endif
# elif defined(_MSC_VER) && defined(__clang__)
#  define TBAG_PP_CONFIG_FLAGS() (TBAG_PP_CONFIG_STRICT())
# elif defined(__MWERKS__)
#  define TBAG_PP_CONFIG_FLAGS() (TBAG_PP_CONFIG_MWCC())
# elif defined(__DMC__)
#  define TBAG_PP_CONFIG_FLAGS() (TBAG_PP_CONFIG_DMC())
# elif defined(__BORLANDC__) && __BORLANDC__ >= 0x581
#  define TBAG_PP_CONFIG_FLAGS() (TBAG_PP_CONFIG_STRICT())
# elif defined(__BORLANDC__) || defined(__IBMC__) || defined(__IBMCPP__) || defined(__SUNPRO_CC)
#  define TBAG_PP_CONFIG_FLAGS() (TBAG_PP_CONFIG_BCC())
# elif defined(_MSC_VER)
#  if !defined(_MSVC_TRADITIONAL) || _MSVC_TRADITIONAL
#   define TBAG_PP_CONFIG_FLAGS() (TBAG_PP_CONFIG_MSVC())
#  else
#   define TBAG_PP_CONFIG_FLAGS() (TBAG_PP_CONFIG_STRICT())
#  endif
# else
#  define TBAG_PP_CONFIG_FLAGS() (TBAG_PP_CONFIG_STRICT())
# endif
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PREPROCESSOR_CONFIG_CONFIG_HPP__

