/**
 * @file   HttpClient.cpp
 * @brief  HttpClient class implementation.
 * @author zer0
 * @date   2020-03-10
 */

#include <libtbag/http/HttpClient.hpp>
#include <cstring>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace http {

HttpClient::HttpClient()
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
        return { std::string(error_buffer), E_OPEN };
    }

    _conn.reset(conn, [](typename decltype(_conn)::element_type * v){
        mg_close_connection(v);
    });
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

} // namespace http

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

