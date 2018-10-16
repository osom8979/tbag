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
#include <libtbag/Type.hpp>

#include <deque>
#include <memory>
#include <utility>
#include <type_traits>

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
            swap(obj);
        }
        return *this;
    }

public:
    void swap(ReuseQueue & obj)
    {
        _active.swap(obj._active);
        _ready.swap(obj._ready);
    }

    friend void swap(ReuseQueue & lh, ReuseQueue & rh)
    {
        lh.swap(rh);
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

namespace tr {

/**
 * ReuseQueue class prototype.
 *
 * @author zer0
 * @date   2016-08-03
 * @date   2017-11-14 (Use the std::shared_ptr)
 */
template <typename ValueType>
class ReuseQueue
{
public:
    using Value = ValueType;
    using Queue = std::deque<Value>;

    using value_type             = typename Queue::value_type;
    using allocator_type         = typename Queue::allocator_type;
    using reference              = typename Queue::reference;
    using const_reference        = typename Queue::const_reference;
    using iterator               = typename Queue::iterator;
    using const_iterator         = typename Queue::const_iterator;
    using size_type              = typename Queue::size_type;
    using difference_type        = typename Queue::difference_type;
    using pointer                = typename Queue::pointer;
    using const_pointer          = typename Queue::const_pointer;
    using reverse_iterator       = typename Queue::reverse_iterator;
    using const_reverse_iterator = typename Queue::const_reverse_iterator;

    static_assert(std::is_default_constructible<value_type>::value,
                  "The default constructor is required.");

private:
    Queue _active;
    Queue _ready;

public:
    ReuseQueue() : _active(), _ready()
    { /* EMPTY. */ }

    ReuseQueue(ReuseQueue const & obj) : ReuseQueue()
    { (*this) = obj; }

    ReuseQueue(ReuseQueue && obj) TBAG_NOEXCEPT : ReuseQueue()
    { (*this) = std::move(obj); }

    ~ReuseQueue()
    { /* EMPTY. */ }

public:
    ReuseQueue & operator =(ReuseQueue const & obj)
    {
        if (this != &obj) {
            _active = obj._active;
            _ready = obj._ready;
        }
        return *this;
    }

    ReuseQueue & operator =(ReuseQueue && obj) TBAG_NOEXCEPT
    {
        swap(obj);
        return *this;
    }

public:
    void swap(ReuseQueue & obj) TBAG_NOEXCEPT
    {
        if (this != &obj) {
            _active.swap(obj._active);
            _ready.swap(obj._ready);
        }
    }

    friend void swap(ReuseQueue & lh, ReuseQueue & rh) TBAG_NOEXCEPT
    {
        lh.swap(rh);
    }

public:
    inline std::size_t size() const TBAG_NOEXCEPT_SP_OP(_active.size())
    { return _active.size(); }

    inline bool empty() const TBAG_NOEXCEPT_SP_OP(_active.empty())
    { return _active.empty(); }

public:
    inline std::size_t sizeOfReady() const TBAG_NOEXCEPT_SP_OP(_ready.size())
    { return _ready.size(); }

    inline bool emptyOfReady() const TBAG_NOEXCEPT_SP_OP(_ready.empty())
    { return _ready.empty(); }

public:
    inline std::size_t sizeOfTotal() const
    TBAG_NOEXCEPT_SPECIFIER(TBAG_NOEXCEPT_OPERATOR(_active.size()) &&
                            TBAG_NOEXCEPT_OPERATOR(_ready.size()))
    { return _active.size() + _ready.size(); }

    inline bool emptyOfTotal() const
    TBAG_NOEXCEPT_SPECIFIER(TBAG_NOEXCEPT_OPERATOR(_active.empty()) &&
                            TBAG_NOEXCEPT_OPERATOR(_ready.empty()))
    { return _active.empty() && _ready.empty(); }

public:
    inline Value       & front()       { return _active.front(); }
    inline Value const & front() const { return _active.front(); }

    inline Value       & back()       { return _active.back(); }
    inline Value const & back() const { return _active.back(); }

public:
    // @formatter:off
          iterator begin()       TBAG_NOEXCEPT_SP_OP(_active.begin()) { return _active.begin(); }
    const_iterator begin() const TBAG_NOEXCEPT_SP_OP(_active.begin()) { return _active.begin(); }
          iterator   end()       TBAG_NOEXCEPT_SP_OP(_active.  end()) { return _active.  end(); }
    const_iterator   end() const TBAG_NOEXCEPT_SP_OP(_active.  end()) { return _active.  end(); }

          reverse_iterator rbegin()       TBAG_NOEXCEPT_SP_OP(_active.rbegin()) { return _active.rbegin(); }
    const_reverse_iterator rbegin() const TBAG_NOEXCEPT_SP_OP(_active.rbegin()) { return _active.rbegin(); }
          reverse_iterator   rend()       TBAG_NOEXCEPT_SP_OP(_active.  rend()) { return _active.  rend(); }
    const_reverse_iterator   rend() const TBAG_NOEXCEPT_SP_OP(_active.  rend()) { return _active.  rend(); }

            const_iterator  cbegin() const TBAG_NOEXCEPT_SP_OP(_active. cbegin()) { return _active. cbegin(); }
            const_iterator    cend() const TBAG_NOEXCEPT_SP_OP(_active.   cend()) { return _active.   cend(); }
    const_reverse_iterator crbegin() const TBAG_NOEXCEPT_SP_OP(_active.crbegin()) { return _active.crbegin(); }
    const_reverse_iterator   crend() const TBAG_NOEXCEPT_SP_OP(_active.  crend()) { return _active.  crend(); }
    // @formatter:on

public:
    template <typename Predicated>
    void push(Predicated predicated)
    {
        Value temp;
        if (!_ready.empty()) {
            temp = _ready.front();
            _ready.pop_front();
        }
        predicated(temp);
        _active.push_back(temp);
    }

    bool pop()
    {
        if (_active.empty()) {
            return false;
        }
        _ready.push_back(_active.front());
        _active.pop_front();
        return true;
    }

    void popAll()
    {
        while (pop()) {
            // EMPTY.
        }
    }

    void clear()
    {
        _ready.clear();
        _active.clear();
    }

    // ------------------
    // Extension methods.
    // ------------------

    bool frontAndPop(Value * result = nullptr)
    {
        if (_active.empty()) {
            return false;
        }
        if (result != nullptr) {
            *result = _active.front();
        }
        _ready.push_back(_active.front());
        _active.pop_front();
        return true;
    }

    bool frontAndPop(Value & result)
    {
        return frontAndPop(&result);
    }
};

} // namespace tr

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_REUSEQUEUE_HPP__

