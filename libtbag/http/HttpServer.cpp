/**
 * @file   HttpServer.cpp
 * @brief  HttpServer class implementation.
 * @author zer0
 * @date   2020-03-10
 */

#include <libtbag/http/HttpServer.hpp>
#include <libtbag/http/CivetWebExtension.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace http {

static int __begin_request(mg_connection * conn)
{
    return 0;
}

static void __end_request(mg_connection const * conn, int reply_status_code)
{
    // EMPTY.
}

static int __log_message(mg_connection const * conn, char const * message)
{
    return 0;
}

static int __log_access(mg_connection const * conn, char const * message)
{
    return 0;
}

static int __init_ssl(void * ssl_context, void * user_data)
{
    return 0;
}

static int __external_ssl_ctx(void ** ssl_ctx, void * user_data)
{
    return 0;
}

static void __connection_close(mg_connection const * conn)
{
    // EMPTY.
}

static void __init_lua(mg_connection const * conn, void * lua_context)
{
    // EMPTY.
}

static int __http_error(mg_connection * conn, int status, char const * errmsg)
{
    return 0;
}

static void __init_context(mg_context const * ctx)
{
    // EMPTY.
}

static void __exit_context(mg_context const * ctx)
{
    // EMPTY.
}

static void * __init_thread(mg_context const * ctx, int thread_type)
{
    return nullptr;
}

static void __exit_thread(mg_context const * ctx, int thread_type, void * thread_pointer)
{
    // EMPTY.
}

static int __init_connection(mg_connection const * conn, void ** conn_data)
{
    return 0;
}

static CivetCallbacks __create_default_callbacks()
{
    // clang-format off
    CivetCallbacks result;
    result.begin_request    = &__begin_request;
    result.end_request      = &__end_request;
    result.log_message      = &__log_message;
    result.log_access       = &__log_access;
    result.init_ssl         = &__init_ssl;
    result.external_ssl_ctx = &__external_ssl_ctx;
    result.connection_close = &__connection_close;
    result.init_lua         = &__init_lua;
    result.http_error       = &__http_error;
    result.init_context     = &__init_context;
    result.exit_context     = &__exit_context;
    result.init_thread      = &__init_thread;
    result.exit_thread      = &__exit_thread;
    result.init_connection  = &__init_connection;
    return result;
    // clang-format on
}

HttpServer::HttpServer()
{
    // EMPTY.
}

HttpServer::~HttpServer()
{
    // EMPTY.
}

} // namespace http

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

