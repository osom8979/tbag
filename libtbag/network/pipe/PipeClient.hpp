/**
 * @file   PipeClient.hpp
 * @brief  PipeClient class prototype.
 * @author zer0
 * @date   2017-05-10
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_PIPE_PIPECLIENT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_PIPE_PIPECLIENT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/network/stream/StreamClient.hpp>
#include <libtbag/uvpp/Pipe.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace pipe    {

/**
 * PipeClient class prototype.
 *
 * @author zer0
 * @date   2017-05-09
 */
class TBAG_API PipeClient : public stream::StreamClient<uvpp::Pipe>
{
public:
    using Parent = stream::StreamClient<uvpp::Pipe>;

public:
    PipeClient(Loop & loop) : Parent(loop)
    {
        // EMPTY.
    }

    virtual ~PipeClient()
    {
        // EMPTY.
    }

public:
    virtual bool realInitialize(ClientBackend & backend, String const & destination, int port) override
    {
        if (filesystem::Path(destination).exists() == false) {
            return false;
        }

        backend.connect(backend.atConnectReq(), destination.c_str());
        return true;
    }
};

/**
 * FunctionalPipeClient typedef.
 *
 * @author zer0
 * @date   2017-05-09
 */
using FunctionalPipeClient = FunctionalClient<PipeClient>;

} // namespace pipe
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_PIPE_PIPECLIENT_HPP__

