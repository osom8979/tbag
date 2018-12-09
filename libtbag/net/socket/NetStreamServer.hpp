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

protected:
    virtual void onBegin();
    virtual void onEnd();

protected:
    virtual bool onAccept(std::intptr_t id, std::string const & ip);
    virtual bool onRecv  (std::intptr_t id, char const * buffer, std::size_t size);
    virtual void onClose (std::intptr_t id);
};

} // namespace socket
} // namespace net

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NET_SOCKET_NETSTREAMSERVER_HPP__

