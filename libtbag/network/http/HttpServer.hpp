/**
 * @file   HttpServer.hpp
 * @brief  HttpServer class prototype.
 * @author zer0
 * @date   2017-05-22
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPSERVER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPSERVER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/Type.hpp>
#include <libtbag/log/Log.hpp>

#include <libtbag/network/details/NetCommon.hpp>
#include <libtbag/network/stream/StreamServer.hpp>
#include <libtbag/network/http/HttpParser.hpp>
#include <libtbag/network/http/HttpBuilder.hpp>
#include <libtbag/network/Uri.hpp>
#include <libtbag/uvpp/Loop.hpp>

#include <cassert>
#include <functional>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

/**
 * HttpServer class prototype.
 *
 * @author zer0
 * @date   2017-05-22
 */
template <typename ServerType>
class HttpServer : public ServerType
{
public:
    using Parent = ServerType;

    using ClientInterface = details::ClientInterface;
    using ServerInterface = details::ServerInterface;

public:
    using Loop    = uvpp::Loop;
    using String  = HttpParser::String;
    using Size    = HttpParser::Size;
    using Buffer  = std::vector<char>;

    using SharedClient = ServerInterface::SharedClient;
    using   WeakClient = ServerInterface::WeakClient;

    using SystemClock = std::chrono::system_clock;
    using TimePoint   = SystemClock::time_point;
    using Millisec    = std::chrono::milliseconds;
    using Seconds     = std::chrono::seconds;

    using OnOpen    = std::function<void(WeakClient)>;
    using OnRequest = std::function<void(Err, HttpParser const &, HttpBuilder &, uint64_t &)>;
    using OnClose   = std::function<void(WeakClient)>;

public:
    struct ClientData
    {
        Buffer      buffer;
        HttpBuilder builder;
        HttpParser  parser;
        Millisec    timeout;
        TimePoint   start_time;
    };

private:
    OnOpen    _open_cb;
    OnRequest _request_cb;
    OnOpen    _close_cb;

public:
    HttpServer(Loop & loop) : Parent(loop) { /* EMPTY. */ }
    ~HttpServer() { /* EMPTY. */ }

public:
    // @formatter:off
    inline void setOnOpen   (OnOpen    const & cb) { _open_cb    = cb; }
    inline void setOnRequest(OnRequest const & cb) { _request_cb = cb; }
    inline void setOnClose  (OnClose   const & cb) { _close_cb   = cb; }
    // @formatter:on

// Event callback.
public:
    virtual void onConnection(Err code) override
    {
        if (code != Err::E_SUCCESS) {
            tDLogE("HttpServer::onConnection() {} error.", getErrName(code));
            return;
        }

        auto node = this->accept();
        if (auto shared = node.lock()) {
            if (shared->start()) {
                if (static_cast<bool>(_open_cb)) {
                    _open_cb(node);
                }
            } else {
                tDLogE("HttpServer::onConnection() Start client error.");
            }
        } else {
            tDLogE("HttpServer::onConnection() Expired client.");
        }
    }

    virtual void onClientRead(WeakClient node, Err code, char const * buffer, Size size) override
    {
        //this->write();
        //virtual bool write(binf const * buffer, Size size, uint64_t millisec = 0) = 0;
        //virtual bool write(char const * buffer, Size size, uint64_t millisec = 0) = 0;

        auto shared = node.lock();
        if (static_cast<bool>(shared) == false) {
            tDLogE("HttpServer::onClientRead() Expired client.");
            return;
        }

        tDLogI("HttpServer::onClientRead() step 01.");

        ClientData * client_data = static_cast<ClientData*>(shared->getUserData());
        if (client_data == nullptr) {
            tDLogE("HttpServer::onClientRead() User data is nullptr.");
            return;
        }

        tDLogI("HttpServer::onClientRead() step 02.");

        if (code != Err::E_SUCCESS /* with Err::E_EOF*/) {
            shared->stop();
            shared->close();
            return;
        }

        tDLogI("HttpServer::onClientRead() step 03.");

        HttpParser  & request  = client_data->parser;
        HttpBuilder & response = client_data->builder;

        request.execute(buffer, size);
        if (request.isComplete()) {

            tDLogI("HttpServer::onClientRead() step 04.");
            if (static_cast<bool>(_request_cb)) {
                uint64_t timeout = 25000U;
                _request_cb(code, request, response, timeout);

                auto buffer = response.responseDefault();
                if (shared->write(buffer.data(), buffer.size(), timeout) == false) {
                    tDLogW("HttpServer::onClientRead() Write error.");
                }
            }
        }
    }

    virtual void onClientClose(WeakClient node) override
    {
        if (auto shared = node.lock()) {
            //shared->stop();
            if (static_cast<bool>(_close_cb)) {
                _close_cb(node);
            }
        } else {
            tDLogE("HttpServer::onClientClose() Expired client.");
        }
    }

// Empty event methods.
public:
    virtual void onClientShutdown(WeakClient node, Err code) override { /* EMPTY. */ }
    virtual void onClientWrite(WeakClient node, Err code) override { /* EMPTY. */ }
    virtual void onServerClose() override { /* EMPTY. */ }

public:
    virtual void * onClientUserDataAlloc(WeakClient node) override
    {
        tDLogI("HttpServer::onClientUserDataAlloc() Create user data.");
        auto * client_data = new (std::nothrow) ClientData;
        assert(client_data != nullptr);
        return client_data;
    }

    virtual void onClientUserDataDealloc(WeakClient node, void * data) override
    {
        auto * client_data = static_cast<ClientData*>(data);
        assert(client_data != nullptr);
        delete client_data;
    }
};

using TcpHttpServer  = HttpServer<stream::TcpServer>;
using PipeHttpServer = HttpServer<stream::PipeServer>;

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPSERVER_HPP__

