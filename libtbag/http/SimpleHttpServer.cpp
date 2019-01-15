/**
 * @file   SimpleHttpServer.cpp
 * @brief  SimpleHttpServer class implementation.
 * @author zer0
 * @date   2019-01-14
 */

#include <libtbag/http/SimpleHttpServer.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace http {

// -------------------------------
// SimpleHttpServer implementation
// -------------------------------

SimpleHttpServer::SimpleHttpServer(std::string const & host, int port, Callbacks const & cbs)
        : HttpServer(initAndParams(host, port, cbs))
        /* ---------------------------------------------------------------------- *
         * Don't initialize <code>_callbacks</code> member.                       *
         * This member is initialized in the <code>initAndParams()</code> method. *
         * ---------------------------------------------------------------------- */
{
    // EMPTY.
}

SimpleHttpServer::~SimpleHttpServer()
{
    // EMPTY.
}

SimpleHttpServer::MqParams SimpleHttpServer::initAndParams(std::string const & host, int port, Callbacks const & cbs)
{
   _callbacks = cbs;
    return HttpServer::getDefaultParams(host, port);
}

void SimpleHttpServer::onBegin()
{
    if (_callbacks.begin_cb) {
        _callbacks.begin_cb();
    } else {
        HttpServer::onBegin();
    }
};

void SimpleHttpServer::onEnd()
{
    if (_callbacks.end_cb) {
        _callbacks.end_cb();
    } else {
        HttpServer::onEnd();
    }
}

bool SimpleHttpServer::onAccept(std::intptr_t id, std::string const & ip)
{
    if (_callbacks.accept_cb) {
        return _callbacks.accept_cb(id, ip);
    } else {
        return HttpServer::onAccept(id, ip);
    }
}

void SimpleHttpServer::onClose(std::intptr_t id)
{
    if (_callbacks.close_cb) {
        _callbacks.close_cb(id);
    } else {
        HttpServer::onClose(id);
    }
}

HttpResponse SimpleHttpServer::onRegularHttp(std::intptr_t id, HttpRequest const & request)
{
    if (_callbacks.http_cb) {
        return _callbacks.http_cb(id, request);
    } else {
        return HttpServer::onRegularHttp(id, request);
    }
}

void SimpleHttpServer::onError(std::intptr_t id, Err code)
{
    if (_callbacks.error_cb) {
        _callbacks.error_cb(id, code);
    } else {
        HttpServer::onError(id, code);
    }
}

} // namespace http

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

