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
#define TBAG_ERROR_INFO_MAP(_TBAG_XX)                 \
    _TBAG_XX(     SUCCESS, "No error"               ) \
    _TBAG_XX(     FAILURE, "Unknown error"          ) \
    _TBAG_XX( UNSUPPORTED, "Unsupported operation"  ) \
    _TBAG_XX(NULL_POINTER, "Nullpointer exception"  ) \
    _TBAG_XX(  READ_ERROR, "Data read error"        ) \
    _TBAG_XX(  BUSY_ERROR, "Busy error"             ) \
    /* Locale */ \
    _TBAG_XX(LOCALE_CONVERTER_ERROR, "UConverter error")  \
    /* File */ \
    _TBAG_XX(END_OF_FILE, "End of file")  \
    /* Network */ \
    _TBAG_XX(CONNECTION_RESET, "A connection was forcibly closed by a peer") \
    /* Process */ \
    _TBAG_XX(UNKNOWN_PROCESS_ID, "Unknown process id") \
    /* -- END -- */
#endif

/** Native type of error code. */
typedef int ErrorCodeType;

/**
 * List of error code.
 *
 * @author zer0
 * @date   2016-12-14
 */
enum class ErrorCode : ErrorCodeType
{
    _ERROR_CODE_START_NUMBER_ = -1,
#define _TBAG_XX(name, message) name,
    TBAG_ERROR_INFO_MAP(_TBAG_XX)
#undef _TBAG_XX
    ERROR_CODE_SIZE
};

TBAG_API char const * getErrorMessage(ErrorCode code);

inline char const * getErrorMessage(ErrorCodeType code)
{
    return getErrorMessage(static_cast<ErrorCode>(code));
}

} // namespace debug

typedef ::libtbag::debug::ErrorCodeType ErrType;
typedef ::libtbag::debug::ErrorCode     Err;

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_DEBUG_ERRORCODE_HPP__

