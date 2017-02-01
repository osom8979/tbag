/**
 * @file   Stream.cpp
 * @brief  Stream class implementation.
 * @author zer0
 * @date   2016-12-27
 * @date   2017-02-01 (Move package: libtbag/uv -> libtbag/uvpp)
 */

#include <libtbag/uvpp/Stream.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvpp/Request.hpp>
#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

// --------------------
// Global libuv events.
// --------------------

static void __global_uv_shutdown_cb__(uv_shutdown_t * request, int status)
{
    // Callback called after a shutdown request has been completed.
    // status will be 0 in case of success, < 0 otherwise.

    ShutdownRequest * req = static_cast<ShutdownRequest*>(request->data);
    if (req == nullptr) {
        __tbag_error("__global_uv_shutdown_cb__() request data is nullptr.");
        return;
    }

    Stream * s = static_cast<Stream*>(req->getOwner());
    if (s == nullptr) {
        __tbag_error("__global_uv_shutdown_cb__() request owner is nullptr.");
        return;
    }
    s->onShutdown(*req, status == 0 ? Err::SUCCESS : Err::FAILURE);
}

static void __global_uv_connection_cb__(uv_stream_t * server, int status)
{
    // Callback called when a stream server has received an incoming connection.
    // The user can accept the connection by calling uv_accept().
    // status will be 0 in case of success, < 0 otherwise.

    Stream * s = static_cast<Stream*>(server->data);
    if (s == nullptr) {
        __tbag_error("__global_uv_connection_cb__() server data is nullptr.");
        return;
    }
    s->onConnection(status == 0 ? Err::SUCCESS : Err::FAILURE);
}

static void __global_uv_stream_alloc_cb__(uv_handle_t * handle, size_t suggested_size, uv_buf_t * buf)
{
    // Type definition for callback passed to uv_read_start() and uv_udp_recv_start().
    // The user must allocate memory and fill the supplied uv_buf_t structure.
    // If NULL is assigned as the buffer’s base or 0 as its length, a UV_ENOBUFS error
    // will be triggered in the uv_udp_recv_cb or the uv_read_cb callback.
    //
    // A suggested size (65536 at the moment in most cases) is provided,
    // but it’s just an indication, not related in any way to the pending data to be read.
    // The user is free to allocate the amount of memory they decide.

    Stream * s = static_cast<Stream*>(handle->data);
    if (s == nullptr) {
        __tbag_error("__global_uv_stream_alloc_cb__() handle.data is nullptr.");
        return;
    }

    auto result_buffer = s->onAlloc(suggested_size);
    buf->base = result_buffer.buffer;
    buf->len  = result_buffer.size;
}

static void __global_uv_read_cb__(uv_stream_t * stream, ssize_t nread, uv_buf_t const * buf)
{
    // nread is > 0 if there is data available or < 0 on error.
    // When we’ve reached EOF, nread will be set to UV_EOF.
    // When nread < 0, the buf parameter might not point to a valid buffer;
    // in that case buf.len and buf.base are both set to 0.
    //
    // nread might be 0, which does not indicate an error or EOF.
    // This is equivalent to EAGAIN or EWOULDBLOCK under read(2).

    Stream * s = static_cast<Stream*>(stream->data);
    if (s == nullptr) {
        __tbag_error("__global_uv_read_cb__() stream.data is nullptr.");
        return;
    }

    Err code;
    if (nread == UV_EOF) {
        code = Err::END_OF_FILE;
    } else if (nread == UV_ECONNRESET){
        code = Err::CONNECTION_RESET;
    } else if (nread >= 0){
        code = Err::SUCCESS;
    } else {
        __tbag_debug("__global_uv_read_cb__() error [{}] {}.", nread, getUvErrorName(nread));
        code = Err::FAILURE;
    }

    s->onRead(code, buf->base, static_cast<std::size_t>(nread));
}

static void __global_uv_write_cb__(uv_write_t * request, int status)
{
    // Callback called after data was written on a stream.
    // status will be 0 in case of success, < 0 otherwise.

    WriteRequest * req = static_cast<WriteRequest*>(request->data);
    if (req == nullptr) {
        __tbag_error("__global_uv_write_cb__() request data is nullptr.");
        return;
    }

    Stream * s = static_cast<Stream*>(req->getOwner());
    if (s == nullptr) {
        __tbag_error("__global_uv_write_cb__() request owner is nullptr.");
        return;
    }
    s->onWrite(*req, status == 0 ? Err::SUCCESS : Err::FAILURE);
}

// ----------------------
// Stream implementation.
// ----------------------

Stream::Stream(UvHandleType type) : Handle(type)
{
    if (isStream() == false) {
        __tbag_error("Stream::Stream({}) type is not stream type", static_cast<int>(type));
        throw std::bad_alloc();
    }
}

Stream::~Stream()
{
    // EMPTY.
}

std::size_t Stream::getWriteQueueSize() const TBAG_NOEXCEPT
{
    return Parent::cast<uv_stream_t>()->write_queue_size;
}

bool Stream::isReadable() const TBAG_NOEXCEPT
{
    return ::uv_is_readable(Parent::cast<const uv_stream_t>()) == 1;
}

bool Stream::isWritable() const TBAG_NOEXCEPT
{
    return ::uv_is_writable(Parent::cast<const uv_stream_t>()) == 1;
}

bool Stream::setBlocking(bool enable)
{
    // When blocking mode is enabled all writes complete synchronously.
    // The interface remains unchanged otherwise,
    // e.g. completion or failure of the operation will still
    // be reported through a callback which is made asynchronously.
    //
    // Warning:
    // Relying too much on this API is not recommended.
    // It is likely to change significantly in the future.
    // Currently only works on Windows for uv_pipe_t handles.
    // On UNIX platforms, all uv_stream_t handles are supported.
    //
    // Also libuv currently makes no ordering guarantee when the blocking mode
    // is changed after write requests have already been submitted.
    // Therefore it is recommended to set the blocking mode immediately
    // after opening or creating the stream.

    int const CODE = ::uv_stream_set_blocking(Parent::cast<uv_stream_t>(), enable ? 1 : 0);
    if (CODE != 0) {
        __tbag_error("Stream::setBlocking() error [{}] {}", CODE, getUvErrorName(CODE));
        return false;
    }
    return true;
}

bool Stream::shutdown(ShutdownRequest & request)
{
    request.setOwner(this); // IMPORTANT!!

    // It waits for pending write requests to complete.
    // The handle should refer to a initialized stream.
    // req should be an uninitialized shutdown request struct.
    // The cb is called after shutdown is complete.

    int const CODE = ::uv_shutdown(request.cast<uv_shutdown_t>(),
                                   Parent::cast<uv_stream_t>(),
                                   __global_uv_shutdown_cb__);
    if (CODE != 0) {
        __tbag_error("Stream::shutdown() error [{}] {}", CODE, getUvErrorName(CODE));
        return false;
    }
    return true;
}

bool Stream::listen(int backlog)
{
    // backlog indicates the number of connections the kernel might queue, same as listen(2).
    // When a new incoming connection is received the uv_connection_cb callback is called.
    int const CODE = ::uv_listen(Parent::cast<uv_stream_t>(), backlog, __global_uv_connection_cb__);
    if (CODE != 0) {
        // EADDRINUSE: The other socket is using the same port.
        // EBADF: Bad socket descriptor
        // EOPNOTSUP: Unsupported listen operator.
        // ENOTSOCK: It is not a socket.
        __tbag_error("Stream::listen() error [{}] {}", CODE, getUvErrorName(CODE));
        return false;
    }
    return true;
}

bool Stream::accept(Stream & client)
{
    // This call is used in conjunction with uv_listen() to accept incoming connections.
    // Call this function after receiving a uv_connection_cb to accept the connection.
    // Before calling this function the client handle must be initialized. < 0 return
    // value indicates an error.
    //
    // When the uv_connection_cb callback is called it is guaranteed that this function
    // will complete successfully the first time. If you attempt to use it more than once,
    // it may fail. It is suggested to only call this function once per uv_connection_cb call.

    int const CODE = ::uv_accept(Parent::cast<uv_stream_t>(), client.cast<uv_stream_t>());
    if (CODE != 0) {
        __tbag_error("Stream::accept() error [{}] {}", CODE, getUvErrorName(CODE));
        return false;
    }
    return true;
}

bool Stream::startRead()
{
    // The uv_read_cb callback will be made several times until
    // there is no more data to read or uv_read_stop() is called.

    int const CODE = ::uv_read_start(Parent::cast<uv_stream_t>(), __global_uv_stream_alloc_cb__, __global_uv_read_cb__);
    if (CODE != 0) {
        __tbag_error("Stream::startRead() error [{}] {}", CODE, getUvErrorName(CODE));
        return false;
    }
    return true;
}

bool Stream::stopRead()
{
    // The uv_read_cb callback will no longer be called.
    // This function is idempotent and may be safely called on a stopped stream.

    int const CODE = ::uv_read_stop(Parent::cast<uv_stream_t>());
    if (CODE != 0) {
        __tbag_error("Stream::startRead() error [{}] {}", CODE, getUvErrorName(CODE));
        return false;
    }
    return true;
}

bool Stream::write(WriteRequest & request, binf * infos, std::size_t infos_size)
{
    request.setOwner(this); // IMPORTANT!!

    std::vector<uv_buf_t> uv_infos;
    uv_infos.resize(infos_size);
    for (std::size_t i = 0; i < infos_size; ++i) {
        uv_infos[i].base = (infos + i)->buffer;
        uv_infos[i].len  = (infos + i)->size;
    }

    int const CODE = ::uv_write(request.cast<uv_write_t>(),
                                Parent::cast<uv_stream_t>(),
                                &uv_infos[0],
                                uv_infos.size(),
                                __global_uv_write_cb__);
    if (CODE != 0) {
        __tbag_error("Stream::write() error [{}] {}", CODE, getUvErrorName(CODE));
        return false;
    }
    return true;
}

bool Stream::write(WriteRequest & request, char const * buffer, std::size_t size)
{
    binf info;
    info.buffer = const_cast<char*>(buffer);
    info.size   = size;
    return write(request, &info, 1U);
}

std::size_t Stream::tryWrite(binf * infos, std::size_t infos_size, Err * result)
{
    // Same as uv_write(), but won’t queue a write request if it can’t be completed immediately.
    // Will return either:
    // > 0: number of bytes written (can be less than the supplied buffer size).
    // < 0: negative error code (UV_EAGAIN is returned if no data can be sent immediately).

    std::vector<uv_buf_t> uv_infos;
    uv_infos.resize(infos_size);
    for (std::size_t i = 0; i < infos_size; ++i) {
        uv_infos[i].base = (infos + i)->buffer;
        uv_infos[i].len  = (infos + i)->size;
    }

    // @formatter:off
    int const WRITE_RESULT = ::uv_try_write(Parent::cast<uv_stream_t>(), &uv_infos[0], uv_infos.size());
    if (WRITE_RESULT <= 0) {
        if (result != nullptr) { *result = Err::FAILURE; }
        __tbag_error("Stream::tryWrite() error [{}] {}", WRITE_RESULT, getUvErrorName(WRITE_RESULT));
        return 0U;
    }
    if (result != nullptr) { *result = Err::SUCCESS; }
    return static_cast<std::size_t>(WRITE_RESULT);
    // @formatter:on
}

std::size_t Stream::tryWrite(char const * buffer, std::size_t size, Err * result)
{
    binf info;
    info.buffer = const_cast<char*>(buffer);
    info.size   = size;
    return tryWrite(&info, 1U, result);
}

// --------------
// Event methods.
// --------------

void Stream::onShutdown(ShutdownRequest & request, Err code)
{
    __tbag_debug("Stream::onShutdown({}) called.", static_cast<int>(code));
}

void Stream::onConnection(Err code)
{
    __tbag_debug("Stream::onConnection({}) called.", static_cast<int>(code));
}

binf Stream::onAlloc(std::size_t suggested_size)
{
    __tbag_debug("Stream::onAlloc() called (suggested_size:{}).", suggested_size);
    return binf((char*)::malloc(suggested_size), suggested_size);
}

void Stream::onRead(Err code, char const * buffer, std::size_t size)
{
    __tbag_debug("Stream::onRead({}) called (size:{}).", static_cast<int>(code), size);
}

void Stream::onWrite(WriteRequest & request, Err code)
{
    __tbag_debug("Stream::onWrite({}) called.", static_cast<int>(code));
}

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

