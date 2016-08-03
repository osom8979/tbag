/**
 * @file   SafetyReuseMap.hpp
 * @brief  SafetyReuseMap class prototype.
 * @author zer0
 * @date   2016-08-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_SAFETYREUSEMAP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_SAFETYREUSEMAP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/Noncopyable.hpp>

#include <map>
#include <queue>
#include <mutex>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace container {

/**
 * SafetyReuseMap class prototype.
 *
 * @author zer0
 * @date   2016-08-03
 *
 * @warning
 *  Don't use the std::multimap class template.
 */
template <typename KeyType
        , typename ValueType
        , typename MapType   = std::map<KeyType, ValueType>
        , typename MutexType = std::mutex>
class SafetyReuseMap : public Noncopyable
{
public:
    using Key     = KeyType;
    using Value   = ValueType;
    using Map     = MapType;
    using Mutex   = MutexType;

    using MapItr  = typename MapType::iterator;
    using MapPair = typename Map::value_type;
    using Queue   = std::queue<Value>;
    using Guard   = std::lock_guard<Mutex>;

private:
    mutable Mutex _mutex;
    Map   _active_map;
    Queue _remove_queue;

public:
    SafetyReuseMap() = default;
    SafetyReuseMap(SafetyReuseMap const & obj) = default;
    SafetyReuseMap(SafetyReuseMap && obj) = default;
    ~SafetyReuseMap() = default;

public:
    SafetyReuseMap & operator =(SafetyReuseMap const & obj) = default;
    SafetyReuseMap & operator =(SafetyReuseMap && obj) = default;

public:
    void clear()
    {
        Guard guard(_mutex);
        _active_map.clear();
        if (_remove_queue.empty() == false) {
            _remove_queue.pop();
        }
    }

public:
    Value * create(KeyType const & key)
    {
        Guard guard(_mutex);
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
        Guard guard(_mutex);
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
        Guard guard(_mutex);
        auto active_itr = _active_map.find(key);
        if (active_itr == _active_map.end()) {
            // Not found in the active map.
            return nullptr;
        }

        return &(active_itr->second);
    }

public:
    inline std::size_t size() const noexcept
    {
        Guard guard(_mutex);
        return _active_map.size();
    }

    inline bool empty() const noexcept
    {
        Guard guard(_mutex);
        return _active_map.empty();
    }

    inline std::size_t sizeOfRemoveQueue() const noexcept
    {
        Guard guard(_mutex);
        return _remove_queue.size();
    }

    inline bool emptyOfRemoveQueue() const noexcept
    {
        Guard guard(_mutex);
        return _remove_queue.empty();
    }
};

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_SAFETYREUSEMAP_HPP__

