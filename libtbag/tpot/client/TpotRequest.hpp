/**
 * @file   TpotRequest.hpp
 * @brief  TpotRequest class prototype.
 * @author zer0
 * @date   2017-05-27
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TPOT_CLIENT_TPOTREQUEST_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TPOT_CLIENT_TPOTREQUEST_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot   {
namespace client {

// ------------
// Entry-point.
// ------------

TBAG_API int runTpotRequestWithInteractiveMode();

} // namespace client
} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TPOT_CLIENT_TPOTREQUEST_HPP__

