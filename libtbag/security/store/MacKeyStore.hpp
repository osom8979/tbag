/**
 * @file   MacKeyStore.hpp
 * @brief  MacKeyStore class prototype.
 * @author zer0
 * @date   2017-12-07
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_SECURITY_STORE_MACKEYSTORE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_SECURITY_STORE_MACKEYSTORE_HPP__

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

namespace security {
namespace store    {

// Forward declaration.

/**
 * MacKeyStore class prototype.
 *
 * @author zer0
 * @date   2017-12-07
 */
class TBAG_API MacKeyStore : private Noncopyable
{
private:
    // Insert member variables.

public:
    MacKeyStore();
    virtual ~MacKeyStore();
};

} // namespace store
} // namespace security

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SECURITY_STORE_MACKEYSTORE_HPP__

