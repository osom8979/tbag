/**
 * @file   DependencyWorker.hpp
 * @brief  DependencyWorker class prototype.
 * @author zer0
 * @date   2016-08-31
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_DEPENDENCYWORKER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_DEPENDENCYWORKER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <functional>
#include <vector>
#include <set>
#include <unordered_map>
#include <algorithm>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace container {

/**
 * DependencyWorker class prototype.
 *
 * @author zer0
 * @date   2016-08-31
 */
template <typename KeyType>
class DependencyWorker
{
public:
    static constexpr std::size_t DEFAULT_MAX_RECURSIVE = 10;

public:
    using Key          = KeyType;
    using Worker       = std::function<void(void)>;
    using Dependencies = std::vector<Key>;

public:
    struct WorkingProperty
    {
        Dependencies dependencies;
        Worker worker;
    };

public:
    using WorkingMap  = std::unordered_map<Key, WorkingProperty>;
    using WorkingPair = typename WorkingMap::value_type;

private:
    std::size_t _max_recursive = DEFAULT_MAX_RECURSIVE;
    WorkingMap  _workers;

public:
    DependencyWorker() = default;
    ~DependencyWorker() = default;

public:
    DependencyWorker(std::size_t max_recursive) : _max_recursive(max_recursive)
    {
        // EMPTY.
    }

public:
    DependencyWorker(DependencyWorker const & obj) = default;
    DependencyWorker(DependencyWorker && obj) = default;

public:
    DependencyWorker & operator =(DependencyWorker const & obj) = default;
    DependencyWorker & operator =(DependencyWorker && obj) = default;

public:
    inline std::size_t getMaxRecursive() const noexcept
    { return _max_recursive; }
    inline void setMaxRecursive(std::size_t count) noexcept
    { _max_recursive = count; }
    inline bool empty() const noexcept
    { return _workers.empty(); }
    inline std::size_t size() const noexcept
    { return _workers.size(); }

public:
    void clear()
    {
        _workers.clear();
    }

    bool insert(Key key, WorkingProperty const & worker)
    {
        return _workers.insert(std::make_pair(key, worker)).second;
    }

    bool insert(Key key, WorkingProperty && worker)
    {
        return _workers.insert(std::make_pair(key, worker)).second;
    }

    bool insert(Key key, Dependencies const & dependencies, Worker const & worker)
    {
        return insert(key, WorkingProperty{dependencies, worker});
    }

    bool insert(Key key, Dependencies && dependencies, Worker && worker)
    {
        return insert(key, WorkingProperty{dependencies, worker});
    }

private:
    bool run(Key const & key, Dependencies & completed_keys, std::size_t recursive_count)
    {
        // Endless loop guard.
        if (recursive_count == 0) {
            return false;
        }
        --recursive_count;

        // Exists worker.
        auto find_itr = _workers.find(key);
        if (find_itr == _workers.end()) {
            return false;
        }

        // Children dependencies.
        for (auto & dependency : find_itr->second.dependencies) {
            if (run(dependency, completed_keys, recursive_count) == false) {
                return false;
            }
        }

        // Exists key.
        bool not_found_key = std::find(completed_keys.begin(), completed_keys.end(), key) == completed_keys.end();
        if (static_cast<bool>(find_itr->second.worker) && not_found_key) {
            // WORKING!
            find_itr->second.worker();
            completed_keys.push_back(key);
        }
        return true;
    }

public:
    bool run(Key const & key)
    {
        Dependencies completed_keys;
        return run(key, completed_keys, _max_recursive);
    }
};

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_DEPENDENCYWORKER_HPP__

