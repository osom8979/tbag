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
        , std::size_t  StateCount
        , typename     Queue = std::queue<Type>
        , typename     Mutex = std::mutex >
class StateQueue
{
public:
    using BaseType   = Type;
    using QueueType  = Queue;
    using QueueArray = std::array<Queue, StateCount>;

    using MutexType  = Mutex;
    using MutexArray = std::array<Mutex, StateCount>;

public:
    static constexpr std::size_t getStateCount() noexcept {
        return StateCount;
    }

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

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_STATEQUEUE_HPP__

