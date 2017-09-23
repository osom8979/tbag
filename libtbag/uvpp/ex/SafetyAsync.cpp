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

// --------------------------------
// MistakeInspector implementation.
// --------------------------------

SafetyAsync::MistakeInspector::MistakeInspector(Loop & loop, SafetyAsync & async)
        : Idle(loop), _async(async)
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
        // [WARNING] Don't use the inspector assertion of closing!
        // e.g. assert(_async._inspector->isClosing());
        tDLogD("SafetyAsync::MistakeInspector::onIdle() async is closing ...");
        return; // async is closing ...
    }

    _async._jobs.safeRun([&](JobQueue::Queue & queue){
        if (queue.empty()) {
            _async.stopInspector();
        } else {
            if (_async.send() != Err::E_SUCCESS) {
                tDLogE("SafetyAsync::MistakeInspector::onIdle() send error.");
            }
        }
    });
}

void SafetyAsync::MistakeInspector::onClose()
{
    // EMPTY.
}

// ---------------------------
// SafetyAsync implementation.
// ---------------------------

SafetyAsync::SafetyAsync(Loop & loop) : Async(loop), _inspector(nullptr)
{
    _inspector = loop.newInternalHandle<MistakeInspector>(true, loop, *this);
    assert(_inspector->isActive() == false);
    _inspector->start();
}

SafetyAsync::~SafetyAsync()
{
    // EMPTY.
}

void SafetyAsync::closeInspector()
{
    if (static_cast<bool>(_inspector) == false) {
        return;
    }

    if (_inspector->isActive()) {
        _inspector->stop();
    }
    if (_inspector->isClosing() == false) {
        _inspector->close();
    }
}

Err SafetyAsync::startInspector()
{
    if (static_cast<bool>(_inspector) == false) {
        return Err::E_EXPIRED;
    }

    if (_inspector->isClosing()) {
        return Err::E_CLOSING;
    }

    if (_inspector->isActive()) {
        return Err::E_ALREADY;
    }

    Err const CODE = _inspector->start();
    if (CODE != Err::E_SUCCESS) {
        return CODE;
    }

    tDLogD("SafetyAsync::startInspector()");
    return CODE;
}

Err SafetyAsync::stopInspector()
{
    if (static_cast<bool>(_inspector) == false) {
        return Err::E_EXPIRED;
    }

    if (_inspector->isClosing()) {
        return Err::E_CLOSING;
    }

    if (_inspector->isActive() == false) {
        return Err::E_ALREADY;
    }

    Err const CODE = _inspector->stop();
    if (CODE != Err::E_SUCCESS) {
        return CODE;
    }

    tDLogD("SafetyAsync::stopInspector()");
    return CODE;
}

void SafetyAsync::clearJob()
{
    _jobs.clear();
}

Err SafetyAsync::sendJob(SharedJob job)
{
    _jobs.safeRun([&](JobQueue::Queue & queue){
        queue.push(job);
        startInspector();
    });
    return send();
}

Err SafetyAsync::sendClose()
{
    auto job = SharedJob(new (std::nothrow) FunctionalJob([&](){
        this->close();
    }));

    if (static_cast<bool>(job) == false) {
        return Err::E_BADALLOC;
    }
    return sendJob(job);
}

void SafetyAsync::onAsync()
{
    SharedJob job;
    if (_jobs.frontAndPop(job) == Err::E_SUCCESS) {
        if (static_cast<bool>(job)) {
            job->run();
        }
        job.reset();
    }
}

void SafetyAsync::onClose()
{
    tDLogD("SafetyAsync::onClose()");
    clearJob();
    closeInspector();
}

} // namespace ex
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

