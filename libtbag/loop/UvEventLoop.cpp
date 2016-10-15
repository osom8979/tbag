/**
 * @file   UvEventLoop.cpp
 * @brief  UvEventLoop class implementation.
 * @author zer0
 * @date   2016-10-15
 */

#include <libtbag/loop/UvEventLoop.hpp>
#include <libtbag/log/Log.hpp>
#include <mutex>

#include <uv.h>

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
struct UvEventLoop::LoopPimpl : public Noncopyable
{
public:
    using Mutex = std::mutex;
    using Guard = std::lock_guard<Mutex>;

private:
    Mutex _mutex;
    uv_loop_t _loop;

public:
    LoopPimpl()
    {
        ::memset((void*)&_loop, 0x00, sizeof(_loop));
        int const CODE = ::uv_loop_init(&_loop);
        if (CODE != 0) {
            throw InitializeException(CODE);
        }
    }

    ~LoopPimpl()
    {
        ::uv_walk(&_loop, &LoopPimpl::onClose, nullptr);
        runDefault();
        ::uv_loop_close(&_loop);
    }

public:
    inline uv_loop_t * getNative()
    { return &_loop; }
    inline uv_loop_t const * getNative() const
    { return &_loop; }

public:
    bool run(uv_run_mode mode)
    {
        Guard guard(_mutex);
        return ::uv_run(&_loop, mode) == 0;
    }

    bool runDefault()
    {
        return run(UV_RUN_DEFAULT);
    }

public:
    /** Fully close a loop. */
    static void onClose(uv_handle_t * handle, void * arg)
    {
        if (isClosing(handle) == false) {
            // If not closing or closed.
            ::uv_close(handle, nullptr);
        }
    }

public:
    static bool isClosing(uv_handle_t const * handle)
    {
        // != 0: if the handle is closing or closed.
        // == 0: otherwise.
        //
        // Note: This function should only be used between
        //       "the initialization of the handle" and "the arrival of the close callback".
        return ::uv_is_closing(handle) != 0;
    }
};

UvEventLoop::UvEventLoop() throw(InitializeException) : _handle(new LoopPimpl())
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

