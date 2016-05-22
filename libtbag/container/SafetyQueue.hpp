/**
 * @file   SafetyQueue.hpp
 * @brief  SafetyQueue class prototype.
 * @author zer0
 * @date   2016-05-22
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_SAFETYQUEUE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_SAFETYQUEUE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/Exception.hpp>

#include <memory>
#include <mutex>
#include <queue>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace container {

/**
 * SafetyQueue class prototype.
 *
 * @author zer0
 * @date   2016-05-22
 */
template <typename Value
        , typename Mutex = std::mutex>
class SafetyQueue : public Noncopyable
{
public:
    using ValueType = Value;
    using MutexType = Mutex;
    using QueueType = std::queue<ValueType>;
    using LockGuard = std::lock_guard<MutexType>;

private:
    mutable MutexType _mutex;
    QueueType _queue;

public:
    SafetyQueue()  = default;
    ~SafetyQueue() = default;

// Queue operations.
public:
    void clear() {
        LockGuard guard(_mutex);
        while (_queue.empty() == false) {
            _queue.pop();
        }
    }

    void push(ValueType value) {
        LockGuard guard(_mutex);
        _queue.push(value);
    }

    void pop() {
        LockGuard guard(_mutex);
        if (_queue.empty() == false) {
            _queue.pop();
        }
    }

public:
    ValueType frontAndPop() throw(ContainerEmptyException) {
        LockGuard guard(_mutex);
        if (_queue.empty() == true) {
            throw ContainerEmptyException();
        }
        ValueType result = _queue.front();
        _queue.pop();
        return result;
    }

public:
    ValueType front() throw(ContainerEmptyException) {
        LockGuard guard(_mutex);
        if (_queue.empty() == true) {
            throw ContainerEmptyException();
        }
        return _queue.front();
    }

public:
    std::size_t size() const {
        LockGuard guard(_mutex);
        return _queue.size();
    }
};

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_SAFETYQUEUE_HPP__

