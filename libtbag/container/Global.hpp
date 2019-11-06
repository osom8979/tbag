/**
 * @file   Global.hpp
 * @brief  Global class prototype.
 * @author zer0
 * @date   2017-02-16
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_GLOBAL_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_GLOBAL_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <libtbag/pattern/Singleton2.hpp>
#include <libtbag/container/Pointer.hpp>

#include <memory>
#include <string>
#include <unordered_map>
#include <mutex>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace container {

/**
 * Global class prototype.
 *
 * @author zer0
 * @date   2017-02-16
 */
class Global : public pattern::Singleton2<Global>
{
public:
    SINGLETON2_PROTOTYPE(Global);

public:
    using Key     = std::string;
    using Shared  = std::shared_ptr<void>;
    using Weak    = std::weak_ptr<void>;
    using Map     = std::unordered_map<Key, Shared>;
    using MapPair = Map::value_type;
    using MapItr  = Map::iterator;

public:
    using Mutex = std::mutex;
    using Guard = std::lock_guard<Mutex>;

private:
    Mutex mutable _mutex;
    Map _map;

public:
    bool empty() const
    {
        Guard const G(_mutex);
        return _map.empty();
    }

    std::size_t size() const
    {
        Guard const G(_mutex);
        return _map.size();
    }

    std::size_t max_size() const
    {
        Guard const G(_mutex);
        return _map.max_size();
    }

    void clear()
    {
        Guard const G(_mutex);
        _map.clear();
    }

public:
    template <typename UpObject>
    std::weak_ptr<UpObject> find(Key const & key)
    {
        Guard const G(_mutex);
        auto itr = _map.find(key);
        if (itr != _map.end()) {
            return std::weak_ptr<UpObject>(std::static_pointer_cast<UpObject>(itr->second));
        }
        return std::weak_ptr<UpObject>();
    }

    bool erase(Key const & key)
    {
        Guard const G(_mutex);
        return _map.erase(key) == 1U;
    }

    bool insert(Key const & key, Shared h)
    {
        Guard const G(_mutex);
        return _map.insert(MapPair(key, h)).second;
    }

public:
    /** Create(new) & insert handle. */
    template <typename UpObject, typename ... Args>
    inline std::shared_ptr<typename remove_cr<UpObject>::type> insertNewObject(Key const & key, Args && ... args)
    {
        typedef typename remove_cr<UpObject>::type ResultObjectType;
        Shared shared(new (std::nothrow) ResultObjectType(std::forward<Args>(args) ...));
        if (insert(key, shared)) {
            return std::static_pointer_cast<ResultObjectType>(shared);
        }
        return std::shared_ptr<ResultObjectType>();
    }
};

// ---------------
// Global methods.
// ---------------

template <typename UpObject, typename ... Args>
std::shared_ptr<typename remove_cr<UpObject>::type> newGlobalObject(Global::Key const & key, Args && ... args)
{
    typedef typename remove_cr<UpObject>::type ResultObjectType;
    return Global::getInstance()->insertNewObject<ResultObjectType>(key, std::forward<Args>(args) ...);
}

template <typename T>
std::weak_ptr<T> findGlobalObject(Global::Key const & key)
{
    return Global::getInstance()->find<T>(key);
}

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_GLOBAL_HPP__

