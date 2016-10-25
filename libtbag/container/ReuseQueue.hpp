/**
 * @file   ReuseQueue.hpp
 * @brief  ReuseQueue class prototype.
 * @author zer0
 * @date   2016-08-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_REUSEQUEUE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_REUSEQUEUE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <deque>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace container {

/**
 * ReuseQueue class prototype.
 *
 * @author zer0
 * @date   2016-08-03
 */
template <typename ValueType>
class ReuseQueue
{
public:
    using Value = ValueType;
    using Queue = std::deque<Value>;

private:
    Queue _active_queue;
    Queue _remove_queue;

public:
    ReuseQueue() = default;
    ~ReuseQueue() = default;

    ReuseQueue(ReuseQueue const & obj) = default;
    ReuseQueue & operator =(ReuseQueue const & obj) = default;

#if defined(TBAG_HAS_DEFAULTED_FUNCTIONS) && !defined(TBAG_HAS_DEFAULTED_FUNCTIONS_BUT_NOT_MOVE_FUNCTION)
    ReuseQueue(ReuseQueue && obj) = default;
    ReuseQueue & operator =(ReuseQueue && obj) = default;
#endif

public:
    void clear()
    {
        _active_queue.clear();
        _remove_queue.clear();
    }

public:
    Value * push()
    {
        if (_remove_queue.empty()) {
            _active_queue.push_back(Value());
        } else {
            _active_queue.push_back(_remove_queue.front());
            _remove_queue.pop_front();
        }
        return &_active_queue.back();
    }

    void pop()
    {
        if (_active_queue.empty()) {
            return;
        }

        _remove_queue.push_back(_active_queue.front());
        _active_queue.pop_front();
    }

    Value * front()
    {
        if (_active_queue.empty()) {
            return nullptr;
        }
        return &_active_queue.front();
    }

public:
    inline std::size_t size() const TBAG_NOEXCEPT
    { return _active_queue.size(); }
    inline bool empty() const TBAG_NOEXCEPT
    { return _active_queue.empty(); }

    inline std::size_t sizeOfRemoveQueue() const TBAG_NOEXCEPT
    { return _remove_queue.size(); }
    inline bool emptyOfRemoveQueue() const TBAG_NOEXCEPT
    { return _remove_queue.empty(); }
};

template <typename Key>
using ReusePtrQueue = ReuseQueue<std::shared_ptr<Key> >;

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_REUSEQUEUE_HPP__

