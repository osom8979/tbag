/**
 * @file   GnomeKeyStore.hpp
 * @brief  GnomeKeyStore class prototype.
 * @author zer0
 * @date   2017-12-06
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_SECURITY_DETAILS_GNOMEKEYSTORE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_SECURITY_DETAILS_GNOMEKEYSTORE_HPP__

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
namespace details  {

// Forward declaration.

/**
 * GnomeKeyStore class prototype.
 *
 * @author zer0
 * @date   2017-12-06
 */
class TBAG_API GnomeKeyStore : private Noncopyable
{
private:
    // Insert member variables.

public:
    GnomeKeyStore();
    virtual ~GnomeKeyStore();
};

} // namespace details
} // namespace security

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SECURITY_DETAILS_GNOMEKEYSTORE_HPP__

