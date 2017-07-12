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

namespace ex {

/**
 * SafetyAsync class prototype.
 *
 * @author zer0
 * @date   2017-05-01
 *
 * @remarks
 *  An Async handle that guarantees a call.
 */
class TBAG_API SafetyAsync : public Async
{
public:
    friend class uvpp::Loop;

public:
    using Parent = Async;

public:
    /**
     * Job runner.
     */
    struct JobInterface
    {
        JobInterface() { /* EMPTY. */ }
        virtual ~JobInterface() { /* EMPTY. */ }

        virtual void run() = 0;
    };

public:
    /**
     * Functional job callback.
     */
    struct FunctionalJob : public JobInterface
    {
        using OnJob = std::function<void(void)>;

        OnJob job_cb;

        FunctionalJob(OnJob const & cb) : job_cb(cb) { /* EMPTY. */ }
        virtual ~FunctionalJob() { /* EMPTY. */ }

        virtual void run() override
        {
            if (static_cast<bool>(job_cb)) {
                job_cb();
            }
        }
    };

public:
    /**
     * Check the missing jobs.
     */
    struct TBAG_API MistakeInspector : public uvpp::Idle
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
        virtual void onClose() override;
    };

public:
    using SharedInspector = std::shared_ptr<MistakeInspector>;

    using SharedJob = std::shared_ptr<JobInterface>;
    using JobQueue  = container::SafetyQueue<SharedJob>;

private:
    SharedInspector _inspector;
    JobQueue _jobs;

protected:
    SafetyAsync(Loop & loop);

public:
    virtual ~SafetyAsync();

private:
    void closeInspector();
    Err startInspector();
    Err stopInspector();

public:
    inline bool empty() const TBAG_NOEXCEPT_SPECIFIER(TBAG_NOEXCEPT_OPERATOR(_jobs.empty()))
    { return _jobs.empty(); }
    inline std::size_t size() const TBAG_NOEXCEPT_SPECIFIER(TBAG_NOEXCEPT_OPERATOR(_jobs.size()))
    { return _jobs.size(); }

public:
    void clearJob();
    Err sendJob(SharedJob job);
    Err sendClose();

public:
    virtual void onAsync() override;
    virtual void onClose() override;

public:
    /** Create(new) & push & send job. */
    template <typename JobType, typename ... Args>
    inline std::shared_ptr<typename remove_cr<JobType>::type> newSendJob(Args && ... args)
    {
        typedef typename remove_cr<JobType>::type ResultJobType;
        SharedJob shared = SharedJob(new (std::nothrow) JobType(std::forward<Args>(args) ...));
        sendJob(shared);
        return std::static_pointer_cast<ResultJobType, JobInterface>(shared);
    }

    template <typename ... Args>
    inline std::shared_ptr<FunctionalJob> newSendFunc(Args && ... args)
    {
        return newSendJob<FunctionalJob>(std::forward<Args>(args) ...);
    }
};

} // namespace ex
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_EX_SAFETYASYNC_HPP__

