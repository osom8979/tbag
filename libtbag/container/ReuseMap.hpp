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

#include <map>
#include <queue>
#include <mutex>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace container {

/**
 * ReuseMap class prototype.
 *
 * @author zer0
 * @date   2016-08-03
 *
 * @warning
 *  - Don't use the std::multimap class template.
 *  - This class is not thread-safe.
 */
template <typename KeyType
        , typename ValueType
        , typename MapType = std::map<KeyType, ValueType> >
class ReuseMap
{
public:
    using Key     = KeyType;
    using Value   = ValueType;
    using Map     = MapType;

    using MapItr  = typename MapType::iterator;
    using MapPair = typename Map::value_type;
    using Queue   = std::queue<Value>;

private:
    Map   _active_map;
    Queue _remove_queue;

public:
    ReuseMap() = default;
    ReuseMap(ReuseMap const & obj) = default;
    ReuseMap(ReuseMap && obj) = default;
    ~ReuseMap() = default;

public:
    ReuseMap & operator =(ReuseMap const & obj) = default;
    ReuseMap & operator =(ReuseMap && obj) = default;

public:
    void clear()
    {
        _active_map.clear();
        if (_remove_queue.empty() == false) {
            _remove_queue.pop();
        }
    }

public:
    Value * create(KeyType const & key)
    {
        if (_active_map.find(key) != _active_map.end()) {
            // Found in the active map.
            return nullptr;
        }

        std::pair<MapItr, bool> itr;

        if (_remove_queue.empty()) {
            // New memory.
            itr = _active_map.insert(MapPair(key, Value()));
        } else {
            itr = _active_map.insert(MapPair(key, _remove_queue.front()));
            _remove_queue.pop();
        }

        if (itr.second) {
            return &(itr.first->second);
        }
        return nullptr;
    }

    bool erase(KeyType const & key)
    {
        auto active_itr = _active_map.find(key);
        if (active_itr == _active_map.end()) {
            // Not found in the active map.
            return false;
        }

        _remove_queue.push(active_itr->second);
        _active_map.erase(active_itr);
        return true;
    }

    Value * find(KeyType const & key)
    {
        auto active_itr = _active_map.find(key);
        if (active_itr == _active_map.end()) {
            // Not found in the active map.
            return nullptr;
        }

        return &(active_itr->second);
    }

public:
    inline std::size_t size() const noexcept
    { return _active_map.size(); }
    inline bool empty() const noexcept
    { return _active_map.empty(); }

    inline std::size_t sizeOfRemoveQueue() const noexcept
    { return _remove_queue.size(); }
    inline bool emptyOfRemoveQueue() const noexcept
    { return _remove_queue.empty(); }
};

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_REUSEMAP_HPP__

