/**
 * @file   TlsReader.cpp
 * @brief  TlsReader class implementation.
 * @author zer0
 * @date   2017-11-11
 */

#include <libtbag/network/http/tls/TlsReader.hpp>
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

namespace network {
namespace http    {
namespace tls     {

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
        if (read_bio != nullptr) {
            BIO_free(read_bio);
            read_bio = nullptr;
        }
        if (write_bio != nullptr) {
            BIO_free(write_bio);
            write_bio = nullptr;
        }
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
            tDLogE("TlsReader::Impl::handshake() OpenSSL SSL_do_handshake() error: Ret({}), Err({})",
                   CODE, SSL_get_error(ssl, CODE));
            return Err::E_SSL_WTRD;
        } else if (CODE < 0) {
            // The TLS/SSL handshake was not successful
            // because a fatal error occurred either at the protocol level or a connection failure occurred.
            // The shutdown was not clean.
            // It can also occur of action is need to continue the operation for non-blocking BIOs.
            // Call SSL_get_error() with the return value ret to find out the reason.
            int const SSL_ERROR = SSL_get_error(ssl, CODE);
            if (SSL_ERROR == SSL_ERROR_WANT_READ) {
                return Err::E_SSL_WTRD;
            } else {
                tDLogE("TlsReader::Impl::handshake() OpenSSL SSL_do_handshake() error: Ret({}), Err({})",
                       CODE, SSL_ERROR);
                return Err::E_SSL;
            }
        } else {
            return Err::E_UNKNOWN;
        }
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
            // Call SSL_get_error() with the return value ret to find out the reason.
            return Err::E_SSL;
        }

        assert(WRITE_RESULT > 0);

        // The write operation was successful,
        // the return value is the number of bytes actually written to the TLS/SSL connection.
        int const PENDING = BIO_pending(write_bio);
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
     *   Input data -> BIO(read) -> SSL_read() -> Result buffer.
     *  </pre>
     */
    Err decode(void const * data, std::size_t size, std::vector<char> & result)
    {
        int const BIO_WRITE_RESULT = BIO_write(read_bio, data, size);
        if (BIO_WRITE_RESULT == -2) {
            return Err::E_UNSUPOP;
        } else if (BIO_WRITE_RESULT <= 0) {
            return Err::E_SSL;
        }

        assert(BIO_WRITE_RESULT > 0);
        result.resize(static_cast<std::size_t>(BIO_WRITE_RESULT));

        int const READ_RESULT = SSL_read(ssl, result.data(), BIO_WRITE_RESULT);
        if (READ_RESULT <= 0) {
            // The read operation was not successful, because either the connection was closed,
            // an error occurred or action must be taken by the calling process.
            // Call SSL_get_error(3) with the return value ret to find out the reason.
            return Err::E_SSL;
        }

        // The read operation was successful.
        // The return value is the number of bytes
        // actually read from the TLS/SSL connection.
        assert(READ_RESULT > 0);
        assert(READ_RESULT == result.size());
        return Err::E_SUCCESS;
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

} // namespace tls
} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

