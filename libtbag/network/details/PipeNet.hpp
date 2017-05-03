/**
 * @file   PipeNet.hpp
 * @brief  PipeNet class prototype.
 * @author zer0
 * @date   2017-05-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_PIPENET_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_PIPENET_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/network/Client.hpp>
#include <libtbag/network/Server.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace details {

/**
 * PipeNet client class prototype.
 *
 * @author zer0
 * @date   2017-05-03
 */
class TBAG_API PipeNetClient : public Client
{
private:
    // Insert member variables.

public:
    PipeNetClient(Loop & loop);
    virtual ~PipeNetClient();

public:
    virtual Type getType() const override;
};

/**
 * PipeNet server class prototype.
 *
 * @author zer0
 * @date   2017-05-03
 */
class TBAG_API PipeNetServer : public Server
{
private:
    // Insert member variables.

public:
    PipeNetServer(Loop & loop);
    virtual ~PipeNetServer();

public:
    virtual Type getType() const override;
};

} // namespace details
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_PIPENET_HPP__

