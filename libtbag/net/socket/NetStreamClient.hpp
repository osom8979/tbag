/**
 * @file   NetStreamClient.hpp
 * @brief  NetStreamClient class prototype.
 * @author zer0
 * @date   2018-12-09
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NET_SOCKET_NETSTREAMCLIENT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NET_SOCKET_NETSTREAMCLIENT_HPP__

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
 * NetStreamClient class prototype.
 *
 * @author zer0
 * @date   2018-12-09
 */
class TBAG_API NetStreamClient : private Noncopyable
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

    using OnBegin = std::function<void(void)>;
    using OnRecv  = std::function<void(char const *, std::size_t)>;
    using OnEnd   = std::function<void(void)>;

public:
    struct Callbacks
    {
        OnBegin begin_cb;
        OnRecv  recv_cb;
        OnEnd   end_cb;
    };

private:
    UniqueImpl _impl;

public:
    NetStreamClient(MqParams const & params);
    NetStreamClient(std::string const & uri);
    NetStreamClient(MqParams const & params, Callbacks const & cbs);
    NetStreamClient(std::string const & uri, Callbacks const & cbs);
    NetStreamClient(NetStreamClient && obj) TBAG_NOEXCEPT;
    virtual ~NetStreamClient();

public:
    NetStreamClient & operator =(NetStreamClient && obj) TBAG_NOEXCEPT;

public:
    void swap(NetStreamClient & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(NetStreamClient & lh, NetStreamClient & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    inline bool exists() const TBAG_NOEXCEPT
    { return static_cast<bool>(_impl); }

    inline operator bool() const TBAG_NOEXCEPT
    { return exists(); }

public:
    void join();

public:
    Err exit();

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
    Err sendClose(std::intptr_t id = 0);

public:
    static MqParams getParams(std::string const & uri);

protected:
    virtual void onBegin();
    virtual void onRecv(char const * buffer, std::size_t size);
    virtual void onEnd();
};

} // namespace socket
} // namespace net

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NET_SOCKET_NETSTREAMCLIENT_HPP__

