/**
 * @file   SafetyAsync.hpp
 * @brief  SafetyAsync class prototype.
 * @author zer0
 * @date   2017-05-01
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_EX_SAFETYASYNC_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_EX_SAFETYASYNC_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/uvpp/Async.hpp>
#include <libtbag/uvpp/Idle.hpp>
#include <libtbag/container/SafetyQueue.hpp>

#include <memory>
#include <functional>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

// Forward declaration.
class Loop;

namespace ex   {

/**
 * SafetyAsync class prototype.
 *
 * @author zer0
 * @date   2017-05-01
 */
class TBAG_API SafetyAsync : public BaseAsync
{
public:
    using Parent = BaseAsync;

public:
    struct Job
    {
        virtual void run(SafetyAsync * handle) = 0;
    };

public:
    struct TBAG_API FunctionalJob : public Job
    {
        using Func = std::function<void(SafetyAsync*)>;

        Func job;

        virtual void run(SafetyAsync * handle) override
        {
            if (static_cast<bool>(job)) {
                job(handle);
            }
        }
    };

public:
    struct MistakeInspector : public Idle
    {
    public:
        using Parent = Idle;

    private:
        SafetyAsync & _async;

    public:
        MistakeInspector(Loop & loop, SafetyAsync & async);
        virtual ~MistakeInspector();

    public:
        virtual void onIdle() override;
    };

public:
    using SharedJob = std::shared_ptr<Job>;
    using JobQueue  = container::SafetyQueue<SharedJob>;

private:
    MistakeInspector _inspector;
    JobQueue _jobs;

public:
    SafetyAsync(Loop & loop);
    virtual ~SafetyAsync();

public:
    inline bool empty() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_jobs.empty()))
    { return _jobs.empty(); }
    inline std::size_t size() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_jobs.size()))
    { return _jobs.size(); }

public:
    void clearJob();
    uerr pushJob(SharedJob job);
    uerr pushCloseJob();

public:
    virtual void onAsync() override;
    virtual void onClose() override;

public:
    /** Create(new) & push job. */
    template <typename JobType, typename ... Args>
    inline std::shared_ptr<typename remove_cr<JobType>::type> newPushJob(Args && ... args)
    {
        typedef typename remove_cr<JobType>::type ResultJobType;
        SharedJob shared = SharedJob(new (std::nothrow) JobType(std::forward<Args>(args) ...));
        pushJob(shared);
        return std::static_pointer_cast<ResultJobType, Job>(shared);
    }
};

} // namespace ex
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_EX_SAFETYASYNC_HPP__

