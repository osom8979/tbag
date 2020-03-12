/**
 * @file   HttpServer.cpp
 * @brief  HttpServer class implementation.
 * @author zer0
 * @date   2020-03-10
 */

#include <libtbag/http/HttpServer.hpp>
#include <libtbag/http/CivetWebExtension.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cassert>

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

// ----------
// HttpServer
// ----------

void HttpServer::__close_cb(CivetServer * s)
{
    assert(s != nullptr);
    s->close();
}

void HttpServer::opt(Options & o, char const * key, bool arg)
{
    o.emplace_back(key);
    o.emplace_back(arg ? "yes" : "no");
}

void HttpServer::opt(Options & o, char const * key, int arg)
{
    o.emplace_back(key);
    o.emplace_back(libtbag::string::toString(arg));
}

void HttpServer::opt(Options & o, char const * key, std::string const & arg)
{
    o.emplace_back(key);
    o.emplace_back(arg);
}

void HttpServer::opt(Options & o, char const * key, std::vector<std::string> const & args)
{
    o.emplace_back(key);
    o.emplace_back(libtbag::string::mergeTokens(args, ","));
}

HttpServer::HttpServer() : CALLBACKS(__create_default_callbacks()),
                           _server(nullptr, &HttpServer::__close_cb)
{
    // EMPTY.
}

HttpServer::~HttpServer()
{
    // EMPTY.
}

bool HttpServer::req(RequestMap & r, std::string const & path, CivetWebEventFunctional const & f)
{
    std::shared_ptr<CivetWebEventFunctional> handler;
    try {
        handler = std::make_shared<CivetWebEventFunctional>(f);
    } catch (...) {
        return false;
    }
    return r.emplace(path, std::static_pointer_cast<CivetHandler>(handler)).second;
}

ErrMsg HttpServer::open(Options const & options, RequestMap const & requests, void * user)
{
    if (_server) {
        return E_ALREADY;
    }

    try {
        auto * server = new CivetServer(options, &CALLBACKS, user);
        _server = UniqueCivetServer(server, &HttpServer::__close_cb);
    } catch (std::exception const & e) {
        return { std::string(e.what()), E_OPEN };
    } catch (...) {
        return E_UNKEXCP;
    }

    _requests = requests;
    for (auto & req : _requests) {
        _server->addHandler(req.first, req.second.get());
    }
    return E_SUCCESS;
}

void HttpServer::close()
{
    _server.reset();
}

bool HttpServer::isOpen() const
{
    return static_cast<bool>(_server);
}

Err HttpServer::write(mg_connection * conn, std::string const & body)
{
    auto const written_size = mg_write_string(conn, body);
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

} // namespace http

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

