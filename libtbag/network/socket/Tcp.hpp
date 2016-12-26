/**
 * @file   Tcp.hpp
 * @brief  Tcp class prototype.
 * @author zer0
 * @date   2016-11-04
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SOCKET_TCP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SOCKET_TCP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/util/UvUtils.hpp>

#include <string>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

// Forward declaration.
namespace loop { class UvEventLoop; }

namespace network {
namespace socket  {

/**
 * Tcp class prototype.
 *
 * @author zer0
 * @date   2016-11-04
 * @date   2016-11-07 (Refactoring this class)
 */
class TBAG_API Tcp : public util::UvHandle
{
public:
    Tcp();
    ~Tcp();

public:
    bool init(libtbag::loop::UvEventLoop & loop);

public:
    std::string getPeerName() const;
    std::string getSocketName() const;

public:
    static bool isIpv4(std::string const & address);
    static bool isIpv6(std::string const & address);
};

} // namespace socket
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SOCKET_TCP_HPP__

