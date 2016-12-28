/**
 * @file   Idle.hpp
 * @brief  Idle class prototype.
 * @author zer0
 * @date   2016-12-28
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UV_IDLE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UV_IDLE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/uv/Handle.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uv {

// Forward declaration.
class Loop;

/**
 * Idle class prototype.
 *
 * @author zer0
 * @date   2016-12-28
 *
 * @remarks
 *  Idle handles will run the given callback once per loop iteration, @n
 *  right before the uv_prepare_t handles. @n
 *  @n
 *  Note: @n
 *  <pre>
 *   The notable difference with prepare handles is that when there are active idle handles,
 *   the loop will perform a zero timeout poll instead of blocking for i/o.
 *  </pre>
 *
 * @warning
 *  Despite the name, idle handles will get their callbacks
 *  called on every loop iteration, not when the loop is actually "idle".
 */
class TBAG_API Idle : public Handle
{
public:
    using Parent = Handle;

public:
    Idle(Loop & loop);
    virtual ~Idle();

public:
    inline bool isStarted() const TBAG_NOEXCEPT
    { return isActive(); }

public:
    /** Start the handle with the given callback. */
    bool start();

    /** Stop the handle, the callback will no longer be called. */
    bool stop();

// Event methods.
public:
    virtual void onIdle();
};

} // namespace uv

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UV_IDLE_HPP__

