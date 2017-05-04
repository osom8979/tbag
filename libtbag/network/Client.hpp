/**
 * @file   Client.hpp
 * @brief  Client class prototype.
 * @author zer0
 * @date   2017-05-02
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_CLIENT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_CLIENT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/network/details/NetCommon.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

// Forward declaration.
namespace uvpp { class Loop; }

namespace network {

/**
 * Client class prototype.
 *
 * @author zer0
 * @date   2017-05-02
 */
class TBAG_API Client : public details::ClientInterface, public Noncopyable
{
public:
    using Loop    = uvpp::Loop;
    using NetType = details::NetType;

    using SharedClient = std::shared_ptr<Client>;
    using WeakClient   = std::weak_ptr<Client>;

protected:
    Client(Loop & loop);
public:
    virtual ~Client();

public:
    static SharedClient create(Loop & loop, NetType type);
};

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_CLIENT_HPP__

