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
struct PipeClient : public stream::StreamClient
{
    PipeClient(Loop & loop) : stream::StreamClient(loop, StreamType::PIPE)
    { /* EMPTY. */ }
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

