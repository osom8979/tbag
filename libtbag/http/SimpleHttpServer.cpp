/**
 * @file   SimpleHttpServer.cpp
 * @brief  SimpleHttpServer class implementation.
 * @author zer0
 * @date   2019-01-14
 */

#include <libtbag/http/SimpleHttpServer.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace http {

SimpleHttpServer::SimpleHttpServer(std::string const & host, int port, Callbacks const & cbs)
        : CALLBACKS(cbs), HttpServer(getDefaultParams(host, port))
{
    // EMPTY.
}

SimpleHttpServer::~SimpleHttpServer()
{
    // EMPTY.
}

void SimpleHttpServer::onBegin()
{
    if (CALLBACKS.begin_cb) {
        CALLBACKS.begin_cb();
    } else {
        HttpServer::onBegin();
    }
};

void SimpleHttpServer::onEnd()
{
    if (CALLBACKS.end_cb) {
        CALLBACKS.end_cb();
    } else {
        HttpServer::onEnd();
    }
}

bool SimpleHttpServer::onAccept(std::intptr_t id, std::string const & ip)
{
    if (CALLBACKS.accept_cb) {
        return CALLBACKS.accept_cb(id, ip);
    } else {
        return HttpServer::onAccept(id, ip);
    }
}

void SimpleHttpServer::onClose(std::intptr_t id)
{
    if (CALLBACKS.close_cb) {
        CALLBACKS.close_cb(id);
    } else {
        HttpServer::onClose(id);
    }
}

HttpResponse SimpleHttpServer::onRegularHttp(std::intptr_t id, HttpRequest const & request)
{
    if (CALLBACKS.http_cb) {
        return CALLBACKS.http_cb(id, request);
    } else {
        return HttpServer::onRegularHttp(id, request);
    }
}

void SimpleHttpServer::onError(std::intptr_t id, Err code)
{
    if (CALLBACKS.error_cb) {
        CALLBACKS.error_cb(id, code);
    } else {
        HttpServer::onError(id, code);
    }
}

} // namespace http

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

