/**
 * @file   ErrorCode.cpp
 * @brief  ErrorCode class implementation.
 * @author zer0
 * @date   2016-12-14
 */

#include <libtbag/debug/ErrorCode.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace debug {

char const * getErrorMessage(ErrorCode code)
{
    switch (code) {
#define _TBAG_XX(name, message) case ErrorCode::name: return #message;
    TBAG_ERROR_INFO_MAP(_TBAG_XX)
#undef _TBAG_XX
    default: return "Unknown error code.";
    }
}

} // namespace debug

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

