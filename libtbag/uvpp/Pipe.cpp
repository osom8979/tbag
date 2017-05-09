/**
 * @file   Pipe.cpp
 * @brief  Pipe class implementation.
 * @author zer0
 * @date   2017-05-02
 */

#include <libtbag/uvpp/Pipe.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/uvpp/Request.hpp>
#include <libtbag/filesystem/details/FsTypes.hpp> // MAX_PATH_LENGTH

#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

// --------------------
// Global libuv events.
// --------------------

static void __global_uv_pipe_connect_cb__(uv_connect_t * request, int status)
{
    // Callback called after a connection started by uv_connect() is done.
    // status will be 0 in case of success, < 0 otherwise.

    ConnectRequest * req = static_cast<ConnectRequest*>(request->data);
    if (req == nullptr) {
        tDLogE("__global_uv_pipe_connect_cb__() request.data is nullptr.");
    } else if (isDeletedAddress(req)) {
        tDLogE("__global_uv_pipe_connect_cb__() request.data is deleted.");
    } else {
        Pipe * p = static_cast<Pipe*>(req->getOwner());
        if (p == nullptr) {
            tDLogE("__global_uv_pipe_connect_cb__() request.data.owner is nullptr.");
        } else if (isDeletedAddress(p)) {
            tDLogE("__global_uv_pipe_connect_cb__() request.data.owner is deleted.");
        } else {
            p->onConnect(*req, getUerr(status));
        }
    }
}

// --------------------
// Pipe implementation.
// --------------------

Pipe::Pipe() : Stream(uhandle::PIPE)
{
    // EMPTY.
}

Pipe::Pipe(Loop & loop, bool is_ipc) : Pipe()
{
    if (init(loop, is_ipc) != uerr::UVPP_SUCCESS) {
        throw std::bad_alloc();
    }
}

Pipe::~Pipe()
{
    // EMPTY.
}

uerr Pipe::init(Loop & loop, bool is_ipc)
{
    // The ipc argument is a boolean to indicate
    // if this pipe will be used for handle passing between processes.

    int const CODE = ::uv_pipe_init(loop.cast<uv_loop_t>(), Parent::cast<uv_pipe_t>(), (is_ipc ? 1 : 0));
    return getUerr2("Pipe::init()", CODE);
}

uerr Pipe::open(ufile file)
{
    // Note:
    // The passed file descriptor or HANDLE is not checked for its type,
    // but it’s required that it represents a valid pipe.

    int const CODE = ::uv_pipe_open(Parent::cast<uv_pipe_t>(), file);
    return getUerr2("Pipe::open()", CODE);
}

uerr Pipe::bind(char const * name)
{
    // Note:
    // Paths on Unix get truncated to sizeof(sockaddr_un.sun_path) bytes,
    // typically between 92 and 108 bytes.

    int const CODE = ::uv_pipe_bind(Parent::cast<uv_pipe_t>(), name);
    return getUerr2("Pipe::bind()", CODE);
}

void Pipe::connect(ConnectRequest & request, char const * name)
{
    request.setOwner(this); // IMPORTANT!!

    // Note:
    // Paths on Unix get truncated to sizeof(sockaddr_un.sun_path) bytes,
    // typically between 92 and 108 bytes.

    ::uv_pipe_connect(request.cast<uv_connect_t>(), Parent::cast<uv_pipe_t>(), name, __global_uv_pipe_connect_cb__);
}

std::string Pipe::getSockName() const
{
    // A preallocated buffer must be provided.
    // The size parameter holds the length of the buffer and
    // it’s set to the number of bytes written to the buffer on output.
    // If the buffer is not big enough UV_ENOBUFS will be returned and len will contain the required size.
    //
    // Changed in version 1.3.0:
    // the returned length no longer includes the terminating null byte, and the buffer is not null terminated.

    using namespace filesystem::details;
    std::size_t size = MAX_PATH_LENGTH;
    char name[MAX_PATH_LENGTH] = {0,};
    int const CODE = ::uv_pipe_getsockname(Parent::cast<uv_pipe_t>(), name, &size);
    if (getUerr2("Pipe::getSockName()", CODE) != uerr::UVPP_SUCCESS) {
        return std::string();
    }
    return std::string(name, name + size);
}

std::string Pipe::getPeerName() const
{
    // A preallocated buffer must be provided.
    // The size parameter holds the length of the buffer and
    // it’s set to the number of bytes written to the buffer on output.
    // If the buffer is not big enough UV_ENOBUFS will be returned and len will contain the required size.
    //
    // New in version 1.3.0.

    using namespace filesystem::details;
    std::size_t size = MAX_PATH_LENGTH;
    char name[MAX_PATH_LENGTH] = {0,};
    int const CODE = ::uv_pipe_getpeername(Parent::cast<uv_pipe_t>(), name, &size);
    if (getUerr2("Pipe::getPeerName()", CODE) != uerr::UVPP_SUCCESS) {
        return std::string();
    }
    return std::string(name, name + size);
}

void Pipe::pendingInstances(int count)
{
    // Note: This setting applies to Windows only.
    ::uv_pipe_pending_instances(Parent::cast<uv_pipe_t>(), count);
}

int Pipe::getPendingCount()
{
    return ::uv_pipe_pending_count(Parent::cast<uv_pipe_t>());
}

int Pipe::getPendingType()
{
    // First - call uv_pipe_pending_count(),
    // if it’s > 0 then initialize a handle of the given type,
    // returned by uv_pipe_pending_type() and call uv_accept(pipe, handle).

    return ::uv_pipe_pending_type(Parent::cast<uv_pipe_t>());
}

// --------------
// Event methods.
// --------------

void Pipe::onConnect(ConnectRequest & request, uerr code)
{
    tDLogD("Pipe::onConnect({}) called.", getErrorName(code));
}

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

