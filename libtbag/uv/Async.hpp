/**
 * @file   Async.hpp
 * @brief  Async class prototype.
 * @author zer0
 * @date   2016-12-28
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UV_ASYNC_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UV_ASYNC_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/uv/Handle.hpp>
#include <libtbag/container/SafetyQueue.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uv {

// Forward declaration.
class Loop;

/**
 * BaseAsync class prototype.
 *
 * @author zer0
 * @date   2016-12-28
 * @date   2017-01-19 (Rename: Async -> BaseAsync)
 *
 * @remarks
 *  Async handles allow the user to "wakeup" @n
 *  the event loop and get a callback called from another thread.
 *
 * @warning
 *  A uv_async_t handle is always active and cannot be deactivated, @n
 *  except by closing it with uv_close().
 */
class TBAG_API BaseAsync : public Handle
{
public:
    using Parent = Handle;

public:
    BaseAsync();

    /**
     * @warning
     *  it immediately starts the handle.
     */
    BaseAsync(Loop & loop);

public:
    virtual ~BaseAsync();

public:
    /**
     * @warning
     *  Unlike other handle initialization functions, @n
     *  it immediately starts the handle.
     */
    bool init(Loop & loop);

public:
    /**
     * Wakeup the event loop and call the async handle’s callback.
     *
     * @remarks
     *  It’s safe to call this function from any thread. @n
     *  The callback will be called on the loop thread.
     *
     * @warning
     *  libuv will coalesce calls to uv_async_send(), @n
     *  that is, not every call to it will yield an execution of the callback. @n
     *  For example: @n
     *  <pre>
     *   if uv_async_send() is called 5 times in a row before the callback is called,
     *   the callback will only be called once. If uv_async_send() is called again after
     *   the callback was called, it will be called again.
     *  </pre>
     */
    bool send();

// Event methods.
public:
    virtual void onAsync();
};

/**
 * Async class prototype.
 *
 * @author zer0
 * @date   2017-01-19
 */
class TBAG_API Async : public BaseAsync
{
public:
    using Parent = BaseAsync;

public:
    struct Job
    {
        virtual void run(Async & handle) = 0;
    };

public:
    using SharedJob = std::shared_ptr<Job>;
    using JobQueue  = container::SafetyQueue<SharedJob>;

private:
    JobQueue _jobs;
    std::atomic_bool _continuous_deququ;

public:
    Async();
    Async(Loop & loop);
    virtual ~Async();

public:
    inline void setContinuousDeququ(bool flag = true) TBAG_NOEXCEPT
    { _continuous_deququ = flag; }

    inline std::size_t size() const
    { return _jobs.size(); }

public:
    void clear();
    void push(SharedJob job);
    bool sendJob(SharedJob job);

public:
    virtual void onAsync();

public:
    /** Create(new) & push job. */
    template <typename JobType, typename ... Args>
    inline std::shared_ptr<typename remove_cr<JobType>::type> newJob(Args && ... args)
    {
        typedef typename remove_cr<JobType>::type ResultJobType;
        SharedJob shared = SharedJob(new JobType(std::forward<Args>(args) ...));
        push(shared);
        return std::static_pointer_cast<ResultJobType, Job>(shared);
    }
};

} // namespace uv

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UV_ASYNC_HPP__

