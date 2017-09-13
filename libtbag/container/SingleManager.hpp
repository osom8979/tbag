/**
 * @file   SingleManager.hpp
 * @brief  SingleManager class prototype.
 * @author zer0
 * @date   2017-06-06
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_SINGLEMANAGER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_SINGLEMANAGER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Type.hpp>
#include <libtbag/Noncopyable.hpp>

#include <libtbag/container/Manager.hpp>
#include <libtbag/pattern/Singleton3.hpp>

#include <unordered_map>
#include <memory>
#include <mutex>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace container {

/**
 * SingleManager class prototype.
 *
 * @author zer0
 * @date   2017-06-06
 *
 * @remarks
 *  Singletone2 does not work with MSVC. So, we use Singletone3.
 */
template <typename KeyType, typename BaseType, typename MutexType = std::mutex>
class SingleManager : public Manager<KeyType, BaseType, MutexType>
{
public:
    using Parent     = Manager<KeyType, BaseType, MutexType>;
    using Key        = typename Parent::Key;
    using Base       = typename Parent::Base;
    using Mutex      = typename Parent::Mutex;
    using Self       = typename Parent::Self;
    using Guard      = typename Parent::Guard;
    using SharedBase = typename Parent::SharedBase;
    using WeakBase   = typename Parent::WeakBase;
    using BaseMap    = typename Parent::BaseMap;
    using BasePair   = typename Parent::BasePair;

public:
    SINGLETON3_INSTANCE(Self);
protected:
    SingleManager() { /* EMPTY. */ }
public:
    virtual ~SingleManager() { /* EMPTY. */ }

public:
    static bool singleEmpty() { return getInstance()->empty(); }
    static std::size_t singleSize() { return getInstance()->size(); }

public:
    template <typename Derived>
    inline static Derived * getDerivedInstance() TBAG_NOEXCEPT
    {
        return static_cast<Derived*>(getInstance());
    }

public:
    static void singleClear()
    {
        getInstance()->clear();
    }

    static WeakBase singleInsert(Key id, SharedBase const & base)
    {
        return getInstance()->insert(id, base);
    }

    static bool singleRemove(Key id)
    {
        return getInstance()->remove(id);
    }

    static bool singleExists(Key id)
    {
        return getInstance()->exists(id);
    }

    static WeakBase singleGet(Key id)
    {
        return getInstance()->get(id);
    }

public:
    template <typename ... Args>
    static WeakBase singleNewAdd(Key id, Args && ... args)
    {
        return getInstance()->/*template*/newAdd(id, std::forward<Args>(args) ...);
    }

    template <typename Predicated>
    static void singleSafeRun(Predicated predicated)
    {
        getInstance()->/*template*/safeRun(predicated);
    }
};

template <typename K, typename B, typename M>
typename SingleManager<K, B, M>::Self * SingleManager<K, B, M>::__instance = nullptr;

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_SINGLEMANAGER_HPP__

