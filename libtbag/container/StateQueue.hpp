/**
 * @file   StateQueue.hpp
 * @brief  StateQueue class prototype.
 * @author zer0
 * @date   2016-05-21
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_STATEQUEUE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_STATEQUEUE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>

#include <array>
#include <queue>
#include <mutex>
#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace container {

/**
 * StateQueue class prototype.
 *
 * @author zer0
 * @date   2016-05-21
 */
template <typename     Type
        , std::size_t  Count
        , typename     Queue = std::queue<Type>
        , typename     Mutex = std::mutex >
class StateQueue
{
public:
    static std::size_t const QUEUE_COUNT;

public:
    using BaseType   = Type;
    using QueueType  = Queue;
    using QueueArray = std::array<Queue, QUEUE_COUNT>;

    using MutexType  = Mutex;
    using MutexArray = std::array<Mutex, QUEUE_COUNT>;

private:
    QueueArray _queues;
    MutexArray _mutexs;

public:
    StateQueue() {
    }

    StateQueue(StateQueue const & obj) {
        this->copy(obj);
    }

    StateQueue(StateQueue && obj) {
        this->swap(obj);
    }

    ~StateQueue() {
    }

public:
    StateQueue & operator =(StateQueue const & obj) {
        return this->copy(obj);
    }

    StateQueue & operator =(StateQueue && obj) {
        this->swap(obj);
        return *this;
    }

public:
    StateQueue & copy(StateQueue const & obj) {
        if (this != &obj) {
            this->_queues = obj._queues;
        }
        return *this;
    }

    void swap(StateQueue & obj) {
        if (this != &obj) {
            this->_queues.swap(obj._queues);
        }
    }
};

template <typename Type, std::size_t Count, typename Queue, typename Mutex>
std::size_t const StateQueue<Type, Count, Queue, Mutex>::QUEUE_COUNT = Count;

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_STATEQUEUE_HPP__

