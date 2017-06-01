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
class TBAG_API ThreadGroup : private Noncopyable
{
public:
    using ThreadPointer = std::thread*;
    using List = std::list<ThreadPointer>;

private:
    mutable lock::RwLock _lock;
    List _list;

public:
    ThreadGroup();
    ~ThreadGroup();

public:
    void clear();

// exists thread.
public:
    bool exists(std::thread::id const & id) const;
    bool exists(std::thread const * thread) const;
    bool existsThis() const;

public:
    void addThread(std::thread * thread);
    void removeThread(std::thread * thread);

public:
    void joinAll();
    std::size_t size() const TBAG_NOEXCEPT;

public:
    template <typename ... Args>
    std::thread * createThread(Args && ... args)
    {
        std::thread * create_thread = new (std::nothrow) std::thread(std::forward<Args>(args) ...);
        addThread(create_thread);
        return create_thread;
    }
};

} // namespace thread

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_THREAD_THREADGROUP_HPP__

