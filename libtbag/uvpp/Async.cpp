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
    return getUerr2("BaseAsync::init()", CODE);
}

uerr BaseAsync::send()
{
    int const CODE = ::uv_async_send(Parent::cast<uv_async_t>());
    return getUerr2("BaseAsync::send()", CODE);
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

Async::Async() : BaseAsync()
{
    // EMPTY.
}

Async::Async(Loop & loop) : BaseAsync(loop)
{
    // EMPTY.
}

Async::~Async()
{
    // EMPTY.
}

void Async::clearJob()
{
    _jobs.clear();
}

void Async::pushJob(SharedJob job)
{
    _jobs.push(job);
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
}

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

