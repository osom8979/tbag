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

    using OnBegin  = std::function<void(void)>;
    using OnEnd    = std::function<void(void)>;
    using OnAccept = std::function<bool(std::intptr_t, std::string const &)>;
    using OnRecv   = std::function<void(std::intptr_t, char const *, std::size_t)>;
    using OnClose  = std::function<void(std::intptr_t)>;

public:
    struct Callbacks
    {
        OnBegin   begin_cb;
        OnEnd     end_cb;
        OnAccept  accept_cb;
        OnRecv    recv_cb;
        OnClose   close_cb;
    };

private:
    UniqueImpl _impl;

public:
    NetStreamServer(MqParams const & params);
    NetStreamServer(std::string const & uri);
    NetStreamServer(MqParams const & params, Callbacks const & cbs);
    NetStreamServer(std::string const & uri, Callbacks const & cbs);
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
    void join();

public:
    Err send(MqMsg const & msg);

public:
    Err send(char const * buffer, std::size_t size);
    Err send(MqEvent event, char const * buffer, std::size_t size);

public:
    Err send(std::string const & text);
    Err send(MqEvent event, std::string const & text);

public:
    Err send(MqMsg::Buffer const & buffer);
    Err send(MqEvent event, MqMsg::Buffer const & buffer);

public:
    Err sendCloseMsg();

public:
    static MqParams getParams(std::string const & uri);

protected:
    virtual void onBegin();
    virtual void onEnd();

protected:
    virtual bool onAccept(std::intptr_t id, std::string const & ip);
    virtual void onRecv  (std::intptr_t id, char const * buffer, std::size_t size);
    virtual void onClose (std::intptr_t id);
};

} // namespace socket
} // namespace net

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NET_SOCKET_NETSTREAMSERVER_HPP__
