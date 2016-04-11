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
#include <unordered_set>
#include <map>
#include <memory>
#include <mutex>
#include <thread>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace pattern {


/**
 * Observer interface.
 *
 * @author zer0
 * @date   2015-08-26
 */
class Observer
{
public:
    Observer() noexcept = default;
    virtual ~Observer() noexcept = default;

public:
    /**
     * Function of callback event.
     */
    virtual void notify(void * data) = 0;
};

/** Observer class to hash code. */
template <typename ObserverType = Observer>
inline std::size_t toObserverHash(ObserverType * observer)
{
    return reinterpret_cast<size_t>(observer);
}

/** Shared object for the Observer class. */
using SharedObserver = std::shared_ptr<Observer>;

/**
 * SmartObserver hash class.
 *
 * @author zer0
 * @date   2015-08-26
 * @date   2015-10-12 (Switch to the template class)
 */
template <typename ObserverType = Observer
        , typename SmartPointer = std::shared_ptr<ObserverType>
        , typename HashFunctor  = std::hash<SmartPointer>
>
struct SmartObserverHash : public HashFunctor
{
    std::size_t operator()(SmartPointer p) const noexcept
    {
        return toObserverHash<ObserverType>(p.get());
    }
};

/**
 * SmartObserver equal_to class.
 *
 * @author zer0
 * @date   2015-08-26
 * @date   2015-10-12 (Switch to the template class)
 */
template <typename ObserverType  = Observer
        , typename SmartPointer  = std::shared_ptr<ObserverType>
        , typename BinaryFunctor = std::binary_function<SmartPointer, SmartPointer, bool>
>
struct SmartObserverEqualTo: public BinaryFunctor
{
    bool operator()(SmartPointer const & x, SmartPointer const & y) const
    {
        return reinterpret_cast<std::size_t>(x.get()) == reinterpret_cast<std::size_t>(y.get());
    }
};


/**
 * Interface of Observable like classes.
 *
 * @author zer0
 * @date   2015-08-27
 * @date   2015-10-12 (Switch to the template class)
 */
template <typename ObserverType = Observer
        , typename ValueType = std::shared_ptr<ObserverType>
        , typename ParamType = std::shared_ptr<void>
>
class ObservableBase
{
public:
    using Value = ValueType;
    using Param = ParamType;

public:
    ObservableBase() noexcept = default;
    virtual ~ObservableBase() = default;

public:
    virtual void registerObserver(Value const & observer) = 0;
    virtual void unregisterObserver(Value const & observer) = 0;
    virtual void notifyObserver(Param const & data) = 0;

public:
    void notifyObserverFromNullParam()
    {
        this->notifyObserver(Param(nullptr));
    }

    void asyncNotifyObserverFromNullParam()
    {
        this->asyncNotifyObserver(Param(nullptr));
    }

    void asyncNotifyObserver(Param const & data)
    {
        using Caller = void(ObservableBase::*)(Param const &);
        std::thread thread(static_cast<Caller>(&ObservableBase::notifyObserver), this, data);
        thread.detach();
    }
};

/**
 * ObservableSet class prototype.
 *
 * @author zer0
 * @date   2015-08-26
 * @date   2015-10-12 (Switch to the template class)
 */
template <typename ObserverType = Observer
        , typename ValueType = std::shared_ptr<ObserverType>
        , typename ParamType = std::shared_ptr<void>
        , typename HashType  = SmartObserverHash<ObserverType, ValueType>
        , typename PredType  = SmartObserverEqualTo<ObserverType, ValueType>
        , typename SetType   = std::unordered_set<ValueType, HashType, PredType>
>
class ObservableSet : public ObservableBase<ObserverType, ValueType, ParamType>, public Noncopyable
{
public:
    using Mutex = std::mutex;
    using Guard = std::lock_guard<Mutex>;

private:
    Mutex _locker;
    Mutex _notify_locker;
    SetType _collection;

public:
    ObservableSet() = default;
    virtual ~ObservableSet() = default;

public:
    virtual void registerObserver(ValueType const & observer) override
    {
        Guard guard(this->_locker);
        if (_collection.size() + 1 >= _collection.max_size()) {
            return;
        }
        _collection.insert(observer);
    }

    virtual void unregisterObserver(ValueType const & observer) override
    {
        Guard guard(this->_locker);
        _collection.erase(observer);
    }

    virtual void notifyObserver(ParamType const & data) override
    {
        Guard guard(this->_notify_locker);
        //{
        _locker.lock();
        SetType clone = _collection;
        _locker.unlock();
        //}

        if (clone.size() == 0) {
            return;
        }
        for (auto cursor : clone) {
            cursor->notify(data.get());
        }
    }
};


/**
 * ObservableMap class prototype.
 *
 * @author zer0
 * @date   2015-08-27
 * @date   2015-10-12 (Switch to the template class)
 */
template <typename ObserverType = Observer
        , typename KeyType   = int
        , typename ValueType = std::shared_ptr<ObserverType>
        , typename ParamType = std::shared_ptr<void>
        , typename MapType   = std::multimap<KeyType, ValueType>
>
class ObservableMap : public ObservableBase<ObserverType, ValueType, ParamType>, public Noncopyable
{
public:
    using Mutex   = std::mutex;
    using Guard   = std::lock_guard<Mutex>;
    using MapPair = typename MapType::value_type;

public:
    static const KeyType MIN_ORDER;
    static const KeyType MAX_ORDER;
    static const KeyType DEFAULT_ORDER;

private:
    Mutex _locker;
    Mutex _notify_locker;
    MapType _collection;

public:
    ObservableMap() = default;
    virtual ~ObservableMap() = default;

public:
    void registerObserver(KeyType const & order, ValueType const & observer)
    {
        Guard guard(this->_locker);
        if (_collection.size() + 1 >= _collection.max_size()) {
            return;
        }
        _collection.insert(MapPair(order, observer));
    }

    void registerObserver(ValueType const & observer)
    {
        this->registerObserver(DEFAULT_ORDER, observer);
    }

    void unregisterObserver(ValueType const & observer)
    {
        Guard guard(this->_locker);
        if (_collection.size() == 0) {
            return;
        }

        std::size_t compare_hash = toObserverHash(observer.get());
        std::size_t current_hash = 0;

        for (auto itr = _collection.begin(); itr != _collection.end(); ++itr) {
            current_hash = toObserverHash(itr->second.get());
            if (current_hash == compare_hash) {
                _collection.erase(itr);
                break;
            }
        }
    }

    void notifyObserver(ParamType const & data)
    {
        Guard guard(this->_notify_locker);
        //{
        _locker.lock();
        MapType clone = _collection;
        _locker.unlock();
        //}

        if (clone.size() == 0) {
            return;
        }
        for (auto cursor : clone) {
            cursor.second->notify(data.get());
        }
    }
};

template <typename ObserverType, typename KeyType, typename ValueType
        , typename ParamType, typename MapType>
const KeyType ObservableMap<ObserverType, KeyType, ValueType, ParamType, MapType>::MIN_ORDER
        = std::numeric_limits<KeyType>::min();

template <typename ObserverType, typename KeyType, typename ValueType
        , typename ParamType, typename MapType>
const KeyType ObservableMap<ObserverType, KeyType, ValueType, ParamType, MapType>::MAX_ORDER
        = std::numeric_limits<KeyType>::max();

template <typename ObserverType, typename KeyType, typename ValueType
        , typename ParamType, typename MapType>
const KeyType ObservableMap<ObserverType, KeyType, ValueType, ParamType, MapType>::DEFAULT_ORDER
        = static_cast<KeyType>(100);


// ---------------------
// Default type defined.
// ---------------------

using OrderedObservable   = ObservableMap<>;
using UnorderedObservable = ObservableSet<>;

} // namespace pattern

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PATTERN_OBSERVER_HPP__

