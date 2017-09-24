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
    Queue _ready;

public:
    ReuseQueue() : _active(), _ready()
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
            _ready = obj._ready;
        }
        return *this;
    }

    ReuseQueue & operator =(ReuseQueue && obj)
    {
        if (this != &obj) {
            swap(*this, obj);
        }
        return *this;
    }

public:
    friend void swap(ReuseQueue & lh, ReuseQueue & rh)
    {
        lh._active.swap(rh._active);
        lh._ready.swap(rh._ready);
    }

public:
    inline std::size_t size() const TBAG_NOEXCEPT_SP_OP(_active.empty())
    { return _active.size(); }
    inline bool empty() const TBAG_NOEXCEPT_SP_OP(_active.empty())
    { return _active.empty(); }

public:
    inline std::size_t sizeOfReadyQueue() const TBAG_NOEXCEPT_SP_OP(_ready.size())
    { return _ready.size(); }
    inline bool emptyOfReadyQueue() const TBAG_NOEXCEPT_SP_OP(_ready.empty())
    { return _ready.empty(); }

public:
    void clear()
    {
        _active.clear();
        _ready.clear();
    }

public:
    inline Value       & frontRef()       TBAG_NOEXCEPT_SP_OP(_active.front()) { return _active.front(); }
    inline Value const & frontRef() const TBAG_NOEXCEPT_SP_OP(_active.front()) { return _active.front(); }

    Err front(Value & result)
    {
        if (_active.empty()) {
            return Err::E_EQUEUE;
        }
        result = _active.front();
        return Err::E_SUCCESS;
    }

public:
    template <typename ... Args>
    Value & push(Args && ... args)
    {
        if (_ready.empty()) {
            _active.emplace_back(std::forward<Args>(args) ...);
        } else {
            _active.push_back(_ready.front());
            _ready.pop_front();
        }
        return _active.back();
    }

    Err pop()
    {
        if (_active.empty()) {
            return Err::E_EQUEUE;
        }
        _ready.push_back(_active.front());
        _active.pop_front();
        return Err::E_SUCCESS;
    }

public:
    Err frontAndPop(Value & result)
    {
        if (_active.empty()) {
            return Err::E_EQUEUE;
        }
        result = _active.front();
        _ready.push_back(_active.front());
        _active.pop_front();
        return Err::E_SUCCESS;
    }
};

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_REUSEQUEUE_HPP__

