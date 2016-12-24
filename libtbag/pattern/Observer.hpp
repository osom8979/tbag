/**
 * @file   Observer.hpp
 * @brief  Observer class prototype.
 * @author zer0
 * @date   2016-04-11
 *
 * @see <https://en.wikipedia.org/wiki/Observer_pattern>
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PATTERN_OBSERVER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PATTERN_OBSERVER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <set>
#include <mutex>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace pattern {

template <typename Functor>
struct ObservableInterface
{
public:
    using Observer = Functor;

public:
    ObservableInterface() { /* EMPTY. */ }
    ~ObservableInterface() { /* EMPTY. */ }

public:
    virtual bool add(Observer const & observer) = 0;
    virtual void notify() = 0;
};

/**
 * ObservableSet class prototype.
 *
 * @author zer0
 * @date   2015-08-26
 * @date   2015-10-12 (Switch to the template class)
 * @date   2016-04-11 (Remove std::shared_ptr template class)
 */
template <typename Functor>
class ObservableSet : public ObservableInterface<Functor>, public Noncopyable
{
public:
    using Observer = typename ObservableInterface<Functor>::Observer;

public:
    struct ObserverLess : public std::binary_function<Observer, Observer, bool>
    {
        bool operator ()(Observer const & x, Observer const & y) const
        {
            return reinterpret_cast<std::size_t>(&x) < reinterpret_cast<std::size_t>(&y);
        }
    };

public:
    using Collection = std::set<Observer, ObserverLess>;
    using Mutex = std::mutex;
    using Guard = std::lock_guard<Mutex>;

protected:
    mutable Mutex _mutex;

protected:
    Collection _collection;

public:
    ObservableSet() { /* EMPTY. */ }
    ~ObservableSet() { /* EMPTY. */ }

public:
    inline Collection getCollection() const
    {
        Guard guard(_mutex);
        return _collection;
    }

public:
    virtual bool add(Observer const & observer) override
    {
        Guard guard(_mutex);
        if (_collection.size() + 1 >= _collection.max_size()) {
            return false;
        }

        _collection.insert(observer);
        return true;
    }

protected:
    Mutex _notify;

public:
    virtual void notify() override
    {
        Guard guard(_notify);
        for (auto & cursor : getCollection()) {
            cursor();
        }
    }
};

} // namespace pattern

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PATTERN_OBSERVER_HPP__

