/**
 * @file   NngSocket.cpp
 * @brief  NngSocket class implementation.
 * @author zer0
 * @date   2019-05-29
 */

#include <libtbag/mq/NngSocket.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/debug/Assert.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq {

/**
 * NngSocket::Impl class prototype.
 *
 * @author zer0
 * @date   2019-05-29
 *
 */
struct NngSocket::Impl : private Noncopyable
{
private:
    nng_socket _socket;
    SocketType _type;
    bool _raw;
    bool _opened;

public:
    inline nng_socket socket() const TBAG_NOEXCEPT { return _socket; }
    inline SocketType   type() const TBAG_NOEXCEPT { return _type;   }
    inline bool          raw() const TBAG_NOEXCEPT { return _raw;    }
    inline bool       opened() const TBAG_NOEXCEPT { return _opened; }

public:
    Impl(SocketType type, bool raw) : _socket(), _type(SocketType::ST_NONE),
                                      _raw(false), _opened(false)
    {
        auto const CODE = open(type, raw);
        if (isFailure(CODE)) {
            throw ErrException(CODE);
        }
    }

    ~Impl()
    {
        close();
    }

    Err open(SocketType type, bool raw)
    {
        // clang-format off
        int code = 0;
        if (raw) {
            switch (type) {
            case SocketType::ST_BUS0:        code = nng_bus0_open_raw       (&_socket); break;
            case SocketType::ST_PAIR0:       code = nng_pair0_open_raw      (&_socket); break;
            case SocketType::ST_PAIR1:       code = nng_pair1_open_raw      (&_socket); break;
            case SocketType::ST_PULL0:       code = nng_pull0_open_raw      (&_socket); break;
            case SocketType::ST_PUSH0:       code = nng_push0_open_raw      (&_socket); break;
            case SocketType::ST_PUB0:        code = nng_pub0_open_raw       (&_socket); break;
            case SocketType::ST_SUB0:        code = nng_sub0_open_raw       (&_socket); break;
            case SocketType::ST_REP0:        code = nng_rep0_open_raw       (&_socket); break;
            case SocketType::ST_REQ0:        code = nng_req0_open_raw       (&_socket); break;
            case SocketType::ST_RESPONDENT0: code = nng_respondent0_open_raw(&_socket); break;
            case SocketType::ST_SURVEYOR0:   code = nng_surveyor0_open_raw  (&_socket); break;
            default:
                TBAG_INACCESSIBLE_BLOCK_ASSERT();
                return E_ILLARGS;
            }
        } else {
            switch (type) {
            case SocketType::ST_BUS0:        code = nng_bus0_open       (&_socket); break;
            case SocketType::ST_PAIR0:       code = nng_pair0_open      (&_socket); break;
            case SocketType::ST_PAIR1:       code = nng_pair1_open      (&_socket); break;
            case SocketType::ST_PULL0:       code = nng_pull0_open      (&_socket); break;
            case SocketType::ST_PUSH0:       code = nng_push0_open      (&_socket); break;
            case SocketType::ST_PUB0:        code = nng_pub0_open       (&_socket); break;
            case SocketType::ST_SUB0:        code = nng_sub0_open       (&_socket); break;
            case SocketType::ST_REP0:        code = nng_rep0_open       (&_socket); break;
            case SocketType::ST_REQ0:        code = nng_req0_open       (&_socket); break;
            case SocketType::ST_RESPONDENT0: code = nng_respondent0_open(&_socket); break;
            case SocketType::ST_SURVEYOR0:   code = nng_surveyor0_open  (&_socket); break;
            default:
                TBAG_INACCESSIBLE_BLOCK_ASSERT();
                return E_ILLARGS;
            }
        }
        // clang-format on

        if (code == 0) {
            _type = type;
            _raw = raw;
            _opened = true;
        }
        return nng_code_err(code);
    }

    Err close()
    {
        if (_opened) {
            _opened = false;
            return nng_code_err(nng_close(_socket));
        }
        return E_ILLSTATE;
    }
};

// ---------
// NngSocket
// ---------

char const * const NngSocket::getSocketTypeName(SocketType type) TBAG_NOEXCEPT
{
    // clang-format off
    switch (type) {
    case SocketType::ST_NONE:        return "NONE"       ;
    case SocketType::ST_BUS0:        return "BUS0"       ;
    case SocketType::ST_PAIR0:       return "PAIR0"      ;
    case SocketType::ST_PAIR1:       return "PAIR1"      ;
    case SocketType::ST_PULL0:       return "PULL0"      ;
    case SocketType::ST_PUSH0:       return "PUSH0"      ;
    case SocketType::ST_PUB0:        return "PUB0"       ;
    case SocketType::ST_SUB0:        return "SUB0"       ;
    case SocketType::ST_REP0:        return "REP0"       ;
    case SocketType::ST_REQ0:        return "REQ0"       ;
    case SocketType::ST_RESPONDENT0: return "RESPONDENT0";
    case SocketType::ST_SURVEYOR0:   return "SURVEYOR0"  ;
    default:                         return "UNKNOWN"    ;
    }
    // clang-format on
}

NngSocket::NngSocket() : _impl(nullptr)
{
    // EMPTY.
}

NngSocket::NngSocket(NngSocket const & obj) TBAG_NOEXCEPT : _impl(nullptr)
{
    (*this) = obj;
}

NngSocket::NngSocket(NngSocket && obj) TBAG_NOEXCEPT : _impl(nullptr)
{
    (*this) = std::move(obj);
}

NngSocket::~NngSocket()
{
    // EMPTY.
}

NngSocket & NngSocket::operator =(NngSocket const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

NngSocket & NngSocket::operator =(NngSocket && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void NngSocket::copy(NngSocket const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
    }
}

void NngSocket::swap(NngSocket & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
    }
}

void NngSocket::reset()
{
    _impl.reset();
}

Err NngSocket::open(SocketType type, bool raw)
{
    Impl * impl = nullptr;
    try {
        impl = new Impl(type, raw);
    } catch (ErrException e) {
        return e.CODE;
    } catch (...) {
        return E_UNKEXCP;
    }
    assert(impl != nullptr);
    _impl.reset(impl);
    return E_SUCCESS;
}

Err NngSocket::close()
{
    if (_impl && _impl->opened()) {
        auto const CODE = _impl->close();
        assert(!_impl->opened());
        _impl.reset();
        return CODE;
    }
    return E_ILLSTATE;
}

NngSocket::SocketType NngSocket::getType() const
{
    assert(exists());
    return _impl->type();
}

char const * const NngSocket::getTypeName() const
{
    assert(exists());
    return getSocketTypeName(_impl->type());
}

bool NngSocket::isRaw() const
{
    assert(exists());
    return _impl->raw();
}

bool NngSocket::isOpened() const
{
    return _impl && _impl->opened();
}

nng_socket NngSocket::getSocket() const
{
    assert(exists());
    return _impl->socket();
}

Err NngSocket::listen(char const * url, nng_listener * lp, int flags)
{
    assert(exists());
    return nng_code_err(nng_listen(getSocket(), url, lp, flags));
}

Err NngSocket::dial(char const * url, nng_dialer * lp, int flags)
{
    assert(exists());
    return nng_code_err(nng_dial(getSocket(), url, lp, flags));
}

Err NngSocket::send(void * data, size_t size, int flags)
{
    assert(exists());
    return nng_code_err(nng_send(getSocket(), data, size, flags));
}

Err NngSocket::recv(void * data, size_t * size, int flags)
{
    assert(exists());
    return nng_code_err(nng_recv(getSocket(), data, size, flags));
}

Err NngSocket::setRecvTimeout(nng_duration ms)
{
    assert(exists());
    return nng_code_err(nng_setopt_ms(getSocket(), NNG_OPT_RECVTIMEO, ms));
}

Err NngSocket::setSendTimeout(nng_duration ms)
{
    assert(exists());
    return nng_code_err(nng_setopt_ms(getSocket(), NNG_OPT_SENDTIMEO, ms));
}

Err NngSocket::getRecvTimeout(nng_duration * ms) const
{
    assert(exists());
    return nng_code_err(nng_getopt_ms(getSocket(), NNG_OPT_RECVTIMEO, ms));
}

Err NngSocket::getSendTimeout(nng_duration * ms) const
{
    assert(exists());
    return nng_code_err(nng_getopt_ms(getSocket(), NNG_OPT_SENDTIMEO, ms));
}

Err NngSocket::setRecvBuffer(int size)
{
    assert(exists());
    return nng_code_err(nng_setopt_int(getSocket(), NNG_OPT_RECVBUF, size));
}

Err NngSocket::setSendBuffer(int size)
{
    assert(exists());
    return nng_code_err(nng_setopt_int(getSocket(), NNG_OPT_SENDBUF, size));
}

Err NngSocket::getRecvBuffer(int * size) const
{
    assert(exists());
    return nng_code_err(nng_getopt_int(getSocket(), NNG_OPT_RECVBUF, size));
}

Err NngSocket::getSendBuffer(int * size) const
{
    assert(exists());
    return nng_code_err(nng_getopt_int(getSocket(), NNG_OPT_SENDBUF, size));
}

} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

