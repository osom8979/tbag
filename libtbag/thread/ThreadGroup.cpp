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

using WeakThread = ThreadGroup::WeakThread;
using uthread = ThreadGroup::uthread;

ThreadGroup::ThreadGroup()
{
    // EMPTY.
}

ThreadGroup::ThreadGroup(ThreadGroup && obj) TBAG_NOEXCEPT : _threads(std::move(obj._threads))
{
    // EMPTY.
}

ThreadGroup::~ThreadGroup()
{
    // EMPTY.
}

ThreadGroup & ThreadGroup::operator =(ThreadGroup && obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _threads = std::move(obj._threads);
    }
    return *this;
}

void ThreadGroup::swap(ThreadGroup & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _threads.swap(obj._threads);
    }
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

WeakThread ThreadGroup::get(uthread const & tid) const
{
    auto itr = _threads.find(tid);
    if (itr == _threads.end()) {
        return {};
    }
    return itr->second;
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

Err ThreadGroup::kill(uthread const & tid, int signum)
{
    auto itr = _threads.find(tid);
    if (itr == _threads.end()) {
        return E_NFOUND;
    }
    return itr->second->kill(signum);
}

Err ThreadGroup::cancel(uthread const & tid)
{
    auto itr = _threads.find(tid);
    if (itr == _threads.end()) {
        return E_NFOUND;
    }
    return itr->second->cancel();
}

} // namespace thread

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

