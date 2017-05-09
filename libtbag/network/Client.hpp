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
#include <libtbag/network/details/NetCommon.hpp>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {

/**
 * Client class prototype.
 *
 * @author zer0
 * @date   2017-05-02
 */
struct Client : public details::NetCommon
{
    // @formatter:off
    virtual Type getType() const
    { return Type::UNKNOWN; }
    virtual Id getId() const
    { return id::UNKNOWN_ID; }

    virtual bool init(String const & destination, int port = 0, uint64_t millisec = 0)
    { return false; }

    virtual bool  start() { return false; }
    virtual bool   stop() { return false; }
    virtual bool  close() { return false; }
    virtual bool cancel() { return false; }

    virtual bool write(binf const * buffer, Size size, uint64_t millisec = 0) { return false; }
    virtual bool write(char const * buffer, Size size, uint64_t millisec = 0) { return false; }

    virtual void onConnect(uerr code) { /* EMPTY. */ }
    virtual void onWrite  (uerr code) { /* EMPTY. */ }
    virtual void onRead   (uerr code, char const * buffer, Size size) { /* EMPTY. */ }
    virtual void onClose  ()          { /* EMPTY. */ }
    // @formatter:on
};

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_CLIENT_HPP__

