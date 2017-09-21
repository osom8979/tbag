/**
 * @file   Manager.hpp
 * @brief  Manager class prototype.
 * @author zer0
 * @date   2017-09-13
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_MANAGER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_MANAGER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <unordered_map>
#include <memory>
#include <mutex>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace container {

/**
 * Manager class prototype.
 *
 * @author zer0
 * @date   2017-09-13
 */
template <typename KeyType, typename BaseType, typename MutexType = std::mutex>
class Manager : private Noncopyable
{
public:
    using Key   = KeyType;
    using Base  = BaseType;
    using Mutex = MutexType;
    using Self  = Manager<Key, Base, Mutex>;
    using Guard = std::lock_guard<Mutex>;

    using SharedBase = std::shared_ptr<Base>;
    using   WeakBase =   std::weak_ptr<Base>;

    using BaseMap  = std::unordered_map<Key, SharedBase>;
    using BasePair = typename BaseMap::value_type;

private:
    mutable Mutex _mutex;
    BaseMap _map;

public:
    Manager() { /* EMPTY. */ }
    ~Manager() { /* EMPTY. */ }

public:
    inline bool empty() const TBAG_NOEXCEPT_SP_OP(_map.empty())
    { Guard g(_mutex); return _map.empty(); }

    inline std::size_t size() const TBAG_NOEXCEPT_SP_OP(_map.size())
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

    WeakBase get(Key id) const
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

    template <typename Predicated>
    inline void safeRun(Predicated predicated)
    {
        Guard guard(_mutex);
        predicated(_map);
    }

    template <typename Predicated>
    inline void safeRun(Predicated predicated) const
    {
        Guard guard(_mutex);
        predicated(_map);
    }
};

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_MANAGER_HPP__

