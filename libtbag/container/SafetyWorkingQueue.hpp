/**
 * @file   SafetyWorkingQueue.hpp
 * @brief  SafetyWorkingQueue class prototype.
 * @author zer0
 * @date   2016-07-26
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_SAFETYWORKINGQUEUE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_SAFETYWORKINGQUEUE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/Exception.hpp>

#include <memory>
#include <mutex>
#include <deque>
#include <functional>
#include <algorithm>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace container {

/**
 * SafetyWorkingQueue class prototype.
 *
 * @author zer0
 * @date   2016-07-26
 * @date   2016-10-11 (Remove @c _working_count member value)
 */
template <typename Value
        , typename Mutex = std::mutex>
class SafetyWorkingQueue : public Noncopyable
{
public:
    using ValueType = Value;
    using MutexType = Mutex;
    using QueueType = std::deque<ValueType>;
    using LockGuard = std::lock_guard<MutexType>;
    using Worker    = std::function<bool(ValueType&)>;

private:
    mutable MutexType _mutex;
    QueueType _insert_queue;
    QueueType _remove_queue;

public:
    SafetyWorkingQueue()
    {
        // EMPTY.
    }

    ~SafetyWorkingQueue()
    {
        this->clear();
    }

public:
    void clear()
    {
        LockGuard guard(_mutex);
        _insert_queue.clear();
        _remove_queue.clear();
    }

    ValueType popFromRemoveQueue() throw (ContainerEmptyException)
    {
        LockGuard guard(_mutex);
        if (_remove_queue.empty()) {
            throw ContainerEmptyException();
        }
        ValueType result = _remove_queue.front();
        _remove_queue.pop_front();
        return result;
    }

    template <typename Predicate>
    ValueType findAndRemoveFromRemoveQueue(Predicate predicate) throw (NotFoundException)
    {
        LockGuard guard(_mutex);
        auto end = _remove_queue.end();
        for (auto itr = _remove_queue.begin(); itr != end; ++itr) {
            if (predicate(*itr) == true) {
                ValueType result = *itr;
                _remove_queue.erase(itr);
                return result;
            }
        }
        throw NotFoundException();
    }

private:
    ValueType popFromInsertQueue() throw (ContainerEmptyException)
    {
        LockGuard guard(_mutex);
        if (_insert_queue.empty()) {
            throw ContainerEmptyException();
        }
        ValueType result = _insert_queue.front();
        _insert_queue.pop_front();
        return result;
    }

    void pushRemoveQueue(ValueType const & value)
    {
        LockGuard guard(_mutex);
        _remove_queue.push_back(value);
    }

public:
    bool push(ValueType const & value)
    {
        LockGuard guard(_mutex);
        _insert_queue.push_back(value);
        return true;
    }

    bool push(ValueType && value)
    {
        LockGuard guard(_mutex);
        _insert_queue.push_back(value);
        return true;
    }

    bool removeFromInsertQueue(std::function<bool(ValueType const &)> const & predicate)
    {
        LockGuard guard(_mutex);
        auto itr = std::find_if(_insert_queue.begin(), _insert_queue.end(), predicate);
        if (itr == _insert_queue.end()) {
            return false;
        }
        return (_insert_queue.erase(itr) != _insert_queue.end());
    }

public:
    bool work(Worker const & worker)
    {
        ValueType value;
        try {
            value = popFromInsertQueue();
        } catch (ContainerEmptyException & e) {
            return false;
        }

        bool move_to_remove_queue = true;
        if (static_cast<bool>(worker)) {
            move_to_remove_queue = worker(value);
        }

        if (move_to_remove_queue) {
            pushRemoveQueue(value);
        }

        return true;
    }

    inline bool work()
    {
        return this->work([](ValueType&) -> bool { return true; });
    }

public:
    std::size_t sizeOfInsertQueue() const
    {
        LockGuard guard(_mutex);
        return _insert_queue.size();
    }

    std::size_t sizeOfRemoveQueue() const
    {
        LockGuard guard(_mutex);
        return _remove_queue.size();
    }
};

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_SAFETYWORKINGQUEUE_HPP__

