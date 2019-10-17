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
    for (auto & t : _threads) {
        if (t && t->equal(tid)) {
            return true;
        }
    }
    return false;
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
    return _threads.insert(thread).second;
}

bool ThreadGroup::erase(uthread const & tid)
{
    auto itr = _threads.begin();
    auto const end = _threads.end();
    for (; itr != end; ++itr) {
        auto shared_thread = *itr;
        if (shared_thread && shared_thread->tid() == tid) {
            break;
        }
    }
    if (itr == end) {
        return false;
    }
    _threads.erase(itr);
    return true;
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

void ThreadGroup::joinAll(bool rethrow)
{
    for (auto & t : _threads) {
        if (t && t->joinable()) {
            t->join(rethrow);
        }
    }
}

Err ThreadGroup::kill(uthread const & tid)
{
    for (auto & t : _threads) {
        if (t && t->equal(tid)) {
            return t->kill();
        }
    }
    return E_NFOUND;
}

} // namespace thread

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

