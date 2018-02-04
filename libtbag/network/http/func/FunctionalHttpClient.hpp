/**
 * @file   FunctionalHttpClient.hpp
 * @brief  FunctionalHttpClient class prototype.
 * @author zer0
 * @date   2017-10-15
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_FUNC_FUNCTIONALHTTPCLIENT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_FUNC_FUNCTIONALHTTPCLIENT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Type.hpp>

#include <libtbag/network/http/HttpClient.hpp>
#include <libtbag/functional/CallbackHelper.hpp>

#include <functional>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {
namespace func    {

/**
 * FunctionalHttpClient class prototype.
 *
 * @author zer0
 * @date   2017-10-15
 */
template <typename HttpClientType>
struct FunctionalHttpClient : public HttpClientType
{
public:
    using Parent     = HttpClientType;
    using Loop       = HttpClient::Loop;
    using StreamType = HttpClient::StreamType;
    using ReadPacket = HttpClient::ReadPacket;

    STATIC_ASSERT_CHECK_IS_BASE_OF(HttpClient, Parent);

public:
    template <typename ... Args>
    FunctionalHttpClient(Args && ... args) : Parent(std::forward<Args>(args) ...)
    { /* EMPTY. */ }
    virtual ~FunctionalHttpClient()
    { /* EMPTY. */ }

public:
    using           onConnect_func = std::function<void(Err)>;
    using              onRead_func = std::function<void(Err, ReadPacket const &)>;
    using        onParseError_func = std::function<void(Err, void*)>;
    using onSwitchingProtocol_func = std::function<bool(HttpProperty const &, void*)>;
    using              onOpen_func = std::function<void(void)>;
    using               onEof_func = std::function<void(void)>;
    using             onError_func = std::function<void(EventType, Err)>;

private:
              onConnect_func           __onConnect_cb;
                 onRead_func              __onRead_cb;
           onParseError_func        __onParseError_cb;
    onSwitchingProtocol_func __onSwitchingProtocol_cb;
                 onOpen_func              __onOpen_cb;
                  onEof_func               __onEof_cb;
                onError_func             __onError_cb;

public:
    inline void           set_onConnect(          onConnect_func const & cb) {           __onConnect_cb = cb; }
    inline void              set_onRead(             onRead_func const & cb) {              __onRead_cb = cb; }
    inline void        set_onParseError(       onParseError_func const & cb) {        __onParseError_cb = cb; }
    inline void set_onSwitchingProtocol(onSwitchingProtocol_func const & cb) { __onSwitchingProtocol_cb = cb; }
    inline void              set_onOpen(             onOpen_func const & cb) {              __onOpen_cb = cb; }
    inline void               set_onEof(              onEof_func const & cb) {               __onEof_cb = cb; }
    inline void             set_onError(            onError_func const & cb) {             __onError_cb = cb; }

protected:
    virtual void onConnect(Err code) override
    {
        if (static_cast<bool>(__onConnect_cb)) {
            __onConnect_cb(code);
        } else {
            Parent::onConnect(code);
        }
    }

    virtual void onRead(Err code, ReadPacket const & packet) override
    {
        if (static_cast<bool>(__onRead_cb)) {
            __onRead_cb(code, packet);
        } else {
            Parent::onRead(code, packet);
        }
    }

    virtual void onParseError(Err code, void * arg) override
    {
        if (static_cast<bool>(__onParseError_cb)) {
            __onParseError_cb(code, arg);
        } else {
            Parent::onParseError(code, arg);
        }
    }

    virtual bool onSwitchingProtocol(HttpProperty const & property, void * arg) override
    {
        if (static_cast<bool>(__onSwitchingProtocol_cb)) {
            return __onSwitchingProtocol_cb(property, arg);
        } else {
            return Parent::onSwitchingProtocol(property, arg);
        }
    }

    virtual void onOpen() override
    {
        if (static_cast<bool>(__onOpen_cb)) {
            __onOpen_cb();
        } else {
            Parent::onOpen();
        }
    }

    virtual void onEof() override
    {
        if (static_cast<bool>(__onEof_cb)) {
            __onEof_cb();
        } else {
            Parent::onEof();
        }
    }

    virtual void onError(EventType from, Err code) override
    {
        if (static_cast<bool>(__onError_cb)) {
            __onError_cb(from, code);
        } else {
            Parent::onError(from, code);
        }
    }

public:
    // @formatter:off
    TBAG_VOID_CALLBACK_HELPER(onWrite   , Err);
    TBAG_VOID_CALLBACK_HELPER(onShutdown, Err);
    TBAG_VOID_CALLBACK_HELPER(onClose);
    TBAG_VOID_CALLBACK_HELPER(onTimer);
    // @formatter:on

public:
    // @formatter:off
    TBAG_VOID_CALLBACK_HELPER(onContinue   , void*);
    TBAG_VOID_CALLBACK_HELPER(onWsMessage  , ws::WsOpCode, util::Buffer const &, void*);
    TBAG_VOID_CALLBACK_HELPER(onRegularHttp, HttpProperty const &, void*);
    // @formatter:on
};

using FuncHttpClient = FunctionalHttpClient<HttpClient>;

} // namespace func
} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_FUNC_FUNCTIONALHTTPCLIENT_HPP__

