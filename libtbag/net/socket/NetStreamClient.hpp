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

private:
    UniqueImpl _impl;

public:
    NetStreamClient(MqParams const & params);
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

protected:
    virtual void onBegin();
    virtual bool onRecv(char const * buffer, std::size_t size);
    virtual void onEnd();
};

} // namespace socket
} // namespace net

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NET_SOCKET_NETSTREAMCLIENT_HPP__

