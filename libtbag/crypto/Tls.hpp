/**
 * @file   Tls.hpp
 * @brief  Tls class prototype.
 * @author zer0
 * @date   2017-11-11
 * @date   2018-12-25 (Change namespace: libtbag::network::http::tls -> libtbag::http)
 * @date   2019-01-13 (Change namespace: libtbag::http -> libtbag::crypto)
 * @date   2019-01-20 (Rename: TlsReader -> Tls)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CRYPTO_TLSREADER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CRYPTO_TLSREADER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/crypto/X509.hpp>
#include <libtbag/uvpp/UvCommon.hpp>

#include <memory>
#include <vector>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace crypto {

/**
 * Tls class prototype.
 *
 * @author zer0
 * @date   2017-11-11
 * @date   2018-12-25 (Change namespace: libtbag::network::http::tls -> libtbag::http)
 * @date   2019-01-13 (Change namespace: libtbag::http -> libtbag::crypto)
 * @date   2019-01-20 (Rename: TlsReader -> Tls)
 */
class TBAG_API Tls : private Noncopyable
{
public:
    struct Impl;
    friend struct Impl;

public:
    using UniqueImpl = std::unique_ptr<Impl>;

public:
    struct reference_ssl_context { /* EMPTY. */ };
    struct create_memory_cert { /* EMPTY. */ };

private:
    UniqueImpl _impl;

public:
    /**
     * Client-side constructor.
     */
    Tls();

    /**
     * Server-side constructor.
     *
     * @param[in] cert_pem
     *      Certificated PEM text.
     * @param[in] key_pem
     *      Private key PEM text.
     */
    Tls(std::string const & cert_pem, std::string const & key_pem);
    Tls(X509Pem const & x509);

    /**
     * Server-side constructor that creates an in-memory certificate.
     */
    Tls(create_memory_cert);

    /**
     * Server-side node constructor.
     *
     * @param[in] tls
     *      Original Tls context.
     */
    Tls(reference_ssl_context, Tls const & tls);

    ~Tls();

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
    int pendingOfEncodeBufferSize() const;
    int pendingOfDecodeBufferSize() const;

public:
    std::vector<char> encode(void const * data, std::size_t size, Err * code);
    std::vector<char> decode(void const * data, std::size_t size, Err * code);

    /** Read & decode from pending data. */
    std::vector<char> decode(Err * code);
};

enum class TlsState
{
    TS_NOT_READY,
    TS_HANDSHAKE,
    TS_FINISH,

    TS_EXCHANGE_KEY, // TODO: REMOVE THIS!
};

inline char const * const getTlsStateNmae(TlsState state) TBAG_NOEXCEPT
{
    switch (state) {
    case TlsState::TS_NOT_READY:    return "NOT_READY";
    case TlsState::TS_HANDSHAKE:    return "TS_HANDSHAKE";
    case TlsState::TS_FINISH:       return "FINISH";
    default:                        return "UNKNOWN";
    }
}

struct TlsReaderInterface
{
    virtual Err onWrite(char const * buffer, std::size_t size) = 0;
    virtual Err onRead(char const * buffer, std::size_t size) = 0;

    TlsReaderInterface() { /* EMPTY. */ }
    virtual ~TlsReaderInterface() { /* EMPTY. */ }
};

/**
 * TlsReader class prototype.
 *
 * @author zer0
 * @date   2019-01-26
 */
class TBAG_API TlsReader : private Noncopyable
{
public:
    using ref_ssl = Tls::reference_ssl_context;
    using mem_crt = Tls::create_memory_cert;
    using Buffer = libtbag::util::Buffer;

private:
    Tls      _tls;
    TlsState _state;

private:
    TlsReaderInterface * _callback = nullptr;

public:
    TlsReader(TlsReaderInterface * cb);
    TlsReader(TlsReaderInterface * cb, std::string const & cert_pem, std::string const & key_pem);
    TlsReader(TlsReaderInterface * cb, mem_crt const & UNUSED_PARAM(init));
    TlsReader(TlsReaderInterface * cb, ref_ssl const & UNUSED_PARAM(init), Tls const & tls);
    ~TlsReader();

public:
    inline char const * const getStateNmae() const TBAG_NOEXCEPT
    { return getTlsStateNmae(_state); }

    inline bool isFinished() const TBAG_NOEXCEPT
    { return _state == TlsState::TS_FINISH; }

public:
    Err connect();
    Err accept();

public:
    Err parse(char const * buffer, std::size_t size);

protected:
    Err onHandshaking(char const * buffer, std::size_t size);
    Err onApplication(char const * buffer, std::size_t size);
};

} // namespace crypto

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CRYPTO_TLSREADER_HPP__

