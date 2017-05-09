/**
 * @file   PipeClient.hpp
 * @brief  PipeClient class prototype.
 * @author zer0
 * @date   2017-05-09
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_PIPECLIENT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_PIPECLIENT_HPP__

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
 * PipeClient class prototype.
 *
 * @author zer0
 * @date   2017-05-09
 */
class TBAG_API PipeClient : public Noncopyable
{
private:
    // Insert member variables.

public:
    PipeClient();
    virtual ~PipeClient();
};

} // namespace details
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_PIPECLIENT_HPP__

