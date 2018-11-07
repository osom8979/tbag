/**
 * @file   UxCommon.cpp
 * @brief  UxCommon class implementation.
 * @author zer0
 * @date   2018-11-07
 */

#include <libtbag/uvxx/UxCommon.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvxx {

char const * getUxStateName(UxState state) TBAG_NOEXCEPT
{
    switch (state) {
    // @formatter:off
    case UxState::UX_NONE:     return "NONE";
    case UxState::UX_READY:    return "READY";
    case UxState::UX_RUNNING:  return "RUNNING";
    case UxState::UX_CLOSED:   return "NONE";
    default:                   return "UNKNOWN";
    // @formatter:on
    }
}

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

