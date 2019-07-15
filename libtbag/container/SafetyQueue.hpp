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
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/Noncopyable.hpp>

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
template <typename ValueType
        , typename MutexType = std::mutex>
class SafetyQueue : private Noncopyable
{
public:
    using Value = ValueType;
    using Queue = std::queue<Value>;

    using Mutex = MutexType;
    using Guard = std::lock_guard<Mutex>;

private:
    mutable Mutex _mutex;
    Queue _queue;

public:
    SafetyQueue()
    { /* EMPTY. */ }
    ~SafetyQueue()
    { clear(); }

public:
    inline bool empty() const
    { Guard g(_mutex); return _queue.empty(); }
    inline std::size_t size() const
    { Guard g(_mutex); return _queue.size(); }

public:
    void clear()
    {
        Guard guard(_mutex);
        while (_queue.empty() == false) {
            _queue.pop();
        }
    }

    void push(Value const & value)
    {
        Guard guard(_mutex);
        _queue.push(value);
    }

    void push(Value && value)
    {
        Guard guard(_mutex);
        _queue.push(value);
    }

    void pop()
    {
        Guard guard(_mutex);
        if (_queue.empty() == false) {
            _queue.pop();
        }
    }

    Err popUntil(std::size_t size)
    {
        Guard guard(_mutex);
        while (_queue.size() > size) {
            if (_queue.empty()) {
                return E_QUEUE;
            }
            _queue.pop();
        }
        return E_SUCCESS;
    }

    Err front(Value & result)
    {
        Guard guard(_mutex);
        if (_queue.empty() == true) {
            return E_QUEUE;
        }
        result = _queue.front();
        return E_SUCCESS;
    }

    Err frontAndPop(Value & result)
    {
        Guard guard(_mutex);
        if (_queue.empty()) {
            return E_QUEUE;
        }
        result = _queue.front();
        _queue.pop();
        return E_SUCCESS;
    }

public:
    template <typename Predicated>
    inline void safeRun(Predicated predicated)
    {
        Guard guard(_mutex);
        predicated(_queue);
    }
};

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_SAFETYQUEUE_HPP__

