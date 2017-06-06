/**
 * @file   AddressToLine.hpp
 * @brief  AddressToLine class prototype.
 * @author zer0
 * @date   2017-06-06
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_DEBUG_ST_UNIX_ADDRESSTOLINE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_DEBUG_ST_UNIX_ADDRESSTOLINE_HPP__

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

namespace debug {
namespace st    {
namespace unix  {

// Forward declaration.

/**
 * AddressToLine class prototype.
 *
 * @author zer0
 * @date   2017-06-06
 */
class TBAG_API AddressToLine : private Noncopyable
{
private:
    // Insert member variables.

public:
    AddressToLine();
    virtual ~AddressToLine();
};

} // namespace unix
} // namespace st
} // namespace debug

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_DEBUG_ST_UNIX_ADDRESSTOLINE_HPP__

