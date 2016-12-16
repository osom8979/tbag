/**
 * @file   ErrorCode.hpp
 * @brief  ErrorCode class prototype.
 * @author zer0
 * @date   2016-12-14
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_DEBUG_ERRORCODE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_DEBUG_ERRORCODE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace debug {

#ifndef TBAG_ERROR_INFO_MAP
#define TBAG_ERROR_INFO_MAP(_TBAG_XX)   \
    _TBAG_XX(SUCCESS, No error)         \
    _TBAG_XX(FAILURE, Unknown error)    \
    /* locale/Convert */                                \
    _TBAG_XX(LOCALE_CONVERTER_ERROR, UConverter error)  \
    /* -- END -- */
#endif

/** Native type of error code. */
typedef int CodeType;

/**
 * List of error code.
 *
 * @author zer0
 * @date   2016-12-14
 */
enum class ErrorCode : CodeType
{
    _ERROR_CODE_START_NUMBER_ = -1,
#define _TBAG_XX(name, message) name,
    TBAG_ERROR_INFO_MAP(_TBAG_XX)
#undef _TBAG_XX
    ERROR_CODE_SIZE
};

TBAG_API char const * getErrorMessage(ErrorCode code);

inline char const * getErrorMessage(CodeType code)
{
    return getErrorMessage(static_cast<ErrorCode>(code));
}

} // namespace debug

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_DEBUG_ERRORCODE_HPP__

