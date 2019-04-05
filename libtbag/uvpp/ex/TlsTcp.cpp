/**
 * @file   TlsTcp.cpp
 * @brief  TlsTcp class implementation.
 * @author zer0
 * @date   2019-01-26
 */

#include <libtbag/uvpp/ex/TlsTcp.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvpp/Request.hpp>

#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {
namespace ex   {

// --------------------
// Global libuv events.
// --------------------

static void __global_uv_tcp_connect_tls_cb__(uv_connect_t * request, int status)
{
//    ConnectRequest * req = static_cast<ConnectRequest*>(request->data);
//    if (req == nullptr) {
//        tDLogE("__global_uv_tcp_connect_cb__() request.data is nullptr.");
//    } else if (isDeletedAddress(req)) {
//        tDLogE("__global_uv_tcp_connect_cb__() request.data is deleted.");
//    } else {
//        Tcp * s = static_cast<Tcp*>(req->getOwner());
//        if (s == nullptr) {
//            tDLogE("__global_uv_tcp_connect_cb__() request.data.owner is nullptr.");
//        } else if (isDeletedAddress(s)) {
//            tDLogE("__global_uv_tcp_connect_cb__() request.data.owner is deleted.");
//        } else {
//            s->onConnect(*req, convertUvErrorToErr(status));
//        }
//    }
}

static void __global_uv_connection_tls_cb__(uv_stream_t * server, int status)
{
//    Stream * s = static_cast<Stream*>(server->data);
//    if (s == nullptr) {
//        tDLogE("__global_uv_connection_cb__() server.data is nullptr.");
//    } else if (isDeletedAddress(s)) {
//        tDLogE("__global_uv_connection_cb__() server.data is deleted.");
//    } else {
//        s->onConnection(convertUvErrorToErr(status));
//    }
}

static void __global_uv_stream_alloc_tls_cb__(uv_handle_t * handle, size_t suggested_size, uv_buf_t * buf)
{
//    Stream * s = static_cast<Stream*>(handle->data);
//    if (s == nullptr) {
//        tDLogE("__global_uv_stream_alloc_cb__() handle.data is nullptr.");
//    } else if (isDeletedAddress(s)) {
//        tDLogE("__global_uv_stream_alloc_cb__() handle.data is deleted.");
//    } else {
//        auto result_buffer = s->onAlloc(suggested_size);
//        buf->base = result_buffer.buffer;
//        buf->len  = result_buffer.size;
//    }
}

static void __global_uv_read_tls_cb__(uv_stream_t * stream, ssize_t nread, uv_buf_t const * buf)
{
//    Stream * s = static_cast<Stream*>(stream->data);
//    if (s == nullptr) {
//        tDLogE("__global_uv_read_cb__() stream.data is nullptr.");
//    } else if (isDeletedAddress(s)) {
//        tDLogE("__global_uv_read_cb__() stream.data is deleted.");
//    } else {
//        Err code;
//        if (nread >= 0){
//            code = E_SUCCESS;
//        } else {
//            code = convertUvErrorToErr(static_cast<int>(nread));
//        }
//        s->onRead(code, buf->base, static_cast<std::size_t>(nread));
//    }
}

// ----------------------
// TlsTcp implementation.
// ----------------------

TlsTcp::TlsTcp() : Tcp()
{
    // EMPTY.
}

TlsTcp::TlsTcp(Loop & loop) : Tcp(loop)
{
    // EMPTY.
}

TlsTcp::~TlsTcp()
{
    // EMPTY.
}

Err TlsTcp::connectTls(ConnectRequest & request, sockaddr const * address)
{
    request.setOwner(this); // IMPORTANT!!
    int const CODE = ::uv_tcp_connect(request.cast<uv_connect_t>(),
                                      Parent::cast<uv_tcp_t>(),
                                      address,
                                      __global_uv_tcp_connect_tls_cb__);
    return convertUvErrorToErrWithLogging("TlsTcp::connectTls()", CODE);
}

Err TlsTcp::listenTls(int backlog)
{
    int const CODE = ::uv_listen(Parent::cast<uv_stream_t>(), backlog, __global_uv_connection_tls_cb__);
    return convertUvErrorToErrWithLogging("TlsTcp::listenTls()", CODE);
}

Err TlsTcp::startReadTls()
{
    int const CODE = ::uv_read_start(Parent::cast<uv_stream_t>(),
                                     __global_uv_stream_alloc_tls_cb__,
                                     __global_uv_read_tls_cb__);
    return convertUvErrorToErrWithLogging("TlsTcp::startReadTls()", CODE);
}

Err TlsTcp::writeTls(WriteRequest & request, binf const * infos, std::size_t infos_size)
{
    return write(request, infos, infos_size);
}

Err TlsTcp::writeTls(WriteRequest & request, char const * buffer, std::size_t size)
{
    return write(request, buffer, size);
}

std::size_t TlsTcp::tryWriteTls(binf * infos, std::size_t infos_size, Err * result)
{
    return tryWrite(infos, infos_size, result);
}

std::size_t TlsTcp::tryWriteTls(char const * buffer, std::size_t size, Err * result)
{
    return tryWrite(buffer, size, result);
}

} // namespace ex
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

