/**
 * @file   Aes.hpp
 * @brief  Aes class prototype.
 * @author zer0
 * @date   2020-03-05
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CRYPTO_AES_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CRYPTO_AES_HPP__

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
 * Aes class prototype.
 *
 * @author zer0
 * @date   2020-03-05
 */
class TBAG_API Aes : private Noncopyable
{
private:
    // Insert member variables.

public:
    Aes();
    virtual ~Aes();
};

} // namespace crypto

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CRYPTO_AES_HPP__

