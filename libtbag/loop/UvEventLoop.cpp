/**
 * @file   UvEventLoop.cpp
 * @brief  UvEventLoop class implementation.
 * @author zer0
 * @date   2016-10-15
 */

#include <libtbag/loop/UvEventLoop.hpp>
#include <libtbag/log/Log.hpp>

#include <functional>
#include <mutex>

#include <uv.h>

// Developer only.
//#define ENABLE_UV_EVENT_LOOP_DEBUG_VERBOSE

#if defined(ENABLE_UV_EVENT_LOOP_DEBUG_VERBOSE)
# define __UV_EVENT_LOOP_DEBUG_VERBOSE_LOG(format, ...)
#else
# define __UV_EVENT_LOOP_DEBUG_VERBOSE_LOG(format, ...)
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace loop {

/**
 * Pointer to implementation of @c uv_loop_t.
 *
 * @author zer0
 * @date   2016-10-15
 */
struct UvEventLoop::HandlePimpl : public Noncopyable
{
public:
    using Mutex = std::mutex;
    using Guard = std::lock_guard<Mutex>;

    using OnCloseCallback = std::function<void(uv_handle_t*, void*)>;

private:
    Mutex _mutex;
    OnCloseCallback _close;
    uv_loop_t _loop;

public:
    HandlePimpl() : _close([this](uv_handle_t * handle, void * arg){ this->onCloseStep(handle, arg); })
    {
        ::memset((void*)&_loop, 0x00, sizeof(_loop));
        int const CODE = ::uv_loop_init(&_loop);
        if (CODE != 0) {
            throw InitializeException(CODE);
        }
    }

    ~HandlePimpl()
    {
        ::uv_walk(&_loop, _close.target<void(uv_handle_t*, void*)>(), nullptr);
        _mutex.lock();
        ::uv_run(&_loop, UV_RUN_DEFAULT);
        _mutex.unlock();
        ::uv_loop_close(&_loop);
    }

public:
    inline uv_loop_t * getNative()
    { return &_loop; }
    inline uv_loop_t const * getNative() const
    { return &_loop; }

public:
    /** Fully close a loop. */
    void onCloseStep(uv_handle_t * handle, void * arg)
    {
        if (isClosing(handle) == false) {
            // If not closing or closed.
            ::uv_close(handle, nullptr);
        }
    }

public:
    inline static bool isClosing(uv_handle_t const * handle)
    {
        // != 0: if the handle is closing or closed.
        // == 0: otherwise.
        //
        // Note: This function should only be used between
        //       the initialization of the handle and the arrival of the close callback.
        return ::uv_is_closing(handle) != 0;
    }

public:
    bool runDefault()
    {
        Guard guard(_mutex);
        return ::uv_run(&_loop, UV_RUN_DEFAULT) == 0;
    }
};

UvEventLoop::UvEventLoop() throw(InitializeException) : _handle(new HandlePimpl())
{
    // EMPTY.
}

UvEventLoop::~UvEventLoop()
{
    // EMPTY.
}

bool UvEventLoop::runDefault()
{
    return _handle->runDefault();
}

void * UvEventLoop::getNative()
{
    return _handle->getNative();
}

void const * UvEventLoop::getNative() const
{
    return _handle->getNative();
}

} // namespace loop

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

