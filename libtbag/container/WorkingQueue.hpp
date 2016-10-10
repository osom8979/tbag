/**
 * @file   WorkingQueue.hpp
 * @brief  WorkingQueue class prototype.
 * @author zer0
 * @date   2016-07-26
 * @date   2016-10-11 (Remove safety ~ no locking ~)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_WORKINGQUEUE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_WORKINGQUEUE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/Exception.hpp>

#include <memory>
#include <deque>
#include <functional>
#include <algorithm>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace container {

/**
 * WorkingQueue class prototype.
 *
 * @author zer0
 * @date   2016-07-26
 */
template <typename ValueType>
class WorkingQueue : public Noncopyable
{
public:
    using Value  = ValueType;
    using Queue  = std::deque<Value>;
    using Worker = std::function<bool(Value&)>;

private:
    Queue _insert_queue;
    Queue _remove_queue;

public:
    WorkingQueue() = default;
    ~WorkingQueue() = default;

public:
    void clear()
    {
        _insert_queue.clear();
        _remove_queue.clear();
    }

    Value popFromRemoveQueue() throw (ContainerEmptyException)
    {
        if (_remove_queue.empty()) {
            throw ContainerEmptyException();
        }
        Value result = _remove_queue.front();
        _remove_queue.pop_front();
        return result;
    }

    template <typename Predicate>
    Value findAndRemoveFromRemoveQueue(Predicate predicate) throw (NotFoundException)
    {
        auto end = _remove_queue.end();
        for (auto itr = _remove_queue.begin(); itr != end; ++itr) {
            if (predicate(*itr) == true) {
                Value result = *itr;
                _remove_queue.erase(itr);
                return result;
            }
        }
        throw NotFoundException();
    }

private:
    Value popFromInsertQueue() throw (ContainerEmptyException)
    {
        if (_insert_queue.empty()) {
            throw ContainerEmptyException();
        }
        Value result = _insert_queue.front();
        _insert_queue.pop_front();
        return result;
    }

    void pushRemoveQueue(Value const & value)
    { _remove_queue.push_back(value); }

public:
    inline void push(Value const & value)
    { _insert_queue.push_back(value); }
    inline void push(Value && value)
    { _insert_queue.push_back(value); }

    bool removeFromInsertQueue(std::function<bool(Value const &)> const & predicate)
    {
        auto itr = std::find_if(_insert_queue.begin(), _insert_queue.end(), predicate);
        if (itr == _insert_queue.end()) {
            return false;
        }
        return (_insert_queue.erase(itr) != _insert_queue.end());
    }

public:
    bool work(Worker const & worker)
    {
        Value value;
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
        return this->work([](Value&) -> bool { return true; });
    }

public:
    inline std::size_t sizeOfInsertQueue() const noexcept(noexcept(_insert_queue.size()))
    { return _insert_queue.size(); }
    inline std::size_t sizeOfRemoveQueue() const noexcept(noexcept(_remove_queue.size()))
    { return _remove_queue.size(); }
};

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_WORKINGQUEUE_HPP__

