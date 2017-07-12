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
#include <libtbag/Err.hpp>

#include <deque>
#include <utility>

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
    Queue _active;
    Queue _remove;

public:
    ReuseQueue() : _active(), _remove()
    {
        // EMPTY.
    }

    ReuseQueue(ReuseQueue const & obj) : ReuseQueue()
    {
        (*this) = obj;
    }

    ReuseQueue(ReuseQueue && obj) : ReuseQueue()
    {
        (*this) = std::move(obj);
    }

    ~ReuseQueue()
    {
        // EMPTY.
    }

public:
    ReuseQueue & operator =(ReuseQueue const & obj)
    {
        if (this != &obj) {
            _active = obj._active;
            _remove = obj._remove;
        }
        return *this;
    }

    ReuseQueue & operator =(ReuseQueue && obj)
    {
        if (this != &obj) {
            _active.swap(obj._active);
            _remove.swap(obj._remove);
        }
        return *this;
    }

public:
    inline std::size_t size() const TBAG_NOEXCEPT_SPECIFIER(TBAG_NOEXCEPT_OPERATOR(_active.empty()))
    { return _active.size(); }
    inline bool empty() const TBAG_NOEXCEPT_SPECIFIER(TBAG_NOEXCEPT_OPERATOR(_active.empty()))
    { return _active.empty(); }

public:
    inline std::size_t sizeOfRemoveQueue() const TBAG_NOEXCEPT_SPECIFIER(TBAG_NOEXCEPT_OPERATOR(_remove.size()))
    { return _remove.size(); }
    inline bool emptyOfRemoveQueue() const TBAG_NOEXCEPT_SPECIFIER(TBAG_NOEXCEPT_OPERATOR(_remove.empty()))
    { return _remove.empty(); }

public:
    void clear()
    {
        _active.clear();
        _remove.clear();
    }

public:
    Value & push()
    {
        if (_remove.empty()) {
            _active.push_back(Value());
        } else {
            _active.push_back(_remove.front());
            _remove.pop_front();
        }
        return _active.back();
    }

    Err pop()
    {
        if (_active.empty()) {
            return Err::E_EQUEUE;
        }
        _remove.push_back(_active.front());
        _active.pop_front();
        return Err::E_SUCCESS;
    }

    Err front(Value & result)
    {
        if (_active.empty()) {
            return Err::E_EQUEUE;
        }
        result = _active.front();
        return Err::E_SUCCESS;
    }
};

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_REUSEQUEUE_HPP__

