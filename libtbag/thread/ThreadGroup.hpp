/**
 * @file   ThreadGroup.hpp
 * @brief  ThreadGroup class prototype.
 * @author zer0
 * @date   2016-05-12
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_THREAD_THREADGROUP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_THREAD_THREADGROUP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/lock/RwLock.hpp>

#include <thread>
#include <list>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace thread {

/**
 * ThreadGroup class prototype.
 *
 * @author zer0
 * @date   2016-05-12
 */
class ThreadGroup : public Noncopyable
{
public:
    using ThreadPointer = std::thread*;
    using List = std::list<ThreadPointer>;

private:
    mutable lock::RwLock _lock;
    List _list;

public:
    ThreadGroup() {
        __EMPTY_BLOCK__
    }

    ~ThreadGroup() {
        clear();
    }

public:
    void clear() {
        lock::WriteLockGuard guard(_lock);
        for (List::iterator itr = _list.begin(), end = _list.end(); itr != end; ++itr) {
            delete *itr;
        }
        _list.clear();
    }

// exists thread.
public:
    bool exists(std::thread::id const & id) const {
        lock::ReadLockGuard guard(_lock);
        for (List::const_iterator itr = _list.cbegin(), end = _list.cend(); itr != end; ++itr) {
            if ((*itr)->get_id() == id) {
                return true;
            }
        }
        return false;
    }

    bool exists(std::thread const * thread) const {
        if (thread != nullptr) {
            return exists(thread->get_id());
        }
        return true;
    }

    bool existsThis() const {
        return exists(std::this_thread::get_id());
    }

public:
    template <typename Function>
    std::thread * createThread(Function callback) {
        std::thread * create_thread = new (std::nothrow) std::thread(callback);
        addThread(create_thread);
        return create_thread;
    }

    void addThread(std::thread * thread) {
        if (thread != nullptr && exists(thread) == false) {
            lock::WriteLockGuard guard(_lock);
            _list.push_back(thread);
        }
    }

    void removeThread(std::thread * thread) {
        lock::WriteLockGuard guard(_lock);

        // [WARNING]
        // Don't use std::find.
        // Reason: No matching function for call to std::find (in G++).
        //         'std::find(List::iterator, List::iterator, std::thread * &)'
        // List::iterator itr = std::find(_list.begin(), _list.end(), thread);

        for (List::iterator itr = _list.begin(), end = _list.end(); itr != end; ++itr) {
            if ((*itr) == thread) {
                _list.erase(itr);
                break;
            }
        }
    }

public:
    void joinAll() {
        lock::ReadLockGuard guard(_lock);
        for (List::iterator itr = _list.begin(), end = _list.end(); itr != end; ++itr) {
            if ((*itr)->joinable()) {
                (*itr)->join();
            }
        }
    }

    std::size_t size() const noexcept {
        lock::ReadLockGuard guard(_lock);
        return _list.size();
    }
};

} // namespace thread

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_THREAD_THREADGROUP_HPP__

