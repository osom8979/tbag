/**
 * @file   PipeServer.hpp
 * @brief  PipeServer class prototype.
 * @author zer0
 * @date   2017-05-09
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_PIPESERVER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_PIPESERVER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
// Include preprocessor.

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace details {

// Forward declaration.

/**
 * PipeServer class prototype.
 *
 * @author zer0
 * @date   2017-05-09
 */
class TBAG_API PipeServer : public Noncopyable
{
private:
    // Insert member variables.

public:
    PipeServer();
    virtual ~PipeServer();
};

} // namespace details
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_PIPESERVER_HPP__
