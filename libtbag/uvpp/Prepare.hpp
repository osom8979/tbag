/**
 * @file   Prepare.hpp
 * @brief  Prepare class prototype.
 * @author zer0
 * @date   2017-05-02
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_PREPARE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_PREPARE_HPP__

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
 * Prepare class prototype.
 *
 * @author zer0
 * @date   2017-05-02
 *
 * @remarks
 *  Prepare handles will run the given callback once per loop iteration, right before polling for i/o.
 */
class TBAG_API Prepare : public Handle
{
public:
    friend class Loop;

public:
    using Parent = Handle;

protected:
    Prepare(Loop & loop);

public:
    virtual ~Prepare();

public:
    /** Initialize the handle. */
    Err init(Loop & loop);

    /** Start the handle with the given callback. */
    Err start();

    /** Stop the handle, the callback will no longer be called. */
    Err stop();

// Event methods.
public:
    virtual void onPrepare();
};

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_PREPARE_HPP__

