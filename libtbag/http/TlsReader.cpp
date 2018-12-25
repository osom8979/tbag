/**
 * @file   TlsReader.cpp
 * @brief  TlsReader class implementation.
 * @author zer0
 * @date   2017-11-11
 * @date   2018-12-25 (Change namespace: libtbag::network::http::tls -> libtbag::http)
 */

#include <libtbag/http/TlsReader.hpp>
#include <libtbag/log/Log.hpp>

#include <openssl/err.h>
#include <openssl/ssl.h>
#include <openssl/conf.h>
#include <openssl/engine.h>

#include <cassert>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace http {

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
struct TlsReader::Impl : private Noncopyable
{
    TlsReader * parent;

    SSL     * ssl;
    SSL_CTX * context;

    BIO * write_bio;
    BIO *  read_bio;

    Impl(TlsReader * p) : parent(p), ssl(nullptr), context(nullptr), write_bio(nullptr), read_bio(nullptr)
    {
        if (init() == false) {
            throw std::bad_alloc();
        }
    }

    ~Impl()
    {
        release();
    }

    bool init()
    {
        SSL_METHOD const * method = SSLv23_method();
        if (method == nullptr) {
            tDLogE("TlsReader::Impl::init() OpenSSL method error.");
            return false;
        }

        context = SSL_CTX_new(method);
        if (context == nullptr) {
            tDLogE("TlsReader::Impl::init() OpenSSL Unable to create a new SSL context structure.");
            return false;
        }

        SSL_CTX_set_verify(context, SSL_VERIFY_NONE, nullptr);

        ssl = SSL_new(context);
        if (ssl == nullptr) {
            tDLogE("TlsReader::Impl::init() OpenSSL SSL error.");
            return false;
        }

        read_bio  = BIO_new(BIO_s_mem());
        write_bio = BIO_new(BIO_s_mem());
        SSL_set_bio(ssl, read_bio, write_bio);
        return true;
    }

    void release()
    {
        if (ssl != nullptr) {
            SSL_free(ssl);
            ssl = nullptr;
        }
        if (context != nullptr) {
            SSL_CTX_free(context);
            context = nullptr;
        }
    }

    bool isFinished() const
    {
        return SSL_is_init_finished(ssl);
    }

    std::string getCipherName() const
    {
        return SSL_get_cipher(ssl);
    }

    int getError(int ret_code) const
    {
        return SSL_get_error(ssl, ret_code);
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
        SSL_set_accept_state(ssl);
    }

    void connect()
    {
        SSL_set_connect_state(ssl);
    }

    Err shutdown()
    {
        int const CODE = SSL_shutdown(ssl);
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
        int const CODE = SSL_do_handshake(ssl);
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
        auto const REASON = SSL_get_error(ssl, CODE);
        switch (REASON) {
        case SSL_ERROR_NONE:              return Err::E_SSL;
        case SSL_ERROR_SSL:               return Err::E_SSL;
        case SSL_ERROR_WANT_READ:         return Err::E_SSLWREAD;
        case SSL_ERROR_WANT_WRITE:        return Err::E_SSLWWRITE;
        case SSL_ERROR_WANT_X509_LOOKUP:  return Err::E_SSLWX509;
        default:
            tDLogE("TlsReader::Impl::handshake() OpenSSL SSL_do_handshake() error: code({}) reason({})", CODE, REASON);
            return Err::E_SSL;
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
        int const WRITE_RESULT = SSL_write(ssl, data, size);
        if (WRITE_RESULT <= 0) {
            // The write operation was not successful,
            // because either the connection was closed,
            // an error occurred or action must be taken by the calling process.
            tDLogE("TlsReader::Impl::encode() OpenSSL SSL_write() error: reason({})",
                   SSL_get_error(ssl, WRITE_RESULT));
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
        std::vector<char> buffer(buffer_size);
        int const READ_RESULT = SSL_read(ssl, buffer.data(), buffer.size());
        if (READ_RESULT <= 0) {
            // The read operation was not successful, because either the connection was closed,
            // an error occurred or action must be taken by the calling process.
            int const SSL_ERROR = SSL_get_error(ssl, READ_RESULT);
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

        tDLogD("TlsReader::Impl::read({}) SSL_read() result: {}", buffer_size, READ_RESULT);
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

// -------------------------
// TlsReader implementation.
// -------------------------

TlsReader::TlsReader() : _impl(new Impl(this))
{
    // EMPTY.
}

TlsReader::~TlsReader()
{
    // EMPTY.
}

bool TlsReader::isFinished() const
{
    assert(_impl != nullptr);
    return _impl->isFinished();
}

std::string TlsReader::getCipherName() const
{
    assert(_impl != nullptr);
    return _impl->getCipherName();
}

void TlsReader::accept()
{
    assert(_impl != nullptr);
    _impl->accept();
}

void TlsReader::connect()
{
    assert(_impl != nullptr);
    _impl->connect();
}

Err TlsReader::handshake()
{
    assert(_impl != nullptr);
    return _impl->handshake();
}

Err TlsReader::readFromWriteBuffer(std::vector<char> & result)
{
    assert(_impl != nullptr);
    return _impl->readFromWriteBuffer(result);
}

Err TlsReader::writeToReadBuffer(void const * data, std::size_t size, std::size_t * write_size)
{
    assert(_impl != nullptr);
    return _impl->writeToReadBuffer(data, size, write_size);
}

int TlsReader::pendingOfEncodeBufferSize() const
{
    assert(_impl != nullptr);
    return _impl->pendingWriteBio();
}

int TlsReader::pendingOfDecodeBufferSize() const
{
    assert(_impl != nullptr);
    return _impl->pendingReadBio();
}

std::vector<char> TlsReader::encode(void const * data, std::size_t size, Err * code)
{
    assert(_impl != nullptr);
    std::vector<char> result;
    Err const ENCODE_CODE = _impl->encode(data, size, result);
    if (code != nullptr) {
        *code = ENCODE_CODE;
    }
    return result;
}

std::vector<char> TlsReader::decode(void const * data, std::size_t size, Err * code)
{
    assert(_impl != nullptr);
    std::vector<char> result;
    Err const ENCODE_CODE = _impl->decode(data, size, result);
    if (code != nullptr) {
        *code = ENCODE_CODE;
    }
    return result;
}

std::vector<char> TlsReader::decode(Err * code)
{
    assert(_impl != nullptr);
    std::vector<char> result;
    Err const ENCODE_CODE = _impl->decode(result);
    if (code != nullptr) {
        *code = ENCODE_CODE;
    }
    return result;
}

} // namespace http

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

