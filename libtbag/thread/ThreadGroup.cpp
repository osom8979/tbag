/**
 * @file   ThreadGroup.cpp
 * @brief  ThreadGroup class implementation.
 * @author zer0
 * @date   2016-07-27
 */

#include <libtbag/thread/ThreadGroup.hpp>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace thread {

using uthread = ThreadGroup::uthread;

ThreadGroup::ThreadGroup()
{
    // EMPTY.
}

ThreadGroup::~ThreadGroup()
{
    // EMPTY.
}

void ThreadGroup::clear()
{
    _threads.clear();
}

bool ThreadGroup::exists(uthread const & tid) const
{
    return _threads.find(tid) != _threads.end();
}

bool ThreadGroup::exists(Thread const & thread) const
{
    return exists(thread.tid());
}

bool ThreadGroup::existsCurrentThread() const
{
    return exists(Thread::getCurrentThreadId());
}

bool ThreadGroup::insert(SharedThread const & thread)
{
    return _threads.emplace(thread->tid(), thread).second;
}

bool ThreadGroup::erase(uthread const & tid)
{
    return _threads.erase(tid) == 1u;
}

uthread ThreadGroup::createThread(FunctionalThread::Callback const & cb)
{
    auto thread = std::make_shared<FunctionalThread>(cb);
    assert(static_cast<bool>(thread));
    if (isSuccess(thread->run()) && insert(thread)) {
        return thread->tid();
    }
    return uthread();
}

Err ThreadGroup::join(uthread const & tid, bool rethrow)
{
    auto itr = _threads.find(tid);
    if (itr == _threads.end()) {
        return E_NFOUND;
    }
    if (!itr->second->joinable()) {
        return E_ILLSTATE;
    }
    return itr->second->join(rethrow);
}

void ThreadGroup::joinAll(bool rethrow)
{
    for (auto & t : _threads) {
        if (t.second && t.second->joinable()) {
            t.second->join(rethrow);
        }
    }
}

Err ThreadGroup::kill(uthread const & tid)
{
    auto itr = _threads.find(tid);
    if (itr == _threads.end()) {
        return E_NFOUND;
    }
    return itr->second->kill();
}

} // namespace thread

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

