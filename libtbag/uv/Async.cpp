/**
 * @file   Async.cpp
 * @brief  Async class implementation.
 * @author zer0
 * @date   2016-12-28
 */

#include <libtbag/uv/Async.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uv/Loop.hpp>

#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uv {

// --------------------
// Global libuv events.
// --------------------

static void __global_uv_async_cb__(uv_async_t * handle)
{
    BaseAsync * async = static_cast<BaseAsync*>(handle->data);
    if (async == nullptr) {
        __tbag_error("__global_uv_async_cb__() handle data is nullptr.");
        return;
    }
    async->onAsync();
}

// -------------------------
// BaseAsync implementation.
// -------------------------

BaseAsync::BaseAsync() : Handle(UvHandleType::ASYNC)
{
    // EMPTY.
}

BaseAsync::BaseAsync(Loop & loop) : BaseAsync()
{
    if (init(loop) == false) {
        throw std::bad_alloc();
    }
}

BaseAsync::~BaseAsync()
{
    // EMPTY.
}

bool BaseAsync::init(Loop & loop)
{
    int const CODE = ::uv_async_init(loop.cast<uv_loop_t>(), Parent::cast<uv_async_t>(), __global_uv_async_cb__);
    if (CODE != 0) {
        __tbag_error("BaseAsync::BaseAsync() error [{}] {}", CODE, getUvErrorName(CODE));
        return false;
    }
    return true;
}

bool BaseAsync::send()
{
    int const CODE = ::uv_async_send(Parent::cast<uv_async_t>());
    if (CODE != 0) {
        __tbag_error("BaseAsync::send() error [{}] {}", CODE, getUvErrorName(CODE));
        return false;
    }
    return true;
}

// --------------
// Event methods.
// --------------

void BaseAsync::onAsync()
{
    // EMPTY.
}

// ---------------------
// Async implementation.
// ---------------------

Async::Async() : BaseAsync(), _continuous_deququ(false)
{
    // EMPTY.
}

Async::Async(Loop & loop) : BaseAsync(loop), _continuous_deququ(false)
{
    // EMPTY.
}

Async::~Async()
{
    clear();
}

void Async::clear()
{
    _jobs.clear();
}

void Async::push(SharedJob job)
{
    _jobs.push(job);
}

bool Async::sendJob(SharedJob job)
{
    _jobs.push(job);
    return Parent::send();
}

void Async::onAsync()
{
    SharedJob job;
    do {
        auto code = _jobs.frontAndPop(job);
        if (code == JobQueue::Code::EMPTY_CONTAINER) {
            break;
        } else if (code == JobQueue::Code::SUCCESS && static_cast<bool>(job)) {
            job->run(*this);
        }
    } while (_continuous_deququ.load());
}

} // namespace uv

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

