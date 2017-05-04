/**
 * @file   FsEvent.cpp
 * @brief  FsEvent class implementation.
 * @author zer0
 * @date   2017-05-02
 */

#include <libtbag/uvpp/FsEvent.hpp>
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

static void __global_uv_fs_event_cb__(uv_fs_event_t * handle, const char * filename, int events, int status)
{
    // Callback passed to uv_fs_event_start() which will be called repeatedly after the handle is started.
    // If the handle was started with a directory the filename parameter will be a relative path
    // to a file contained in the directory. The events parameter is an ORed mask of uv_fs_event elements.

    FsEvent * h = static_cast<FsEvent*>(handle->data);
    if (h == nullptr) {
        __tbag_error("__global_uv_fs_event_cb__() handle.data is nullptr.");
    } else if (isDeletedAddress(h)) {
        __tbag_error("__global_uv_fs_event_cb__() handle.data is deleted.");
    } else {
        h->onFsEvent(filename, FsEvent::getEvent(events), getUerr(status));
    }
}

// -----------------------
// FsEvent implementation.
// -----------------------

FsEvent::FsEvent() : Handle(uhandle::FS_EVENT)
{
    // EMPTY.
}

FsEvent::FsEvent(Loop & loop) : FsEvent()
{
    if (init(loop) != uerr::UVPP_SUCCESS) {
        throw std::bad_alloc();
    }
}

FsEvent::~FsEvent()
{
    // EMPTY.
}

uerr FsEvent::init(Loop & loop)
{
    int const CODE = ::uv_fs_event_init(loop.cast<uv_loop_t>(), Parent::cast<uv_fs_event_t>());
    return getUerr2("FsEvent::init()", CODE);
}

uerr FsEvent::start(char const * path, EventFlag flags)
{
    int const CODE = ::uv_fs_event_start(Parent::cast<uv_fs_event_t>(), __global_uv_fs_event_cb__, path, static_cast<unsigned int>(flags));
    return getUerr2("FsEvent::start()", CODE);
}

uerr FsEvent::stop()
{
    int const CODE = ::uv_fs_event_stop(Parent::cast<uv_fs_event_t>());
    return getUerr2("FsEvent::stop()", CODE);
}

std::string FsEvent::getPath()
{
    // The buffer must be preallocated by the user.
    // Returns 0 on success or an error code < 0 in case of failure.
    // On success, buffer will contain the path and size its length.
    // If the buffer is not big enough UV_ENOBUFS will be returned
    // and size will be set to the required size, including the null terminator.
    //
    // Changed in version 1.3.0:
    // the returned length no longer includes the terminating null byte, and the buffer is not null terminated.
    //
    // Changed in version 1.9.0:
    // the returned length includes the terminating null byte on UV_ENOBUFS, and the buffer is null terminated on success.

    using namespace filesystem::details;
    std::size_t size = MAX_PATH_LENGTH;
    char name[MAX_PATH_LENGTH] = {0,};
    int const CODE = ::uv_fs_event_getpath(Parent::cast<uv_fs_event_t>(), name, &size);
    if (getUerr2("FsEvent::getPath()", CODE) != uerr::UVPP_SUCCESS) {
        return std::string();
    }
    return std::string(name, name + size);
}

FsEvent::Event FsEvent::getEvent(int native_events) TBAG_NOEXCEPT
{
    return static_cast<FsEvent::Event>(native_events);
}

// --------------
// Event methods.
// --------------

void FsEvent::onFsEvent(const char * filename, Event events, uerr status)
{
    __tbag_debug("FsEvent::onFsEvent({}, {}, {}) called.",
                 filename, static_cast<int>(events), getErrorName(status));
}

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

