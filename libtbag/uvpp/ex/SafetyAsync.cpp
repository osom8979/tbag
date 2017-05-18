/**
 * @file   SafetyAsync.cpp
 * @brief  SafetyAsync class implementation.
 * @author zer0
 * @date   2017-05-01
 */

#include <libtbag/uvpp/ex/SafetyAsync.hpp>
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {
namespace ex   {

namespace __impl {

struct CloseJob : public SafetyAsync::Job
{
    virtual void run(SafetyAsync * handle) override
    {
        if (handle != nullptr) {
            handle->close();
        } else {
            tDLogE("CloseJob::run() handle is nullptr.");
        }
    }
};

} // namespace __impl

// --------------------------------
// MistakeInspector implementation.
// --------------------------------

SafetyAsync::MistakeInspector::MistakeInspector(Loop & loop, SafetyAsync & async) : Idle(loop), _async(async)
{
    // EMPTY.
}

SafetyAsync::MistakeInspector::~MistakeInspector()
{
    // EMPTY.
}

void SafetyAsync::MistakeInspector::onIdle()
{
    if (_async.isClosing()) {
        tDLogD("SafetyAsync::MistakeInspector::onIdle() async is closing.");
    } else {
        _async._jobs.safeRun([&](JobQueue::Queue & queue){
            if (queue.empty()) {
                if (_async._inspector->isClosing() == false && _async._inspector->isActive()) {
                    tDLogD("SafetyAsync::MistakeInspector::onIdle() stop inspector!");
                    _async._inspector->stop();
                }
            } else {
                if (_async.send() != Err::E_SUCCESS) {
                    tDLogE("SafetyAsync::MistakeInspector::onIdle() send error.");
                }
            }
        });
    }
}

void SafetyAsync::MistakeInspector::onClose()
{
    tDLogD("SafetyAsync::MistakeInspector::onClose() called.");
}

// ---------------------------
// SafetyAsync implementation.
// ---------------------------

SafetyAsync::SafetyAsync(Loop & loop) : BaseAsync(loop), _inspector(nullptr)
{
    _inspector = loop.newHandle<MistakeInspector>(loop, *this);
    assert(_inspector->isActive() == false);
    _inspector->start();
}

SafetyAsync::~SafetyAsync()
{
    // EMPTY.
}

void SafetyAsync::clearJob()
{
    _jobs.clear();
}

Err SafetyAsync::sendJob(SharedJob job)
{
    _jobs.safeRun([&](JobQueue::Queue & queue){
        queue.push(job);
        if (_inspector->isActive() == false) {
            tDLogD("SafetyAsync::sendJob() start inspector.");
            _inspector->start();
        }
    });
    return send();
}

Err SafetyAsync::sendCloseJob()
{
    return sendJob(SharedJob(new (std::nothrow) __impl::CloseJob));
}

void SafetyAsync::onAsync()
{
    SharedJob job;
    if (_jobs.frontAndPop(job) == JobQueue::Code::SUCCESS) {
        if (static_cast<bool>(job)) {
            job->run(this);
        }
        job.reset();
    }
}

void SafetyAsync::onClose()
{
    _jobs.clear();
    if (_inspector->isActive()) {
        _inspector->stop();
    }
    _inspector->close();

    tDLogD("SafetyAsync::onClose() called.");
}

} // namespace ex
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

