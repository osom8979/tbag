/**
 * @file   Async.cpp
 * @brief  Async class implementation.
 * @author zer0
 * @date   2016-12-28
 * @date   2017-02-01 (Move package: libtbag/uv -> libtbag/uvpp)
 */

#include <libtbag/uvpp/Async.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvpp/Loop.hpp>

#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

// --------------------
// Global libuv events.
// --------------------

static void __global_uv_async_cb__(uv_async_t * handle)
{
    BaseAsync * async = static_cast<BaseAsync*>(handle->data);
    if (async == nullptr) {
        __tbag_error("__global_uv_async_cb__() handle.data is nullptr.");
    } else if (isDeletedAddress(async)) {
        __tbag_error("__global_uv_async_cb__() handle.data is deleted.");
    } else {
        async->onAsync();
    }
}

// -------------------------
// BaseAsync implementation.
// -------------------------

BaseAsync::BaseAsync() : Handle(uhandle::ASYNC)
{
    // EMPTY.
}

BaseAsync::BaseAsync(Loop & loop) : BaseAsync()
{
    if (init(loop) != uerr::UVPP_SUCCESS) {
        throw std::bad_alloc();
    }
}

BaseAsync::~BaseAsync()
{
    // EMPTY.
}

uerr BaseAsync::init(Loop & loop)
{
    int const CODE = ::uv_async_init(loop.cast<uv_loop_t>(), Parent::cast<uv_async_t>(), __global_uv_async_cb__);
    TBAG_UERR_DEFAULT_RETURN(BaseAsync, init, CODE);
}

uerr BaseAsync::send()
{
    int const CODE = ::uv_async_send(Parent::cast<uv_async_t>());
    TBAG_UERR_DEFAULT_RETURN(BaseAsync, send, CODE);
}

// --------------
// Event methods.
// --------------

void BaseAsync::onAsync()
{
    __tbag_debug("BaseAsync::onAsync() called.");
}

// ---------------------
// Async implementation.
// ---------------------

Async::Async() : BaseAsync(), _exit(false)
{
    // EMPTY.
}

Async::Async(Loop & loop) : BaseAsync(loop), _exit(false)
{
    // EMPTY.
}

Async::~Async()
{
    safeClear();
}

void Async::safeClear()
{
    _jobs.clear();
}

void Async::safePush(SharedJob job)
{
    _jobs.push(job);
}

uerr Async::safeSendJob(SharedJob job)
{
    _jobs.push(job);
    return Parent::send();
}

uerr Async::safeClose()
{
    _exit.store(true);
    return Parent::send();
}

void Async::onAsync()
{
    SharedJob job;
    while (_jobs.frontAndPop(job) == JobQueue::Code::SUCCESS) {
        if (static_cast<bool>(job)) {
            job->run(this);
        }
        job.reset();
    }

    if (_exit) {
        close();
    }
}

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

