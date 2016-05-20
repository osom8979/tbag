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
    static constexpr bool const   LOCKED =  true;
    static constexpr bool const UNLOCKED = false;

#if defined(TEST_LIBTBAG_SPINLOCK)
public:
#else
private:
#endif
    std::atomic_bool _state;

public:
    SpinLock() : _state(UNLOCKED) {
        __EMPTY_BLOCK__
    }

public:
    void lock() noexcept {
        while (this->_state.exchange(LOCKED, std::memory_order_acquire) == LOCKED) {
            // BUSY WAIT.
        }
    }

    bool try_lock() noexcept {
        return !(this->_state.exchange(LOCKED, std::memory_order_acquire) == LOCKED);
    }

    void unlock() volatile noexcept {
        this->_state.store(UNLOCKED, std::memory_order_release);
    }
};

} // namespace lock

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOCK_SPINLOCK_HPP__

