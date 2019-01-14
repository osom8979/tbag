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
#include <libtbag/uvpp/Loop.hpp>

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
class TBAG_API NetStreamServer TBAG_FINAL : private Noncopyable
{
public:
    struct Impl;
    friend struct Impl;

public:
    using UniqueImpl     = std::unique_ptr<Impl>;
    using Loop           = libtbag::uvpp::Loop;
    using MqEvent        = libtbag::mq::details::MqEvent;
    using MqType         = libtbag::mq::details::MqType;
    using MqRequestState = libtbag::mq::details::MqRequestState;
    using MqMachineState = libtbag::mq::details::MqMachineState;
    using MqMsg          = libtbag::mq::details::MqMsg;
    using MqMode         = libtbag::mq::details::MqMode;
    using MqParams       = libtbag::mq::details::MqParams;

public:
    struct Callbacks
    {
        using OnBegin  = std::function<void(void)>;
        using OnEnd    = std::function<void(void)>;
        using OnAccept = std::function<bool(std::intptr_t, std::string const &)>;
        using OnRecv   = std::function<void(std::intptr_t, char const *, std::size_t)>;
        using OnClose  = std::function<void(std::intptr_t)>;

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
    ~NetStreamServer();

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
    Loop & loop();
    Loop const & loop() const;

public:
    void join();

public:
    Err exit();

public:
    Err send(MqMsg const & msg);

    Err send(char const * buffer, std::size_t size, std::intptr_t id = 0);
    Err send(MqEvent event, char const * buffer, std::size_t size, std::intptr_t id = 0);

    Err send(std::string const & text, std::intptr_t id = 0);
    Err send(MqEvent event, std::string const & text, std::intptr_t id = 0);

    Err send(MqMsg::Buffer const & buffer, std::intptr_t id = 0);
    Err send(MqEvent event, MqMsg::Buffer const & buffer, std::intptr_t id = 0);

    Err sendClose(std::intptr_t id = 0);

public:
    static MqParams getParams(std::string const & uri);
};

} // namespace socket
} // namespace net

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NET_SOCKET_NETSTREAMSERVER_HPP__

