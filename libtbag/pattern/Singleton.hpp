/**
 * @file   Singleton.hpp
 * @brief  Singleton class prototype.
 * @author zer0
 * @date   2016-04-04
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PATTERN_SINGLETON_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PATTERN_SINGLETON_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/pattern/Observer.hpp>

#include <cstdlib>
#include <atomic>
#include <mutex>

/**
 * Don't use @c std::atexit function.
 *
 * @remarks
 *  Default variable is true.
 */
constexpr bool isManualRelease() noexcept
{
    return false;
}

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace pattern {

/**
 * SingletonLifeManager class prototype.
 *
 * @author zer0
 * @date   2016-04-11
 *
 * @remarks
 *  Management of Singleton classes.
 */
class SingletonLifeManager : public Noncopyable
{
public:
    UnorderedObservable _observable;

protected:
    SingletonLifeManager() {
        __EMPTY_BLOCK__
    }

public:
    ~SingletonLifeManager() {
        this->_observable.notify();
    }

public:
    void add(std::function<void(void)> const & observer) {
        this->_observable.add(observer);
    }

// Singleton section.
private:
    static std::atomic<SingletonLifeManager*> __instance;
    static std::mutex __instance_lock;

public:
    static void releaseInstance() {
        SingletonLifeManager * temp = __instance.load();
        if (temp != nullptr) {
            std::lock_guard<std::mutex> guard(__instance_lock);
            temp = __instance.load();
            if (temp != nullptr) {
                delete temp; // Release, all singleton objects.
                __instance.store(nullptr);
            }
        }
    }

public:
    static SingletonLifeManager * getInstance() {
        SingletonLifeManager * temp = __instance.load();
        if (temp == nullptr) {
            std::lock_guard<std::mutex> guard(__instance_lock);
            temp = __instance.load();
            if (temp == nullptr) {
                temp = new SingletonLifeManager();
                __instance.store(temp);

                if (!isManualRelease()) {
                    // Register release method.
                    std::atexit(&SingletonLifeManager::releaseInstance);
                }
            }
        }
        return temp;
    }
};

std::atomic<SingletonLifeManager*> SingletonLifeManager::__instance(nullptr);
std::mutex SingletonLifeManager::__instance_lock;

} // namespace pattern

#ifndef SINGLETON_PROTOTYPE
#define SINGLETON_PROTOTYPE(base_class)                         \
    protected:                                                  \
        base_class();                                           \
    public:                                                     \
        ~base_class();                                          \
    private:                                                    \
        static std::atomic<base_class*> __instance;             \
        static std::mutex __instance_lock;                      \
    private:                                                    \
        static void releaseInstance();                          \
    public:                                                     \
        static base_class * getInstance();                      \
    private: // default access modifier of class.
#endif

#ifndef SINGLETON_IMPLEMENT
#define SINGLETON_IMPLEMENT(base_class)                             \
    std::atomic<base_class*> base_class::__instance(nullptr);       \
    std::mutex base_class::__instance_lock;                         \
                                                                    \
    void base_class::releaseInstance() {                            \
        base_class * temp = __instance.load();                      \
        if (temp != nullptr) {                                      \
            std::lock_guard<std::mutex> guard(__instance_lock);     \
            temp = __instance.load();                               \
            if (temp != nullptr) {                                  \
                delete temp;                                        \
                __instance.store(nullptr);                          \
            }                                                       \
        }                                                           \
    }                                                               \
                                                                    \
    base_class * base_class::getInstance()                          \
    {                                                               \
        base_class * temp = __instance.load();                      \
        if (temp == nullptr) {                                      \
            std::lock_guard<std::mutex> guard(__instance_lock);     \
            temp = __instance.load();                               \
            if (temp == nullptr) {                                  \
                temp = new base_class();                            \
                __instance.store(temp);                             \
                using namespace libtbag;                            \
                using namespace libtbag::pattern;                   \
                SingletonLifeManager::getInstance()->add([](){      \
                        base_class::releaseInstance();              \
                    });                                             \
            }                                                       \
        }                                                           \
        return temp;                                                \
    }
#endif

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PATTERN_SINGLETON_HPP__

