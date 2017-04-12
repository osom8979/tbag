/**
 * @file   Timer.hpp
 * @brief  Timer class prototype.
 * @author zer0
 * @date   2017-01-08
 * @date   2017-02-01 (Move package: libtbag/uv -> libtbag/uvpp)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_TIMER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_TIMER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/uvpp/Handle.hpp>

#include <cstdint>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

// Forward declaration.
class Loop;

/**
 * Timer class prototype.
 *
 * @author zer0
 * @date   2017-01-08
 */
class TBAG_API Timer : public Handle
{
public:
    using Parent = Handle;

public:
    Timer();
    Timer(Loop & loop);
    virtual ~Timer();

public:
    /** Initialize the handle. */
    uerr init(Loop & loop);

    /** Start the timer. timeout and repeat are in milliseconds. */
    uerr start(uint64_t timeout, uint64_t repeat = 0);

    /** Stop the timer, the callback will not be called anymore. */
    uerr stop();

    /** Stop the timer, and if it is repeating restart it using the repeat value as the timeout. */
    uerr again();

    /** Set the repeat interval value in milliseconds. */
    void setRepeat(uint64_t repeat);

    /** Get the timer repeat value. */
    uint64_t getRepeat();

public:
    virtual void onTimer();
};

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_TIMER_HPP__

