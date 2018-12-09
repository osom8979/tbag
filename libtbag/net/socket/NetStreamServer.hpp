/**
 * @file   NetStreamServer.hpp
 * @brief  NetStreamServer class prototype.
 * @author zer0
 * @date   2018-12-09
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NET_SOCKET_NETSTREAMSERVER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NET_SOCKET_NETSTREAMSERVER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/mq/details/MqCommon.hpp>

#include <cstdint>
#include <string>
#include <memory>
#include <functional>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace net    {
namespace socket {

/**
 * NetStreamServer class prototype.
 *
 * @author zer0
 * @date   2018-12-09
 */
class TBAG_API NetStreamServer : private Noncopyable
{
public:
    struct Impl;
    friend struct Impl;

public:
    using UniqueImpl = std::unique_ptr<Impl>;

public:
    using MqEvent        = libtbag::mq::details::MqEvent;
    using MqType         = libtbag::mq::details::MqType;
    using MqRequestState = libtbag::mq::details::MqRequestState;
    using MqMachineState = libtbag::mq::details::MqMachineState;
    using MqMsg          = libtbag::mq::details::MqMsg;
    using MqMode         = libtbag::mq::details::MqMode;
    using MqParams       = libtbag::mq::details::MqParams;

private:
    UniqueImpl _impl;

public:
    NetStreamServer(MqParams const & params);
    NetStreamServer(std::string const & uri);
    NetStreamServer(NetStreamServer && obj) TBAG_NOEXCEPT;
    virtual ~NetStreamServer();

public:
    NetStreamServer & operator =(NetStreamServer && obj) TBAG_NOEXCEPT;

public:
    void swap(NetStreamServer & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(NetStreamServer & lh, NetStreamServer & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    inline bool exists() const TBAG_NOEXCEPT
    { return static_cast<bool>(_impl); }

    inline operator bool() const TBAG_NOEXCEPT
    { return exists(); }

public:
    static MqParams getParams(std::string const & uri);

protected:
    virtual void onBegin();
    virtual void onEnd();

protected:
    virtual bool onAccept(std::intptr_t id, std::string const & ip);
    virtual bool onRecv  (std::intptr_t id, char const * buffer, std::size_t size);
    virtual void onClose (std::intptr_t id);
};

/**
 * Functional StreamServer prototype.
 *
 * @author zer0
 * @date   2018-12-09
 */
struct NetFuncStreamServer : public NetStreamServer
{
    using OnBegin  = std::function<void()>;
    using OnEnd    = std::function<void()>;
    using OnAccept = std::function<bool(std::intptr_t, std::string const &)>;
    using OnRecv   = std::function<bool(std::intptr_t, char const *, std::size_t)>;
    using OnClose  = std::function<void(std::intptr_t)>;

    OnBegin    begin_cb;
    OnEnd        end_cb;
    OnAccept  accept_cb;
    OnRecv      recv_cb;
    OnClose    close_cb;

    template <typename ... Args>
    NetFuncStreamServer(Args && ... args) : NetStreamServer(std::forward<Args>(args) ...)
    { /* EMPTY. */ }

    virtual ~NetFuncStreamServer()
    { /* EMPTY. */ }

    virtual void onBegin() override
    {
        if (begin_cb) {
            begin_cb();
        } else {
            NetStreamServer::onBegin();
        }
    }

    virtual void onEnd() override
    {
        if (end_cb) {
            end_cb();
        } else {
            NetStreamServer::onEnd();
        }
    }

    virtual bool onAccept(std::intptr_t id, std::string const & ip) override
    {
        if (accept_cb) {
            return accept_cb(id, ip);
        } else {
            return NetStreamServer::onAccept(id, ip);
        }
    }

    virtual bool onRecv(std::intptr_t id, char const * buffer, std::size_t size) override
    {
        if (recv_cb) {
            return recv_cb(id, buffer, size);
        } else {
            return NetStreamServer::onRecv(id, buffer, size);
        }
    }

    virtual void onClose(std::intptr_t id) override
    {
        if (close_cb) {
            return close_cb(id);
        } else {
            return NetStreamServer::onClose(id);
        }
    }
};

} // namespace socket
} // namespace net

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NET_SOCKET_NETSTREAMSERVER_HPP__

