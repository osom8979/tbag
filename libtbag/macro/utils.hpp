/**
 * @file   utils.hpp
 * @brief  common macro utilities.
 * @author zer0
 * @date   2016-05-27
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MACRO_UTILS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MACRO_UTILS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

// --------------------------
// Inline information macros.
// --------------------------

#define TO_STRING_IMP(m) #m
#define TO_STRING(x) TO_STRING_IMP(x)

#define LINE_STRING TO_STRING(__LINE__)
#define FILE_STRING __FILE__
#define DATE_STRING __DATE__
#define TIME_STRING __TIME__

#define FUNC_STRING __PRETTY_FUNCTION__

#define __DATETIME__  DATE_STRING  "T" TIME_STRING
#define __LOCATION__  FILE_STRING  ":" LINE_STRING
#define DEBUG_STAMP   __DATETIME__ " " __LOCATION__

#define VERSION_SEPARATOR "."

// --------------
// Compare macros
// --------------

#define COMPARE_AND(x)      (x) && (x)
#define COMPARE_OR(x)       (x) || (x)
#define RANGE_OR(min, max)  (min) || (max)

// -------------
// Remove macros
// -------------

#define UNUSED_PARAM(x)
#define NAMELESS(x)
#define COMMENT(x)
#define IMPORTANT(x)
#define STEP(n,x)
#define BRACE(x)

#define BLOCK_DO(x)     do
#define BLOCK_END(x)    while (false)
#define BLOCK_OPEN(x)   BLOCK_DO(x) {
#define BLOCK_CLOSE(x)  } BLOCK_END(x);

#ifndef REMOVE_UNUSED_DECLARATION
/** Remove unused declaration warning. */
#define REMOVE_UNUSED_DECLARATION(name, x) \
    inline void __remove_##unused##name##__() \
    { x; }
#endif

#ifndef REMOVE_UNUSED_VARIABLE
/** Remove unused variables warning. */
#define REMOVE_UNUSED_VARIABLE(x) \
    if (x) { }
#endif

// -----------------
// Namespace macros.
// -----------------

#ifndef NAMESPACE_ANONYMOUS
# define NAMESPACE_ANONYMOUS
# if defined(__cplusplus)
#  define NAMESPACE_ANONYMOUS_OPEN   namespace {
#  define NAMESPACE_ANONYMOUS_CLOSE  }
# else /* __cplusplus */
#  define NAMESPACE_ANONYMOUS_OPEN
#  define NAMESPACE_ANONYMOUS_CLOSE
# endif /* __cplusplus */
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MACRO_UTILS_HPP__

