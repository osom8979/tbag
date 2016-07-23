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
#include <libtbag/Noncopyable.hpp>

#include <functional>
#include <set>
#include <mutex>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace pattern {

template <typename T>
struct ObservableInterface
{
public:
    using Observer = T;

public:
    ObservableInterface() = default;
    virtual ~ObservableInterface() = default;

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
template <typename T = std::function<void(void)> >
class ObservableSet : public ObservableInterface<T>, public Noncopyable
{
public:
    using Observer = typename ObservableInterface<T>::Observer;

public:
    struct ObserverLess : std::binary_function<Observer, Observer, bool>
    {
        bool operator ()(Observer const & x, Observer const & y) const {
            return reinterpret_cast<std::size_t>(&x) < reinterpret_cast<std::size_t>(&y);
        }
    };

public:
    using Collection = std::set<Observer, ObserverLess>;

private:
    std::mutex _locker;
    Collection _collection;

public:
    ObservableSet() = default;
    ~ObservableSet() = default;

public:
    virtual bool add(Observer const & observer) override {
        std::lock_guard<std::mutex> guard(this->_locker);
        if (_collection.size() + 1 >= _collection.max_size()) {
            return false;
        }
        _collection.insert(observer);
        return true;
    }

private:
    std::mutex _notify_locker;

public:
    virtual void notify() override {
        std::lock_guard<std::mutex> guard(this->_notify_locker);
        //{
        this->_locker.lock();
        Collection clone = this->_collection;
        this->_locker.unlock();
        //}

        for (auto & cursor : clone) {
            cursor();
        }
    }
};

// --------------------
// Default type define.
// --------------------

using UnorderedObservable = ObservableSet<>;

} // namespace pattern

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PATTERN_OBSERVER_HPP__

