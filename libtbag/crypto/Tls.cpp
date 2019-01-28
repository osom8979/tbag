/**
 * @file   Tls.cpp
 * @brief  Tls class implementation.
 * @author zer0
 * @date   2017-11-11
 * @date   2018-12-25 (Change namespace: libtbag::network::http::tls -> libtbag::http)
 * @date   2019-01-13 (Change namespace: libtbag::http -> libtbag::crypto)
 * @date   2019-01-20 (Rename: TlsReader -> Tls)
 */

#include <libtbag/crypto/Tls.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/filesystem/File.hpp>

#include <openssl/err.h>
#include <openssl/ssl.h>
#include <openssl/conf.h>
#include <openssl/engine.h>

#include <cassert>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace crypto {

#if !defined(SSLerror)
#define SSLerror(r)  ERR_PUT_error(ERR_LIB_SSL,(0xfff),(r),__FILE__,__LINE__)
#endif

static int SSL_CTX_use_PrivateKey_memory(SSL_CTX * ctx, char const * buffer, int len, int type)
{
    int j, ret = 0;
    EVP_PKEY * pkey = nullptr;
    BIO * in = nullptr;

    in = BIO_new(BIO_s_mem());
    if (in == nullptr) {
        SSLerror(ERR_R_BUF_LIB);
        goto end;
    }

    if (BIO_write(in, (void*)buffer, len) <= 0) {
        SSLerror(ERR_R_SYS_LIB);
        goto end;
    }

    if (type == SSL_FILETYPE_PEM) {
        j = ERR_R_PEM_LIB;
        pkey = PEM_read_bio_PrivateKey(in, nullptr,
                                       ctx->default_passwd_callback,
                                       ctx->default_passwd_callback_userdata);
    } else if (type == SSL_FILETYPE_ASN1) {
        j = ERR_R_ASN1_LIB;
        pkey = d2i_PrivateKey_bio(in, nullptr);
    } else {
        SSLerror(SSL_R_BAD_SSL_FILETYPE);
        goto end;
    }

    if (pkey == nullptr) {
        SSLerror(j);
        goto end;
    }
    ret = SSL_CTX_use_PrivateKey(ctx, pkey);
    EVP_PKEY_free(pkey);

end:
    BIO_free(in);
    return (ret);
}

static int SSL_CTX_use_certificate_memory(SSL_CTX * ctx, char const * buffer, int len, int type)
{
    int j, ret = 0;
    X509 * x = nullptr;
    BIO * in = nullptr;

    in = BIO_new(BIO_s_mem());
    if (in == nullptr) {
        SSLerror(ERR_R_BUF_LIB);
        goto end;
    }

    if (BIO_write(in, (void*)buffer, len) <= 0) {
        SSLerror(ERR_R_SYS_LIB);
        goto end;
    }
    if (type == SSL_FILETYPE_ASN1) {
        j = ERR_R_ASN1_LIB;
        x = d2i_X509_bio(in, nullptr);
    } else if (type == SSL_FILETYPE_PEM) {
        j = ERR_R_PEM_LIB;
        x = PEM_read_bio_X509(in, nullptr, ctx->default_passwd_callback,
                              ctx->default_passwd_callback_userdata);
    } else {
        SSLerror(SSL_R_BAD_SSL_FILETYPE);
        goto end;
    }

    if (x == nullptr) {
        SSLerror(j);
        goto end;
    }

    ret = SSL_CTX_use_certificate(ctx, x);

end:
    X509_free(x);
    BIO_free(in);
    return (ret);
}

/**
 * Use the OpenSSL-TLS.
 *
 * @author zer0
 * @date 2018-02-04
 *
 * @remarks
 *  <pre>
 *          |-> DATA/TLS/Encode -> SSL_write() -> BIO(Write) -> readFromWriteBuffer() ->|
 *   [USER] |                                                                           | [SOCKET]
 *          |<- DATA/TLS/Decode ->  SSL_read() <-  BIO(Read) <-   writeToReadBuffer() <-|
 *  </pre>
 */
struct Tls::Impl : private Noncopyable
{
    std::shared_ptr<SSL_CTX> context;
    std::shared_ptr<SSL> ssl;

    BIO * write_bio;
    BIO *  read_bio;

    Impl() : write_bio(nullptr), read_bio(nullptr)
    {
        if (!init()) {
            throw std::bad_alloc();
        }
        if (!initBio()) {
            throw std::bad_alloc();
        }
    }

    Impl(std::string const & cert_pem, std::string const & key_pem)
    {
        if (!initPem(cert_pem, key_pem)) {
            throw std::bad_alloc();
        }
        if (!initBio()) {
            throw std::bad_alloc();
        }
    }

    Impl(std::shared_ptr<SSL_CTX> const & ctx) : context(ctx)
    {
        if (!initBio(ctx.get())) {
            throw std::bad_alloc();
        }
    }

    ~Impl()
    {
        ssl.reset();
        context.reset();
    }

    bool isFinished() const
    {
        assert(static_cast<bool>(ssl));
        return SSL_is_init_finished(ssl.get());
    }

    std::string getCipherName() const
    {
        assert(static_cast<bool>(ssl));
        return SSL_get_cipher(ssl.get());
    }

    int getError(int ret_code) const
    {
        assert(static_cast<bool>(ssl));
        return SSL_get_error(ssl.get(), ret_code);
    }

    bool init()
    {
        context.reset(SSL_CTX_new(SSLv23_method()), [](SSL_CTX * ctx){
            SSL_CTX_free(ctx);
        });
        if (!context) {
            tDLogE("Tls::Impl::init() OpenSSL Unable to create a new SSL context structure.");
            return false;
        }
        SSL_CTX_set_verify(context.get(), SSL_VERIFY_NONE, nullptr);
        return true;
    }

    /**
     * Initialize Privacy-enhanced Electronic Mail (Privacy Enhanced Mail, PEM) file.
     *
     * @remarks
     *  X.509 base64
     */
    bool initPem(std::string const & cert_pem, std::string const & key_pem)
    {
        assert(!cert_pem.empty());
        assert(!key_pem.empty());

        context.reset(SSL_CTX_new(SSLv23_server_method()), [](SSL_CTX * ctx){
            SSL_CTX_free(ctx);
        });

        if (!context) {
            tDLogE("Tls::Impl::initPem() OpenSSL Unable to create a new SSL context structure.");
            return false;
        }

        SSL_CTX_set_ecdh_auto(context.get(), 1);

        auto const CERT_CODE = SSL_CTX_use_certificate_memory(context.get(), cert_pem.data(), (int)cert_pem.size(), SSL_FILETYPE_PEM);
        if (CERT_CODE <= 0) {
            tDLogE("Tls::Impl::initPem() Certificate memory error: {}", CERT_CODE);
            return false;
        }

        auto const KEY_CODE = SSL_CTX_use_PrivateKey_memory(context.get(), key_pem.data(), (int)key_pem.size(), SSL_FILETYPE_PEM);
        if (KEY_CODE <= 0) {
            tDLogE("Tls::Impl::initPem() Private key memory error: {}", KEY_CODE);
            return false;
        }

        // Verify private key
        if (!SSL_CTX_check_private_key(context.get())) {
            tDLogE("Tls::Impl::initPem() Private key does not match the public certificate.");
            return false;
        }

        return true;
    }

    bool initBio()
    {
        assert(static_cast<bool>(context));
        return initBio(context.get());
    }

    bool initBio(SSL_CTX * ctx)
    {
        assert(ctx != nullptr);
        ssl.reset(SSL_new(ctx), [](SSL * ssl){
            SSL_free(ssl);
        });

        if (!ssl) {
            tDLogE("Tls::Impl::initBio() OpenSSL SSL error.");
            return false;
        }

        read_bio  = BIO_new(BIO_s_mem());
        write_bio = BIO_new(BIO_s_mem());
        SSL_set_bio(ssl.get(), read_bio, write_bio);
        return true;
    }

    int checkError(int ret_code) const
    {
        switch (getError(ret_code)) {
        case SSL_ERROR_NONE:
        case SSL_ERROR_SSL:
            // Don't break, flush data first.
        case SSL_ERROR_WANT_READ:
        case SSL_ERROR_WANT_WRITE:
        case SSL_ERROR_WANT_X509_LOOKUP:
            // UpToDate.
            break;
        case SSL_ERROR_ZERO_RETURN:
        case SSL_ERROR_SYSCALL:
        case SSL_ERROR_WANT_CONNECT:
        case SSL_ERROR_WANT_ACCEPT:
            //ERR_print_errors_fp(stderr);
        default: break;
        }
        return ret_code;
    }

    void accept()
    {
        assert(static_cast<bool>(ssl));
        SSL_set_accept_state(ssl.get());
    }

    void connect()
    {
        assert(static_cast<bool>(ssl));
        SSL_set_connect_state(ssl.get());
    }

    Err shutdown()
    {
        assert(static_cast<bool>(ssl));
        int const CODE = SSL_shutdown(ssl.get());
        if (CODE == 0) {
            // The shutdown is not yet finished.
            // Call SSL_shutdown() for a second time, if a bidirectional shutdown shall be performed.
            // The output of SSL_get_error(3) may be misleading,
            // as an erroneous SSL_ERROR_SYSCALL may be flagged even though no error occurred.
            return Err::E_SSL;
        } else if (CODE == 1) {
            // The shutdown was successfully completed. The "close notify" alert was sent
            // and the peer's "close notify" alert was received.
            return Err::E_SUCCESS;
        } else if (CODE < 0) {
            // The shutdown was not successful
            // because a fatal error occurred either at the protocol level or a connection failure occurred.
            // It can also occur if action is need to continue the operation for non-blocking BIOs.
            // Call SSL_get_error(3) with the return value ret to find out the reason.
            return Err::E_SSL;
        } else {
            return Err::E_UNKNOWN;
        }
    }

    Err handshake()
    {
        assert(static_cast<bool>(ssl));
        int const CODE = SSL_do_handshake(ssl.get());
        if (CODE == 0) {
            // The TLS/SSL handshake was successfully completed,
            // a TLS/SSL connection has been established.
            return Err::E_SUCCESS;
        } else if (CODE == 1) {
            // The TLS/SSL handshake was not successful
            // but was shut down controlled and by the specifications of the TLS/SSL protocol.
            // Call SSL_get_error() with the return value ret to find out the reason.
        } else if (CODE < 0) {
            // The TLS/SSL handshake was not successful
            // because a fatal error occurred either at the protocol level or a connection failure occurred.
            // The shutdown was not clean.
        } else {
            return Err::E_UNKNOWN;
        }

        // @formatter:off
        auto const REASON = SSL_get_error(ssl.get(), CODE);
        switch (REASON) {
        case SSL_ERROR_NONE:              return Err::E_SSL_NONE;
        case SSL_ERROR_SSL:               return Err::E_SSL;
        case SSL_ERROR_WANT_READ:         return Err::E_SSLWREAD;
        case SSL_ERROR_WANT_WRITE:        return Err::E_SSLWWRITE;
        case SSL_ERROR_WANT_X509_LOOKUP:  return Err::E_SSLWX509;
        default:
            tDLogE("Tls::Impl::handshake() OpenSSL SSL_do_handshake() error: code({}) reason({})", CODE, REASON);
            return Err::E_UNKNOWN;
        }
        // @formatter:on
    }

    inline int pendingWriteBio() const
    {
        return BIO_pending(write_bio);
    }

    inline int pendingReadBio() const
    {
        return BIO_pending(read_bio);
    }

    Err readFromWriteBuffer(std::vector<char> & result)
    {
        int const PENDING = pendingWriteBio();
        if (PENDING <= 0) {
            return Err::E_SSL;
        }

        assert(PENDING > 0);
        result.resize(static_cast<std::size_t>(PENDING));

        int const BIO_READ_RESULT = BIO_read(write_bio, result.data(), result.size());
        if (BIO_READ_RESULT > 0) {
            assert(BIO_READ_RESULT == result.size());
            return Err::E_SUCCESS;
        } else if (BIO_READ_RESULT == -2) {
            return Err::E_UNSUPOP;
        } else {
            assert(BIO_READ_RESULT <= 0);
        }
        return Err::E_SSL;
    }

    /**
     * @remarks
     *  <pre>
     *   Input data -> SSL_write() -> BIO(Write) -> Result buffer.
     *  </pre>
     */
    Err encode(void const * data, std::size_t size, std::vector<char> & result)
    {
        assert(static_cast<bool>(ssl));
        int const WRITE_RESULT = SSL_write(ssl.get(), data, size);
        if (WRITE_RESULT <= 0) {
            // The write operation was not successful,
            // because either the connection was closed,
            // an error occurred or action must be taken by the calling process.
            tDLogE("Tls::Impl::encode() OpenSSL SSL_write() error: reason({})",
                   SSL_get_error(ssl.get(), WRITE_RESULT));
            return Err::E_SSL;
        }

        // The write operation was successful,
        // the return value is the number of bytes actually written to the TLS/SSL connection.
        assert(WRITE_RESULT > 0);
        return readFromWriteBuffer(result);
    }

    Err writeToReadBuffer(void const * data, std::size_t size, std::size_t * write_size)
    {
        int const BIO_WRITE_RESULT = BIO_write(read_bio, data, size);
        if (BIO_WRITE_RESULT == -2) {
            return Err::E_UNSUPOP;
        } else if (BIO_WRITE_RESULT <= 0) {
            return Err::E_SSL;
        }

        if (write_size != nullptr) {
            *write_size = static_cast<std::size_t>(BIO_WRITE_RESULT);
        }
        return Err::E_SUCCESS;
    }

    // Maximum record size of 16kB for SSLv3/TLSv1
    // https://wiki.openssl.org/index.php/Manual:SSL_read(3)
    TBAG_CONSTEXPR static std::size_t const MAX_BYTE   = 1024 * 16;
    TBAG_CONSTEXPR static std::size_t const FIRST_BYTE = 1024;

    Err read(std::vector<char> & result, std::size_t buffer_size = FIRST_BYTE)
    {
        assert(static_cast<bool>(ssl));
        std::vector<char> buffer(buffer_size);
        int const READ_RESULT = SSL_read(ssl.get(), buffer.data(), buffer.size());
        if (READ_RESULT <= 0) {
            // The read operation was not successful, because either the connection was closed,
            // an error occurred or action must be taken by the calling process.
            int const SSL_ERROR = SSL_get_error(ssl.get(), READ_RESULT);
            if (SSL_ERROR == SSL_ERROR_WANT_READ) {
                if (buffer_size >= MAX_BYTE) {
                    return Err::E_SSLWREAD;
                } else {
                    buffer.reserve(0);
                    return read(result, MAX_BYTE);
                }
            } else {
                return Err::E_SSL;
            }
        }

        tDLogD("Tls::Impl::read({}) SSL_read() result: {}", buffer_size, READ_RESULT);
        assert(READ_RESULT > 0);
        result.assign(buffer.begin(), buffer.begin() + READ_RESULT);
        return Err::E_SUCCESS;
    }

    /**
     * @remarks
     *  <pre>
     *   Input data -> BIO(read) -> SSL_read() -> Result buffer.
     *  </pre>
     */
    Err decode(void const * data, std::size_t size, std::vector<char> & result)
    {
        std::size_t write_size = 0;
        Err const WRITE_CODE = writeToReadBuffer(data, size, &write_size);
        if (WRITE_CODE != Err::E_SUCCESS) {
            return WRITE_CODE;
        }
        assert(WRITE_CODE == Err::E_SUCCESS);
        return read(result, write_size);
    }

    /** Read & decode from pending data. */
    Err decode(std::vector<char> & result)
    {
        int const PENDING_SIZE = pendingReadBio();
        if (PENDING_SIZE <= 0) {
            return Err::E_SSLEREAD;
        }
        assert(PENDING_SIZE > 0);
        return read(result, static_cast<std::size_t>(PENDING_SIZE));
    }
};

// ------------------
// Tls implementation
// ------------------

Tls::Tls() : _impl(std::make_unique<Impl>())
{
    assert(static_cast<bool>(_impl));
}

Tls::Tls(std::string const & cert_pem, std::string const & key_pem)
        : _impl(std::make_unique<Impl>(cert_pem, key_pem))
{
    assert(static_cast<bool>(_impl));
}

Tls::Tls(X509Pem const & x509)
        : _impl(std::make_unique<Impl>(x509.certificate, x509.private_key))
{
    assert(static_cast<bool>(_impl));
}

Tls::Tls(create_memory_cert)
        : Tls(genX509Pem())
{
    assert(static_cast<bool>(_impl));
}

Tls::Tls(reference_ssl_context, Tls const & tls)
        : _impl(std::make_unique<Impl>(tls._impl->context))
{
    assert(static_cast<bool>(_impl));
}

Tls::~Tls()
{
    // EMPTY.
}

bool Tls::isFinished() const
{
    assert(static_cast<bool>(_impl));
    return _impl->isFinished();
}

std::string Tls::getCipherName() const
{
    assert(static_cast<bool>(_impl));
    return _impl->getCipherName();
}

void Tls::accept()
{
    assert(static_cast<bool>(_impl));
    _impl->accept();
}

void Tls::connect()
{
    assert(static_cast<bool>(_impl));
    _impl->connect();
}

Err Tls::handshake()
{
    assert(static_cast<bool>(_impl));
    return _impl->handshake();
}

Err Tls::readFromWriteBuffer(std::vector<char> & result)
{
    assert(static_cast<bool>(_impl));
    return _impl->readFromWriteBuffer(result);
}

Err Tls::writeToReadBuffer(void const * data, std::size_t size, std::size_t * write_size)
{
    assert(static_cast<bool>(_impl));
    return _impl->writeToReadBuffer(data, size, write_size);
}

int Tls::pendingOfEncodeBufferSize() const
{
    assert(static_cast<bool>(_impl));
    return _impl->pendingWriteBio();
}

int Tls::pendingOfDecodeBufferSize() const
{
    assert(static_cast<bool>(_impl));
    return _impl->pendingReadBio();
}

std::vector<char> Tls::encode(void const * data, std::size_t size, Err * code)
{
    assert(static_cast<bool>(_impl));
    std::vector<char> result;
    Err const ENCODE_CODE = _impl->encode(data, size, result);
    if (code != nullptr) {
        *code = ENCODE_CODE;
    }
    return result;
}

std::vector<char> Tls::decode(void const * data, std::size_t size, Err * code)
{
    assert(static_cast<bool>(_impl));
    std::vector<char> result;
    Err const ENCODE_CODE = _impl->decode(data, size, result);
    if (code != nullptr) {
        *code = ENCODE_CODE;
    }
    return result;
}

std::vector<char> Tls::decode(Err * code)
{
    assert(static_cast<bool>(_impl));
    std::vector<char> result;
    Err const ENCODE_CODE = _impl->decode(result);
    if (code != nullptr) {
        *code = ENCODE_CODE;
    }
    return result;
}

// ------------------------
// TlsReader implementation
// ------------------------

TlsReader::TlsReader(TlsReaderInterface * cb)
        : _tls(), _callback(cb), _state(TlsState::TS_NOT_READY)
{
    // EMPTY.
}

TlsReader::TlsReader(TlsReaderInterface * cb, std::string const & cert_pem, std::string const & key_pem)
        : _tls(cert_pem, key_pem), _callback(cb), _state(TlsState::TS_NOT_READY)
{
    // EMPTY.
}

TlsReader::TlsReader(TlsReaderInterface * cb, mem_crt const & UNUSED_PARAM(init))
        : _tls(mem_crt{}), _callback(cb), _state(TlsState::TS_NOT_READY)
{
    // EMPTY.
}

TlsReader::TlsReader(TlsReaderInterface * cb, ref_ssl const & UNUSED_PARAM(init), Tls const & tls)
        : _tls(ref_ssl{}, tls), _callback(cb), _state(TlsState::TS_NOT_READY)
{
    // EMPTY.
}

TlsReader::~TlsReader()
{
    // EMPTY.
}

Err TlsReader::connect()
{
    _tls.connect();
    Err const HANDSHAKE_CODE = _tls.handshake();
    // Handshake is not finished, we can ignore it.
    assert(HANDSHAKE_CODE == Err::E_SSLWREAD);

    Buffer buffer;
    Err const WRITE_BUFFER_CODE = _tls.readFromWriteBuffer(buffer);
    if (isFailure(WRITE_BUFFER_CODE)) {
        return WRITE_BUFFER_CODE;
    }

    // [CLIENT HELLO]
    Err const WRITE_CODE = _callback->onWrite(buffer.data(), buffer.size());
    if (isFailure(WRITE_CODE)) {
        return WRITE_CODE;
    }

    tDLogD("TlsReader::connect() Update state: Not ready -> Connect & Handshaking");
    assert(_state == TlsState::TS_NOT_READY);
    _state = TlsState::TS_HANDSHAKE;
    return Err::E_SUCCESS;
}

Err TlsReader::accept()
{
    _tls.accept();

    tDLogD("TlsReader::accept() Update state: Not ready -> Accept & Handshaking");
    assert(_state == TlsState::TS_NOT_READY);
    _state = TlsState::TS_HANDSHAKE;
    return Err::E_SUCCESS;
}

Err TlsReader::parse(char const * buffer, std::size_t size)
{
    switch (_state) {
    case TlsState::TS_NOT_READY: return Err::E_ILLSTATE;
    case TlsState::TS_HANDSHAKE: return onHandshaking(buffer, size);
    case TlsState::TS_FINISH:    return onApplication(buffer, size);
    default:                     return Err::E_UNKNOWN;
    }
}

Err TlsReader::onHandshaking(char const * buffer, std::size_t size)
{
    assert(_state == TlsState::TS_HANDSHAKE);

    Err const READ_BUFFER_CODE = _tls.writeToReadBuffer(buffer, size);
    if (isFailure(READ_BUFFER_CODE)) {
        return READ_BUFFER_CODE;
    }

    Err const HANDSHAKE_CODE = _tls.handshake();
    if (HANDSHAKE_CODE != Err::E_SSL_NONE) {
        if (HANDSHAKE_CODE == Err::E_SSLWREAD) {
            Buffer write_buffer;
            _tls.readFromWriteBuffer(write_buffer); // Skip error code check.

            if (!write_buffer.empty()) {
                auto const WRITE_CODE = _callback->onWrite(write_buffer.data(), write_buffer.size());
                if (isFailure(WRITE_CODE)) {
                    return WRITE_CODE;
                }
            }
        } else {
            return HANDSHAKE_CODE;
        }
    }

    if (_tls.isFinished()) {
        tDLogD("TlsReader::onHandshaking() Update state: Handshaking -> Finish");
        _state = TlsState::TS_FINISH;
    }
    return Err::E_SUCCESS;
}

Err TlsReader::onApplication(char const * buffer, std::size_t size)
{
    assert(_state == TlsState::TS_FINISH);

    Err decode_code;
    auto const BUFFER = _tls.decode(buffer, size, &decode_code);
    if (isFailure(decode_code)) {
        return decode_code;
    }

    auto const READ_CODE = _callback->onRead(BUFFER.data(), BUFFER.size());
    if (isFailure(READ_CODE)) {
        return READ_CODE;
    }

    return Err::E_SUCCESS;
}

} // namespace crypto

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

