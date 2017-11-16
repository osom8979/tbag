/**
 * @file   ReuseMap.hpp
 * @brief  ReuseMap class prototype.
 * @author zer0
 * @date   2016-08-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_REUSEMAP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_REUSEMAP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Type.hpp>

#include <map>
#include <unordered_map>
#include <deque>
#include <memory>
#include <utility>
#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace container {

/**
 * ReuseMap class prototype.
 *
 * @author zer0
 * @date   2016-08-03
 * @date   2017-11-14 (Use the std::shared_ptr)
 *
 * @warning
 *  - Don't use the std::multimap class template.
 *  - This class is not thread-safe.
 */
template <typename KeyType
        , typename ValueType
        , typename MapType = std::map<KeyType, std::shared_ptr<ValueType> > >
class ReuseMap
{
public:
    using Key   = KeyType;
    using Value = ValueType;
    using Map   = MapType;

    using SharedValue = std::shared_ptr<Value>;
    using Queue       = std::deque<SharedValue>;

    using key_type        = typename Map::key_type;
    using mapped_type     = typename Map::mapped_type;
    using value_type      = typename Map::value_type;
    using allocator_type  = typename Map::allocator_type;
    using reference       = typename Map::reference;
    using const_reference = typename Map::const_reference;

    using pointer                = typename Map::pointer;
    using const_pointer          = typename Map::const_pointer;
    using size_type              = typename Map::size_type;
    using difference_type        = typename Map::difference_type;
    using iterator               = typename Map::iterator;
    using const_iterator         = typename Map::const_iterator;

    static_assert(is_shared_ptr<Value>::value == false,
                  "Value should not be std::shared_ptr type.");
    static_assert(std::is_same<SharedValue, mapped_type>::value,
                  "SharedValue must be the same type as mapped_type");

private:
    Map   _active;
    Queue _ready;

public:
    ReuseMap() : _active(), _ready()
    { /* EMPTY. */ }

    ReuseMap(ReuseMap const & obj) : ReuseMap()
    { (*this) = obj; }

    ReuseMap(ReuseMap && obj) : ReuseMap()
    { (*this) = std::move(obj); }

    ~ReuseMap()
    { /* EMPTY. */ }


public:
    ReuseMap & operator =(ReuseMap const & obj)
    {
        if (this != &obj) {
            _active = obj._active;
            _ready = obj._ready;
        }
        return *this;
    }

    ReuseMap & operator =(ReuseMap && obj)
    {
        swap(obj);
        return *this;
    }

public:
    void swap(ReuseMap & obj)
    {
        if (this != &obj) {
            _active.swap(obj._active);
            _ready.swap(obj._ready);
        }
    }

    friend void swap(ReuseMap & lh, ReuseMap & rh)
    {
        lh.swap(rh);
    }

public:
    inline std::size_t size() const TBAG_NOEXCEPT_SP_OP(_active.size()) { return _active.size(); }
    inline bool empty() const TBAG_NOEXCEPT_SP_OP(_active.empty()) { return _active.empty(); }

public:
    inline std::size_t sizeOfReady() const TBAG_NOEXCEPT_SP_OP(_ready.size()) { return _ready.size(); }
    inline bool emptyOfReady() const TBAG_NOEXCEPT_SP_OP(_ready.empty()) { return _ready.empty(); }

public:
    // @formatter:off
          iterator   begin()       TBAG_NOEXCEPT_SP_OP(_active. begin()) { return _active. begin(); }
    const_iterator   begin() const TBAG_NOEXCEPT_SP_OP(_active. begin()) { return _active. begin(); }
          iterator     end()       TBAG_NOEXCEPT_SP_OP(_active.   end()) { return _active.   end(); }
    const_iterator     end() const TBAG_NOEXCEPT_SP_OP(_active.   end()) { return _active.   end(); }
    const_iterator  cbegin() const TBAG_NOEXCEPT_SP_OP(_active.cbegin()) { return _active.cbegin(); }
    const_iterator    cend() const TBAG_NOEXCEPT_SP_OP(_active.  cend()) { return _active.  cend(); }
    // @formatter:on

public:
    template <typename ... Args>
    std::pair<iterator, bool> insert(Key const & key, Args && ... args)
    {
        auto itr = _active.find(key);
        if (itr != _active.end()) {
            // Found in the active map.
            return std::pair<iterator, bool>(itr, false);
        }

        SharedValue value;
        if (_ready.empty()) {
            value.reset(new Value(std::forward<Args>(args) ...));
        } else {
            value = _ready.front();
            _ready.pop_front();
            *value = Value(std::forward<Args>(args) ...);
        }
        return _active.emplace(key, value);
    }

    template <typename Predicated>
    std::pair<iterator, bool> insertWithCallback(Key const & key, Predicated predicated)
    {
        auto itr = _active.find(key);
        if (itr != _active.end()) {
            // Found in the active map.
            return std::pair<iterator, bool>(itr, false);
        }

        SharedValue value;
        if (_ready.empty()) {
            value.reset(new Value());
        } else {
            value = _ready.front();
            _ready.pop_front();
        }

        auto result = _active.emplace(key, value);
        if (result.second) {
            predicated(*value);
        }
        return result;
    }

    bool erase(Key const & key)
    {
        auto itr = _active.find(key);
        if (itr == _active.end()) {
            return false; // Not found in the active map.
        }
        _ready.push_back(itr->second);
        _active.erase(itr);
        return true;
    }

    iterator find(Key const & key)
    {
        return _active.find(key);
    }

    void clear()
    {
        while (_active.empty() == false) {
            auto itr = _active.begin();
            _ready.push_back(itr->second);
            _active.erase(itr);
        }
    }

    // ------------------
    // Extension methods.
    // ------------------

    ReuseMap clone() const
    {
        ReuseMap result;
        for (auto & a : _active) {
            result._active.emplace(a.first, SharedValue(new Value(*a.second)));
        }
        for (auto & r : _ready) {
            result._ready.emplace_back(SharedValue(new Value(*r)));
        }
        return result;
    }
};

/**
 * UnorderedReuseMap class prototype.
 *
 * @author zer0
 * @date   2017-11-15
 */
template <typename KeyType, typename ValueType>
using UnorderedReuseMap = ReuseMap<KeyType, ValueType, std::unordered_map<KeyType, std::shared_ptr<ValueType> > >;

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_REUSEMAP_HPP__

