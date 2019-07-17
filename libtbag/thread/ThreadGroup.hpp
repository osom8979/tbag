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
#include <libtbag/predef.hpp>
#include <libtbag/thread/Thread.hpp>
#include <libtbag/thread/FunctionalThread.hpp>

#include <unordered_set>
#include <vector>

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
class TBAG_API ThreadGroup : private Noncopyable
{
public:
    using uthread = Thread::uthread;
    using SharedThread = std::shared_ptr<Thread>;
    using ThreadSet = std::unordered_set<SharedThread>;

    using iterator = ThreadSet::iterator;
    using const_iterator = ThreadSet::const_iterator;

private:
    ThreadSet _threads;

public:
    ThreadGroup();
    ~ThreadGroup();

public:
    inline std::size_t size() const TBAG_NOEXCEPT_SP_OP(_threads.size())
    { return _threads.size(); }
    inline bool empty() const TBAG_NOEXCEPT_SP_OP(_threads.empty())
    { return _threads.empty(); }

public:
    iterator begin() TBAG_NOEXCEPT_SP_OP(_threads.begin())
    { return _threads.begin(); }
    iterator end() TBAG_NOEXCEPT_SP_OP(_threads.end())
    { return _threads.end(); }

    const_iterator begin() const TBAG_NOEXCEPT_SP_OP(_threads.begin())
    { return _threads.begin(); }
    const_iterator end() const TBAG_NOEXCEPT_SP_OP(_threads.end())
    { return _threads.end(); }

    const_iterator cbegin() const TBAG_NOEXCEPT_SP_OP(_threads.cbegin())
    { return _threads.cbegin(); }
    const_iterator cend() const TBAG_NOEXCEPT_SP_OP(_threads.cend())
    { return _threads.  cend(); }

public:
    void clear();

public:
    bool exists(uthread const & id) const;
    bool exists(Thread const & thread) const;
    bool existsCurrentThread() const;

public:
    bool insert(SharedThread const & thread);
    bool erase(uthread const & id);

public:
    uthread createThread(FunctionalThread::Callback const & cb, bool join_in_destructors = true);

public:
    void joinAll(bool rethrow = false);

public:
    std::vector<uthread> ids() const;

public:
    static bool existsThreadId(std::vector<uthread> const & ids, uthread const & id) TBAG_NOEXCEPT;
};

} // namespace thread

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_THREAD_THREADGROUP_HPP__

