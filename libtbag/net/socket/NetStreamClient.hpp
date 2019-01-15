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
#include <libtbag/mq/MqNode.hpp>

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
 * @date   2019-01-15 (Merge with NetStreamClient::Impl class)
 */
class TBAG_API NetStreamClient TBAG_FINAL : public libtbag::mq::MqNode
{
public:
    using OnBegin = std::function<void(void)>;
    using OnEnd   = std::function<void(void)>;
    using OnRecv  = std::function<void(char const *, std::size_t)>;

    struct Callbacks
    {
        OnBegin begin_cb;
        OnEnd   end_cb;
        OnRecv  recv_cb;
    };

private:
    Callbacks _callbacks;

public:
    NetStreamClient(MqParams const & params, Callbacks const & cbs);
    NetStreamClient(std::string const & uri, Callbacks const & cbs);
    ~NetStreamClient();

private:
    using size = std::size_t;
    friend void onConnectCb(void * parent);
    friend void onCloseCb(void * parent);
    friend size onWriteCb(void * node, MqEvent event, char const * buffer, std::size_t size, void * parent);
    friend void onReadCb(void * node, char const * buffer, std::size_t size, void * parent);
};

} // namespace socket
} // namespace net

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NET_SOCKET_NETSTREAMCLIENT_HPP__

