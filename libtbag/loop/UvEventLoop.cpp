/**
 * @file   UvEventLoop.cpp
 * @brief  UvEventLoop class implementation.
 * @author zer0
 * @date   2016-10-15
 */

#include <libtbag/loop/UvEventLoop.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uv/UvCommon.hpp>

#include <cassert>
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
        int const ERROR_CODE = ::uv_loop_init(&_loop);
        REMOVE_UNUSED_VARIABLE(ERROR_CODE);
        assert(ERROR_CODE == 0);
    }

    ~LoopPimpl()
    {
        //runCloseAll();
        int const CODE = ::uv_loop_close(&_loop);
        if (CODE != 0) {
            if (CODE == UV_EBUSY) {
                __tbag_error("UvEventLoop::LoopPimpl::~LoopPimpl() uv_loop is busy.");
            } else {
                __tbag_error("UvEventLoop::LoopPimpl::~LoopPimpl() close uv_loop error [{}]", CODE);
            }
        }
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

    bool runCloseAll()
    {
        ::uv_walk(&_loop, &LoopPimpl::onClose, nullptr);
        return runDefault();
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

public:
    /** Fully close a loop. */
    static void onClose(uv_handle_t * handle, void * arg)
    {
        if (uv::isUvNativeHandleType(handle) == false) {
            return;
        }
        if (isClosing(handle)) {
            return;
        }

        // If not closing or closed.
        __tbag_debug("Not closing or closed uv handle: {}", uv::getUvNativeHandleName(handle));
        ::uv_close(handle, nullptr);
    }
};

// ---------------------------
// UvEventLoop implementation.
// ---------------------------

UvEventLoop::UvEventLoop() : _loop(new LoopPimpl())
{
    // EMPTY.
}

UvEventLoop::~UvEventLoop()
{
    // EMPTY.
}

bool UvEventLoop::runDefault()
{
    return _loop->runDefault();
}

void * UvEventLoop::getNative()
{
    return _loop->getNative();
}

void const * UvEventLoop::getNative() const
{
    return _loop->getNative();
}

} // namespace loop

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

