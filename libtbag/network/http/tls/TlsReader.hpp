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
#include <libtbag/Err.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/uvpp/UvCommon.hpp>

#include <memory>
#include <vector>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {
namespace tls     {

/**
 * TlsReader class prototype.
 *
 * @author zer0
 * @date   2017-11-11
 */
class TBAG_API TlsReader
{
public:
    struct Impl;
    friend struct Impl;

public:
    using UniqueImpl = std::unique_ptr<Impl>;

private:
    UniqueImpl _impl;

public:
    TlsReader();
    virtual ~TlsReader();

public:
    bool isFinished() const;

public:
    std::string getCipherName() const;

public:
    void accept();
    void connect();

public:
    Err handshake();
    Err readFromWriteBuffer(std::vector<char> & result);
    Err writeToReadBuffer(void const * data, std::size_t size, std::size_t * write_size = nullptr);

public:
    std::vector<char> encode(void const * data, std::size_t size, Err * code);
    std::vector<char> decode(void const * data, std::size_t size, Err * code);
};

} // namespace tls
} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_TLS_TLSREADER_HPP__

