/**
 * @file   Singleton.hpp
 * @brief  Singleton class prototype.
 * @author zer0
 * @date   2016-04-04
 *
 * @remarks
 *  Singleton with double-checked locking pattern.
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PATTERN_SINGLETON_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PATTERN_SINGLETON_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/pattern/Observer.hpp>

#include <cstdlib>

#include <atomic>
#include <mutex>
#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace pattern {

#ifndef SINGLETON_WITH_DOUBLE_CHECKED_LOCKING_PATTERN
#define SINGLETON_WITH_DOUBLE_CHECKED_LOCKING_PATTERN
#define SINGLETON_WITH_DOUBLE_CHECKED_LOCKING_PATTERN_OPEN(__result_type, __atomic_instance, __mutex, __temp) \
    __result_type * __temp = __atomic_instance.load();   \
    if (__temp == nullptr) {                             \
        std::lock_guard<std::mutex> guard(__mutex);      \
        __temp = __atomic_instance.load();               \
        if (__temp == nullptr) {                         \
            __temp = new (std::nothrow) __result_type(); \
            __atomic_instance.store(__temp);
#define SINGLETON_WITH_DOUBLE_CHECKED_LOCKING_PATTERN_CLOSE(__temp) \
        }                                                \
    }                                                    \
    return __temp;
#define __SINGLETON_IMPL_OPEN(__r, __i, __m, __t) \
    SINGLETON_WITH_DOUBLE_CHECKED_LOCKING_PATTERN_OPEN(__r, __i, __m, __t)
#define __SINGLETON_IMPL_CLOSE(__t) \
    SINGLETON_WITH_DOUBLE_CHECKED_LOCKING_PATTERN_CLOSE(__t)
#endif // SINGLETON_WITH_DOUBLE_CHECKED_LOCKING_PATTERN_OPEN

/**
 * Don't use @c std::atexit function.
 *
 * @remarks
 *  Default variable is false.
 */
inline TBAG_CONSTEXPR bool isManualRelease() TBAG_NOEXCEPT
{
    return false;
}

/**
 * Singleton property template class.
 *
 * @author zer0
 * @date   2016-04-22
 */
template <typename T>
struct SingletonProperty
{
public:
    using BaseType = T;

protected:
    static std::atomic<BaseType*> _instance;
    static std::mutex _instance_lock;
};

template <typename T>
std::atomic<T*> SingletonProperty<T>::_instance;

template <typename T>
std::mutex SingletonProperty<T>::_instance_lock;

/**
 * SingletonLifeManager class prototype.
 *
 * @author zer0
 * @date   2016-04-11
 *
 * @remarks
 *  Management of Singleton classes.
 */
class SingletonLifeManager
        : public SingletonProperty<SingletonLifeManager>
        , public Noncopyable
{
public:
    using Observable = UnorderedObservable;
    using Property   = SingletonProperty<SingletonLifeManager>;

private:
    Observable _observable;

protected:
    SingletonLifeManager() {
        // EMPTY.
    }

public:
    ~SingletonLifeManager() {
        this->_observable.notify();
    }

public:
    void add(std::function<void(void)> const & observer) {
        this->_observable.add(observer);
    }

public:
    static void releaseInstance() {
        SingletonLifeManager * temp = Property::_instance.load();
        if (temp != nullptr) {
            std::lock_guard<std::mutex> guard(Property::_instance_lock);
            temp = Property::_instance.load();
            if (temp != nullptr) {
                delete temp; // Release, all singleton objects.
                Property::_instance.store(nullptr);
            }
        }
    }

public:
    static SingletonLifeManager * getInstance() {
        // @formatter:off
        __SINGLETON_IMPL_OPEN(SingletonLifeManager, Property::_instance, Property::_instance_lock, temp);
        {
            if (!isManualRelease()) {
                // Register release method.
                std::atexit(&SingletonLifeManager::releaseInstance);
            }
        }
        __SINGLETON_IMPL_CLOSE(temp);
        // @formatter:on
    }
};

/**
 * Singleton template class.
 *
 * @author zer0
 * @date   2016-04-22
 */
template <typename T>
class Singleton : public SingletonProperty<T>, public Noncopyable
{
public:
    using BaseType = T;
    using Property = SingletonProperty<BaseType>;

    static_assert(std::is_same<BaseType, typename Property::BaseType>::value
            , "Property::BaseType must be the same type as BaseType");

protected:
    Singleton() {
        // EMPTY.
    }

    virtual ~Singleton() {
        // EMPTY.
    }

private:
    static void releaseInstance() {
        BaseType * temp = Property::_instance.load();
        if (temp != nullptr) {
            std::lock_guard<std::mutex> guard(Property::_instance_lock);
            temp = Property::_instance.load();
            if (temp != nullptr) {
                delete temp;
                Property::_instance.store(nullptr);
            }
        }
    }

public:
    static BaseType * getInstance() {
        // @formatter:off
        __SINGLETON_IMPL_OPEN(BaseType, Property::_instance, Property::_instance_lock, temp);
        {
            using namespace libtbag::pattern;
            SingletonLifeManager::getInstance()->add([](){
                Singleton<BaseType>::releaseInstance();
            });
        }
        __SINGLETON_IMPL_CLOSE(temp);
        // @formatter:on
    }
};

} // namespace pattern

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#ifndef SINGLETON_INHERITANCE
#define SINGLETON_INHERITANCE(class_name) \
    public ::libtbag::pattern::Singleton<class_name>
#endif

#ifndef SINGLETON_RESTRICT
#define SINGLETON_RESTRICT(class_name)  \
    public:                             \
        friend ::libtbag::pattern::Singleton<class_name>; \
    protected:                          \
        class_name(){}                  \
    public:                             \
        virtual ~class_name(){}         \
    private: // default access modifier of class.
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PATTERN_SINGLETON_HPP__

