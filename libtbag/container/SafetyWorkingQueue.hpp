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
    std::size_t  _working_count;
    bool         _cleaning;
    QueueType    _insert_queue;
    QueueType    _remove_queue;

public:
    SafetyWorkingQueue() : _working_count(0), _cleaning(false)
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
        {   // Clear insert_queue.
            LockGuard guard(_mutex);
            _cleaning = true;
            _insert_queue.clear();
        }

        // Waiting workers.
        while (true) {
            LockGuard guard(_mutex);
            if (_working_count == 0) {
                // Clear remove_queue.
                _remove_queue.clear();
                _cleaning = false;
                break;
            }
        }
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
                _remove_queue.erase(itr);
                return *itr;
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
        ++_working_count;
        return result;
    }

    void pushRemoveQueue(ValueType const & value)
    {
        LockGuard guard(_mutex);
        _remove_queue.push_back(value);
        --_working_count;
    }

public:
    bool push(ValueType const & value)
    {
        LockGuard guard(_mutex);
        if (_cleaning == true) {
            return false;
        }
        _insert_queue.push_back(value);
        return true;
    }

    bool push(ValueType && value)
    {
        LockGuard guard(_mutex);
        if (_cleaning == true) {
            return false;
        }
        _insert_queue.push_back(value);
        return true;
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

public:
    std::size_t getWorkingCount() const
    {
        LockGuard guard(_mutex);
        return _working_count;
    }

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

