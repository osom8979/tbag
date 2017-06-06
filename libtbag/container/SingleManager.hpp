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
#include <libtbag/pattern/Singleton2.hpp>

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
 */
template <typename KeyType, typename BaseType, typename MutexType = std::mutex>
class SingleManager : public libtbag::pattern::Singleton2<SingleManager<KeyType, BaseType, MutexType> >
{
public:
    using Key   = KeyType;
    using Base  = BaseType;
    using Mutex = MutexType;
    using Self  = SingleManager<Key, Base, Mutex>;

    using Guard = std::lock_guard<Mutex>;

    using SharedBase = std::shared_ptr<Base>;
    using   WeakBase =   std::weak_ptr<Base>;

    using BaseMap  = std::unordered_map<Key, SharedBase>;
    using BasePair = typename BaseMap::value_type;

private:
    mutable Mutex _mutex;
    BaseMap _map;

public:
    friend class ::libtbag::pattern::Singleton2<Self>;
protected:
    SingleManager() { /* EMPTY. */ }
public:
    virtual ~SingleManager() { /* EMPTY. */ }

public:
    inline bool empty() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_map.empty()))
    { Guard g(_mutex); return _map.empty(); }

    inline std::size_t size() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_map.size()))
    { Guard g(_mutex); return _map.size(); }

public:
    void clear()
    {
        Guard guard(_mutex);
        _map.clear();
    }

    WeakBase insert(Key id, SharedBase const & base)
    {
        Guard guard(_mutex);
        auto itr = _map.insert(BasePair(id, base));
        if (itr.second) {
            return WeakBase(itr.first->second);
        }
        return WeakBase();
    }

    bool remove(Key id)
    {
        Guard guard(_mutex);
        return _map.erase(id) == 1U;
    }

    bool exists(Key id)
    {
        Guard guard(_mutex);
        return _map.find(id) != _map.end();
    }

    WeakBase get(Key id)
    {
        Guard guard(_mutex);
        auto itr = _map.find(id);
        if (itr != _map.end()) {
            return WeakBase(itr->second);
        }
        return WeakBase();
    }

public:
    /** Create(new) & insert. */
    template <typename ... Args>
    inline WeakBase newAdd(Key id, Args && ... args)
    {
        return insert(id, SharedBase(new (std::nothrow) Base(std::forward<Args>(args) ...)));
    }

// ---------------
// Static methods.
// ---------------

public:
    static bool singleEmpty() { return Self::getInstance()->empty(); }
    static std::size_t singleSize() { return Self::getInstance()->size(); }

public:
    static void singleClear()
    {
        Self::getInstance()->clear();
    }

    static WeakBase singleInsert(Key id, SharedBase const & base)
    {
        return Self::getInstance()->insert(id, base);
    }

    static bool singleRemove(Key id)
    {
        return Self::getInstance()->remove(id);
    }

    static bool singleExists(Key id)
    {
        return Self::getInstance()->exists(id);
    }

    static WeakBase singleGet(Key id)
    {
        return Self::getInstance()->get(id);
    }

public:
    template <typename ... Args>
    static WeakBase singleNewAdd(Key id, Args && ... args)
    {
        return Self::getInstance()->template newAdd(id, std::forward<Args>(args) ...);
    }
};

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_SINGLEMANAGER_HPP__

