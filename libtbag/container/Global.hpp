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
class TBAG_API Global : public pattern::Singleton2<Global>
{
public:
    SINGLETON2_PROTOTYPE(Global);

public:
    struct Object
    {
        virtual int type() = 0;
    };

public:
    using Key    = container::Pointer<void>;
    using Shared = std::shared_ptr<Object>;
    using Weak   = std::weak_ptr<Object>;
    using Map    = std::unordered_map<Key, Shared, typename Key::Hash, typename Key::EqualTo>;

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
    inline void clear() { _map.clear(); }
    // @formatter:on

public:
    inline Map::iterator begin() { return _map.begin(); }
    inline Map::iterator   end() { return _map.end();   }

public:
    template <typename UpObject = Object>
    std::weak_ptr<UpObject> find(Object & h)
    {
        auto itr = _map.find(Key(&h));
        if (itr != _map.end()) {
            return std::weak_ptr<UpObject>(std::static_pointer_cast<UpObject>(itr->second));
        }
        return std::weak_ptr<UpObject>();
    }

    bool erase(Object & h)
    {
        return _map.erase(Key(&h)) == 1U;
    }

    Weak insert(Shared h)
    {
        auto itr = _map.insert(Map::value_type(Key(h.get()), h));
        if (itr.second) {
            return Weak(itr.first->second);
        }
        return Weak();
    }

public:
    /** Create(new) & insert handle. */
    template <typename UpObject, typename ... Args>
    inline std::shared_ptr<typename remove_cr<UpObject>::type> newObject(Args && ... args)
    {
        typedef typename remove_cr<UpObject>::type ResultObjectType;
        Shared obj(new (std::nothrow) UpObject(std::forward<Args>(args) ...));
        return std::static_pointer_cast<ResultObjectType, Object>(insert(obj).lock());
    }
};

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_GLOBAL_HPP__

