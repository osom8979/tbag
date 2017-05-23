/**
 * @file   PipeServer.hpp
 * @brief  PipeServer class prototype.
 * @author zer0
 * @date   2017-05-10
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_PIPE_PIPESERVER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_PIPE_PIPESERVER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/network/stream/StreamServer.hpp>
#include <libtbag/network/pipe/PipeClient.hpp>
#include <libtbag/uvpp/Pipe.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace pipe    {

/**
 * PipeServer class prototype.
 *
 * @author zer0
 * @date   2017-05-09
 */
struct PipeServer : public stream::StreamServer
{
    PipeServer(Loop & loop) : stream::StreamServer(loop, StreamType::PIPE)
    { /* EMPTY. */ }
};

/**
 * FunctionalPipeServer typedef.
 *
 * @author zer0
 * @date   2017-05-09
 */
using FunctionalPipeServer = FunctionalServer<PipeServer>;

} // namespace pipe
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_PIPE_PIPESERVER_HPP__

