/**
 * @file   Check.hpp
 * @brief  Check class prototype.
 * @author zer0
 * @date   2017-05-02
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_CHECK_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_CHECK_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/uvpp/Handle.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

// Forward declaration.
class Loop;

/**
 * Check class prototype.
 *
 * @author zer0
 * @date   2017-05-02
 *
 * @remarks
 *  Check handles will run the given callback once per loop iteration, right after polling for i/o.
 */
class TBAG_API Check : public Handle
{
public:
    using Parent = Handle;

public:
    Check();
    Check(Loop & loop);
    virtual ~Check();

public:
    /** Initialize the handle. */
    Err init(Loop & loop);

    /** Start the handle with the given callback. */
    Err start();

    /** Stop the handle, the callback will no longer be called. */
    Err stop();

// Event methods.
public:
    virtual void onCheck();
};

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_CHECK_HPP__

