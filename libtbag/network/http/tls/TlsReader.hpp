/**
 * @file   TlsReader.hpp
 * @brief  TlsReader class prototype.
 * @author zer0
 * @date   2017-11-11
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_TLS_TLSREADER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_TLS_TLSREADER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {
namespace tls     {

/**
 * TlsReader interface.
 *
 * @author zer0
 * @date   2017-11-11
 */
struct TlsReaderInterface
{
};

/**
 * TlsReader class prototype.
 *
 * @author zer0
 * @date   2017-11-11
 */
class TBAG_API TlsReader : public TlsReaderInterface
{
public:
    TlsReader();
    virtual ~TlsReader();

public:
};

} // namespace tls
} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_TLS_TLSREADER_HPP__

