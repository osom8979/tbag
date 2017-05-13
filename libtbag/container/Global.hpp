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
#include <unordered_map>

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

private:
    Map _map;

public:
    // @formatter:off
    inline bool        empty   () const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_map.empty   ())) { return _map.empty   (); }
    inline std::size_t size    () const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_map.size    ())) { return _map.size    (); }
    inline std::size_t max_size() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_map.max_size())) { return _map.max_size(); }
    // @formatter:on

public:
    // @formatter:off
    inline void   clear() { _map.clear();        }
    inline MapItr begin() { return _map.begin(); }
    inline MapItr   end() { return _map.end();   }
    // @formatter:on

public:
    template <typename UpObject>
    std::weak_ptr<UpObject> find(Key const & key)
    {
        auto itr = _map.find(key);
        if (itr != _map.end()) {
            return std::weak_ptr<UpObject>(std::static_pointer_cast<UpObject>(itr->second));
        }
        return std::weak_ptr<UpObject>();
    }

    bool erase(Key const & key)
    {
        return _map.erase(key) == 1U;
    }

    bool insert(Key const & key, Shared h)
    {
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

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_GLOBAL_HPP__

