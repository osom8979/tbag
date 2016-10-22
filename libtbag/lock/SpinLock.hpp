/**
 * @file   SpinLock.hpp
 * @brief  SpinLock class prototype.
 * @author zer0
 * @date   2016-05-20
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOCK_SPINLOCK_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOCK_SPINLOCK_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <atomic>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace lock {

/**
 * SpinLock class prototype.
 *
 * @author zer0
 * @date   2016-05-20
 */
class SpinLock : public Noncopyable
{
public:
    static TBAG_CONSTEXPR bool const   LOCKED =  true;
    static TBAG_CONSTEXPR bool const UNLOCKED = false;

private:
    std::atomic_bool _state;

public:
    SpinLock() : _state(UNLOCKED)
    { /* EMPTY. */ }

public:
    void lock() TBAG_NOEXCEPT
    {
        while (this->_state.exchange(LOCKED, std::memory_order_acquire) == LOCKED) {
            // BUSY WAIT.
        }
    }

    void unlock() volatile TBAG_NOEXCEPT
    {
        this->_state.store(UNLOCKED, std::memory_order_release);
    }
};

} // namespace lock

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOCK_SPINLOCK_HPP__

