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
            __tbag_error("CloseJob::run() handle is nullptr.");
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
        __tbag_debug("SafetyAsync::MistakeInspector::onIdle() async is closing.");
    } else {
        _async._jobs.safeRun([&](JobQueue::Queue & queue){
            if (queue.empty() == false) {
                if (_async.send() != uerr::UVPP_SUCCESS) {
                    __tbag_error("SafetyAsync::MistakeInspector::onIdle() send error.");
                }
            }
        });
    }
}

void SafetyAsync::MistakeInspector::onClose()
{
    __tbag_debug("SafetyAsync::MistakeInspector::onClose() called.");
}

// ---------------------------
// SafetyAsync implementation.
// ---------------------------

SafetyAsync::SafetyAsync(Loop & loop) : BaseAsync(loop), _inspector(nullptr)
{
    _inspector = loop.newHandle<MistakeInspector>(loop, *this);
    _inspector->start();
    // EMPTY.
}

SafetyAsync::~SafetyAsync()
{
    // EMPTY.
}

void SafetyAsync::clearJob()
{
    _jobs.clear();
}

uerr SafetyAsync::sendJob(SharedJob job)
{
    _jobs.push(job);
    return send();
}

uerr SafetyAsync::sendCloseJob()
{
    _jobs.push(SharedJob(new (std::nothrow) __impl::CloseJob));
    return send();
}

void SafetyAsync::onAsync()
{
    if (_inspector && _inspector->isClosing()) {
        return;
    }

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
    if (_inspector && _inspector->isActive()) {
        _inspector->stop();
        _inspector->close();
    }

    __tbag_debug("SafetyAsync::onClose() called.");
}

} // namespace ex
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

