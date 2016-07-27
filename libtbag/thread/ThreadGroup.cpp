/**
 * @file   ThreadGroup.cpp
 * @brief  ThreadGroup class implementation.
 * @author zer0
 * @date   2016-07-27
 */

#include <libtbag/thread/ThreadGroup.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace thread {

ThreadGroup::ThreadGroup()
{
    // EMPTY.
}

ThreadGroup::~ThreadGroup()
{
    this->clear();
}

void ThreadGroup::clear()
{
    lock::WriteLockGuard guard(_lock);
    for (List::iterator itr = _list.begin(), end = _list.end(); itr != end; ++itr) {
        delete *itr;
    }
    _list.clear();
}

bool ThreadGroup::exists(std::thread::id const & id) const
{
    lock::ReadLockGuard guard(_lock);
    for (List::const_iterator itr = _list.cbegin(), end = _list.cend(); itr != end; ++itr) {
        if ((*itr)->get_id() == id) {
            return true;
        }
    }
    return false;
}

bool ThreadGroup::exists(std::thread const * thread) const
{
    if (thread != nullptr) {
        return this->exists(thread->get_id());
    }
    return true;
}

bool ThreadGroup::existsThis() const
{
    return this->exists(std::this_thread::get_id());
}

void ThreadGroup::addThread(std::thread * thread)
{
    if (thread != nullptr && this->exists(thread) == false) {
        lock::WriteLockGuard guard(_lock);
        _list.push_back(thread);
    }
}

void ThreadGroup::removeThread(std::thread * thread)
{
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

void ThreadGroup::joinAll()
{
    lock::ReadLockGuard guard(_lock);
    for (List::iterator itr = _list.begin(), end = _list.end(); itr != end; ++itr) {
        if ((*itr)->joinable()) {
            (*itr)->join();
        }
    }
}

std::size_t ThreadGroup::size() const noexcept
{
    lock::ReadLockGuard guard(_lock);
    return _list.size();
}

} // namespace thread

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

