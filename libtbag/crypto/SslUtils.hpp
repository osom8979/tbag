/**
 * @file   SslUtils.hpp
 * @brief  SslUtils class prototype.
 * @author zer0
 * @date   2017-12-07
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CRYPTO_SSLUTILS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CRYPTO_SSLUTILS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace crypto {

TBAG_API void initializeSsl();
TBAG_API void releaseSsl();

} // namespace crypto

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CRYPTO_SSLUTILS_HPP__

