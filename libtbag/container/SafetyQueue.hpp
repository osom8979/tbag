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
class SafetyQueue : public Noncopyable
{
public:
    using Value = ValueType;
    using Queue = std::queue<Value>;

    using Mutex = MutexType;
    using Guard = std::lock_guard<Mutex>;

    enum class Code
    {
        UNKNOWN = 0,
        SUCCESS,
        FAILURE,
        EMPTY_CONTAINER,
    };

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

    Code popUntil(std::size_t size)
    {
        Guard guard(_mutex);
        while (_queue.size() > size) {
            if (_queue.empty()) {
                return Code::EMPTY_CONTAINER;
            }
            _queue.pop();
        }
        return Code::SUCCESS;
    }

    Code front(Value & result)
    {
        Guard guard(_mutex);
        if (_queue.empty() == true) {
            return Code::EMPTY_CONTAINER;
        }
        result = _queue.front();
        return Code::SUCCESS;
    }

    Code frontAndPop(Value & result)
    {
        Guard guard(_mutex);
        if (_queue.empty()) {
            return Code::EMPTY_CONTAINER;
        }
        result = _queue.front();
        _queue.pop();
        return Code::SUCCESS;
    }
};

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_SAFETYQUEUE_HPP__

