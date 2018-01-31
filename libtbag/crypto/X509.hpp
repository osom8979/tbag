/**
 * @file   X509.hpp
 * @brief  X509 class prototype.
 * @author zer0
 * @date   2018-01-31
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CRYPTO_X509_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CRYPTO_X509_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
// Include preprocessor.

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace crypto {

// Forward declaration.

/**
 * X509 class prototype.
 *
 * @author zer0
 * @date   2018-01-31
 */
class TBAG_API X509 : private Noncopyable
{
private:
    // Insert member variables.

public:
    X509();
    virtual ~X509();
};

} // namespace crypto

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CRYPTO_X509_HPP__

