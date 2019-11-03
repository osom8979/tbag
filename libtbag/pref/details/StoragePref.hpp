/**
 * @file   StoragePref.hpp
 * @brief  StoragePref class prototype.
 * @author zer0
 * @date   2019-11-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PREF_DETAILS_STORAGEPREF_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PREF_DETAILS_STORAGEPREF_HPP__

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

namespace pref    {
namespace details {

// Forward declaration.

/**
 * StoragePref class prototype.
 *
 * @author zer0
 * @date   2019-11-03
 */
class TBAG_API StoragePref : private Noncopyable
{
private:
    // Insert member variables.

public:
    StoragePref();
    virtual ~StoragePref();
};

} // namespace details
} // namespace pref

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PREF_DETAILS_STORAGEPREF_HPP__

