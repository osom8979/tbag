/**
 * @file   FsPoll.cpp
 * @brief  FsPoll class implementation.
 * @author zer0
 * @date   2017-05-02
 */

#include <libtbag/uvpp/FsPoll.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/filesystem/details/FsTypes.hpp> // MAX_PATH_LENGTH
#include <libtbag/filesystem/details/UvFs-inl.hpp>

#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

// --------------------
// Global libuv events.
// --------------------

static void __global_uv_fs_poll_cb__(uv_fs_poll_t * handle, int status, uv_stat_t const * prev, uv_stat_t const * curr)
{
    // Callback passed to uv_fs_poll_start() which will be called
    // repeatedly after the handle is started, when any change happens to the monitored path.
    //
    // The callback is invoked with status < 0 if path does not exist or is inaccessible.
    // The watcher is not stopped but your callback is not called again until something changes
    // (e.g. when the file is created or the error reason changes).
    //
    // When status == 0, the callback receives pointers to the old and new uv_stat_t structs.
    // They are valid for the duration of the callback only.

    FsPoll * h = static_cast<FsPoll*>(handle->data);
    if (h == nullptr) {
        tDLogE("__global_uv_fs_poll_cb__() handle.data is nullptr.");
    } else if (isDeletedAddress(h)) {
        tDLogE("__global_uv_fs_poll_cb__() handle.data is deleted.");
    } else {
        using namespace filesystem::details;
        FileState prev_state = {0,};
        FileState curr_state = {0,};
        if (status == 0) {
            prev_state = toFileState(*prev);
            curr_state = toFileState(*curr);
        }
        h->onFsPoll(getUerr(status), prev_state, curr_state);
    }
}

// ----------------------
// FsPoll implementation.
// ----------------------

FsPoll::FsPoll(Loop & loop) : Handle(uhandle::FS_POLL)
{
    if (init(loop) != uerr::UVPP_SUCCESS) {
        throw std::bad_alloc();
    }
}

FsPoll::~FsPoll()
{
    // EMPTY.
}

uerr FsPoll::init(Loop & loop)
{
    int const CODE = ::uv_fs_poll_init(loop.cast<uv_loop_t>(), Parent::cast<uv_fs_poll_t>());
    return getUerr2("FsPoll::init()", CODE);
}

uerr FsPoll::start(char const * path, unsigned int interval)
{
    // Note:
    // For maximum portability, use multi-second intervals.
    // Sub-second intervals will not detect all changes on many file systems.

    int const CODE = ::uv_fs_poll_start(Parent::cast<uv_fs_poll_t>(), __global_uv_fs_poll_cb__, path, interval);
    return getUerr2("FsPoll::start()", CODE);
}

uerr FsPoll::stop()
{
    int const CODE = ::uv_fs_poll_stop(Parent::cast<uv_fs_poll_t>());
    return getUerr2("FsPoll::stop()", CODE);
}

std::string FsPoll::getPath()
{
    // The buffer must be preallocated by the user.
    // Returns 0 on success or an error code < 0 in case of failure.
    // On success, buffer will contain the path and size its length.
    // If the buffer is not big enough UV_ENOBUFS will be returned
    // and size will be set to the required size.
    //
    // Changed in version 1.3.0:
    //  the returned length no longer includes the terminating null byte,
    //  and the buffer is not null terminated.
    //
    // Changed in version 1.9.0:
    //  the returned length includes the terminating null byte on UV_ENOBUFS,
    //  and the buffer is null terminated on success.

    using namespace filesystem::details;
    std::size_t size = MAX_PATH_LENGTH;
    char name[MAX_PATH_LENGTH] = {0,};
    int const CODE = ::uv_fs_poll_getpath(Parent::cast<uv_fs_poll_t>(), name, &size);
    if (getUerr2("FsPoll::getPath()", CODE) != uerr::UVPP_SUCCESS) {
        return std::string();
    }
    return std::string(name, name + size);
}

// --------------
// Event methods.
// --------------

void FsPoll::onFsPoll(uerr status, FileState const & prev, FileState const & curr)
{
    tDLogD("FsPoll::onFsPoll({}) called.", getErrorName(status));
}

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

