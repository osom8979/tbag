/**
 * @file   Timer.hpp
 * @brief  Timer class prototype.
 * @author zer0
 * @date   2017-01-08
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UV_TIMER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UV_TIMER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/uv/Stream.hpp>

#include <cstdint>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uv {

// Forward declaration.
class Loop;

/**
 * Timer class prototype.
 *
 * @author zer0
 * @date   2017-01-08
 */
class TBAG_API Timer : public Stream
{
public:
    using Parent = Stream;

public:
    Timer();
    Timer(Loop & loop);
    virtual ~Timer();

public:
    /** Initialize the handle. */
    bool init(Loop & loop);

    /** Start the timer. timeout and repeat are in milliseconds. */
    bool start(uint64_t timeout, uint64_t repeat);

    /** Stop the timer, the callback will not be called anymore. */
    bool stop();

    /** Stop the timer, and if it is repeating restart it using the repeat value as the timeout. */
    bool again();

    /** Set the repeat interval value in milliseconds. */
    void setRepeat(uint64_t repeat);

    /** Get the timer repeat value. */
    uint64_t getRepeat();

public:
    virtual void onTimer();
};

} // namespace uv

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UV_TIMER_HPP__

