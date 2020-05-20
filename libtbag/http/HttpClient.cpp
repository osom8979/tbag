/**
 * @file   HttpClient.cpp
 * @brief  HttpClient class implementation.
 * @author zer0
 * @date   2020-03-10
 */

#include <libtbag/http/HttpClient.hpp>
#include <libtbag/http/CivetWebExtension.hpp>

#include <cassert>
#include <cstring>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace http {

using ErrString = HttpClient::ErrString;

HttpClient::HttpClient() : _conn(nullptr, &mg_close_connection)
{
    // EMPTY.
}

HttpClient::~HttpClient()
{
    // EMPTY.
}

ErrMsg HttpClient::open(std::string const & host, int port, bool use_ssl)
{
    if (_conn) {
        return E_ALREADY;
    }

    char error_buffer[ERROR_MESSAGE_BUFFER_SIZE];
    memset(error_buffer, 0x00, sizeof(error_buffer));

    auto const conn = mg_connect_client(host.c_str(), port, use_ssl ? 1 : 0,
                                        error_buffer, ERROR_MESSAGE_BUFFER_SIZE);
    if (!conn) {
        error_buffer[ERROR_MESSAGE_BUFFER_SIZE-1] = '\0';
        return { E_OPEN, std::string(error_buffer) };
    }

    _conn = UniqueConnection(conn, &mg_close_connection);
    return E_SUCCESS;
}

void HttpClient::close()
{
    _conn.reset();
}

bool HttpClient::isOpen() const
{
    return static_cast<bool>(_conn);
}

Err HttpClient::write(std::string const & body) const
{
    auto const written_size = mg_write_string(_conn.get(), body);
    if (written_size == 0) {
        return E_CLOSED;
    } else if (written_size == -1) {
        return E_WRERR;
    }
    assert(written_size >= 1);
    if (body.size() == static_cast<std::size_t>(written_size)) {
        return E_SUCCESS;
    } else {
        return E_WARNING;
    }
}

ErrMsg HttpClient::wait(int timeout_ms)
{
    char error_buffer[ERROR_MESSAGE_BUFFER_SIZE];
    memset(error_buffer, 0x00, sizeof(error_buffer));
    auto const code = mg_get_response(_conn.get(), error_buffer, ERROR_MESSAGE_BUFFER_SIZE, timeout_ms);
    if (code < 0) {
        error_buffer[ERROR_MESSAGE_BUFFER_SIZE-1] = '\0';
        return { E_TIMEOUT, std::string(error_buffer) };
    }
    return E_SUCCESS;
}

bool HttpClient::getResponseInfo(mg_response_info * out) const
{
    auto const * info = mg_get_response_info(_conn.get());
    if (info == nullptr) {
        return false;
    }
    if (out) {
        *out = *info;
    }
    return true;
}

ErrString HttpClient::read() const
{
    mg_response_info info = {};
    if (!getResponseInfo(&info)) {
        return E_NREADY;
    }

    std::string buffer;
    buffer.resize(info.content_length);
    auto const read_size = mg_read(_conn.get(), &buffer[0], info.content_length);
    if (read_size == 0) {
        return E_ECONNREFUSED;
    } else if (read_size < 0) {
        return E_READ_ERROR;
    } else if (read_size < info.content_length) {
        buffer.resize(read_size);
    }
    return { E_SUCCESS, buffer };
}

} // namespace http

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

