/**
 * @file   NngSocket.cpp
 * @brief  NngSocket class implementation.
 * @author zer0
 * @date   2019-05-29
 */

#include <libtbag/mq/NngSocket.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/time/DurationString.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/util/ByteString.hpp>

#include <cassert>
#include <sstream>
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
    bool _opened;

public:
    inline nng_socket socket() const TBAG_NOEXCEPT { return _socket; }
    inline SocketType   type() const TBAG_NOEXCEPT { return _type;   }
    inline bool       opened() const TBAG_NOEXCEPT { return _opened; }

public:
    Impl(SocketType type, bool raw) : _socket(), _type(SocketType::ST_NONE), _opened(false)
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
            _opened = true;
        }
        return nng_code_err(code);
    }

    Err close()
    {
        if (!_opened) {
            return E_ILLSTATE;
        }
        _opened = false;
        return nng_code_err(nng_close(_socket));
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

bool NngSocket::isOpened() const
{
    return _impl && _impl->opened();
}

nng_socket NngSocket::getSocket() const
{
    assert(exists());
    return _impl->socket();
}

Err NngSocket::listen(std::string const & url, nng_listener * lp, int flags)
{
    assert(exists());
    return nng_code_err(nng_listen(getSocket(), url.c_str(), lp, flags));
}

Err NngSocket::dial(std::string const & url, nng_dialer * lp, int flags)
{
    assert(exists());
    return nng_code_err(nng_dial(getSocket(), url.c_str(), lp, flags));
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

Err NngSocket::setopt(std::string const & key, bool value)
{
    assert(exists());
    return nng_code_err(nng_setopt_bool(getSocket(), key.c_str(), value));
}

Err NngSocket::setopt(std::string const & key, int value)
{
    assert(exists());
    return nng_code_err(nng_setopt_int(getSocket(), key.c_str(), value));
}

Err NngSocket::setopt(std::string const & key, size_t value)
{
    assert(exists());
    return nng_code_err(nng_setopt_size(getSocket(), key.c_str(), value));
}

Err NngSocket::setopt(std::string const & key, std::string const & value)
{
    assert(exists());
    return nng_code_err(nng_setopt_string(getSocket(), key.c_str(), value.c_str()));
}

Err NngSocket::setopt(std::string const & key, void * value)
{
    assert(exists());
    return nng_code_err(nng_setopt_ptr(getSocket(), key.c_str(), value));
}

Err NngSocket::getopt(std::string const & key, bool * value) const
{
    assert(exists());
    return nng_code_err(nng_getopt_bool(getSocket(), key.c_str(), value));
}

Err NngSocket::getopt(std::string const & key, int * value) const
{
    assert(exists());
    return nng_code_err(nng_getopt_int(getSocket(), key.c_str(), value));
}

Err NngSocket::getopt(std::string const & key, size_t * value) const
{
    assert(exists());
    return nng_code_err(nng_getopt_size(getSocket(), key.c_str(), value));
}

Err NngSocket::getopt(std::string const & key, std::string & value) const
{
    assert(exists());
    char * buffer = nullptr;
    auto const code = nng_getopt_string(getSocket(), key.c_str(), &buffer);
    if (buffer != nullptr) {
        assert(code == 0);
        value.assign(buffer);
        nng_strfree(buffer);
    }
    return nng_code_err(code);
}

Err NngSocket::getopt(std::string const & key, void ** value) const
{
    assert(exists());
    return nng_code_err(nng_getopt_ptr(getSocket(), key.c_str(), value));
}

Err NngSocket::setopt_uint64(std::string const & key, uint64_t value)
{
    assert(exists());
    return nng_code_err(nng_setopt_uint64(getSocket(), key.c_str(), value));
}

Err NngSocket::getopt_uint64(std::string const & key, uint64_t * value) const
{
    assert(exists());
    return nng_code_err(nng_getopt_uint64(getSocket(), key.c_str(), value));
}

Err NngSocket::setopt_duration(std::string const & key, nng_duration value)
{
    assert(exists());
    return nng_code_err(nng_setopt_ms(getSocket(), key.c_str(), value));
}

Err NngSocket::getopt_duration(std::string const & key, nng_duration * value) const
{
    assert(exists());
    return nng_code_err(nng_getopt_ms(getSocket(), key.c_str(), value));
}

Err NngSocket::setRecvTimeout(nng_duration ms)
{
    return setopt_duration(NNG_OPT_RECVTIMEO, ms);
}

Err NngSocket::getRecvTimeout(nng_duration * ms) const
{
    return getopt_duration(NNG_OPT_RECVTIMEO, ms);
}

Err NngSocket::setSendTimeout(nng_duration ms)
{
    return setopt_duration(NNG_OPT_SENDTIMEO, ms);
}

Err NngSocket::getSendTimeout(nng_duration * ms) const
{
    return getopt_duration(NNG_OPT_SENDTIMEO, ms);
}

Err NngSocket::setRecvNumberOfMessages(int size)
{
    return setopt(NNG_OPT_RECVBUF, size);
}

Err NngSocket::getRecvNumberOfMessages(int * size) const
{
    return getopt(NNG_OPT_RECVBUF, size);
}

Err NngSocket::setSendNumberOfMessages(int size)
{
    if (0 <= COMPARE_AND(size) <= 8192) {
        return setopt(NNG_OPT_SENDBUF, size);
    } else {
        return E_ILLARGS;
    }
}

Err NngSocket::getSendNumberOfMessages(int * size) const
{
    return getopt(NNG_OPT_SENDBUF, size);
}

Err NngSocket::setRecvMaxSize(std::size_t size)
{
    return setopt(NNG_OPT_RECVMAXSZ, size);
}

Err NngSocket::getRecvMaxSize(std::size_t * size) const
{
    return getopt(NNG_OPT_RECVMAXSZ, size);
}

Err NngSocket::setUnlimitedRecvMaxSize()
{
    return setRecvMaxSize(0);
}

Err NngSocket::setReconnectTimeMin(nng_duration ms)
{
    return setopt_duration(NNG_OPT_RECONNMINT, ms);
}

Err NngSocket::getReconnectTimeMin(nng_duration * ms) const
{
    return getopt_duration(NNG_OPT_RECONNMINT, ms);
}

Err NngSocket::setReconnectTimeMax(nng_duration ms)
{
    return setopt_duration(NNG_OPT_RECONNMAXT, ms);
}

Err NngSocket::getReconnectTimeMax(nng_duration * ms) const
{
    return getopt_duration(NNG_OPT_RECONNMAXT, ms);
}

Err NngSocket::setSocketName(std::string const & name)
{
    if (name.size() >= SOCKNAME_MAX_LENGTH) {
        return E_ILLARGS;
    }
    return setopt(NNG_OPT_SOCKNAME, name);
}

Err NngSocket::getSocketName(std::string & name) const
{
    return getopt(NNG_OPT_SOCKNAME, name);
}

Err NngSocket::setMaxTTL(int size)
{
    return setopt(NNG_OPT_MAXTTL, size);
}

Err NngSocket::getMaxTTL(int * size) const
{
    return getopt(NNG_OPT_MAXTTL, size);
}

Err NngSocket::getRaw(bool * mode) const
{
    return getopt(NNG_OPT_RAW, mode);
}

Err NngSocket::getUrl(std::string & url) const
{
    return getopt(NNG_OPT_URL, url);
}

Err NngSocket::getProtocolNumber(int * number) const
{
    return getopt(NNG_OPT_PROTO, number);
}

Err NngSocket::getPeerProtocolNumber(int * number) const
{
    return getopt(NNG_OPT_PEER, number);
}

Err NngSocket::getProtocolName(std::string & name) const
{
    return getopt(NNG_OPT_PROTONAME, name);
}

Err NngSocket::getPeerProtocolName(std::string & name) const
{
    return getopt(NNG_OPT_PEERNAME, name);
}

std::string NngSocket::getPrintableInformationText() const
{
    using namespace libtbag::time;
    using namespace libtbag::string;
    std::stringstream ss;

    nng_duration recv_timeout;
    if (isSuccess(getRecvTimeout(&recv_timeout))) {
        ss << "RecvTimeout: " << getUpperTimeTextByMilliseconds(recv_timeout) << NEW_LINE;
    }

    nng_duration send_timeout;
    if (isSuccess(getSendTimeout(&send_timeout))) {
        ss << "SendTimeout: " << getUpperTimeTextByMilliseconds(send_timeout) << NEW_LINE;
    }

    int recv_messages;
    if (isSuccess(getRecvNumberOfMessages(&recv_messages))) {
        ss << "RecvNumberOfMessages: " << recv_messages << NEW_LINE;
    }

    int send_messages;
    if (isSuccess(getSendNumberOfMessages(&send_messages))) {
        ss << "SendNumberOfMessages: " << send_messages << NEW_LINE;
    }

    std::size_t recv_max_size;
    if (isSuccess(getRecvMaxSize(&recv_max_size))) {
        ss << "RecvMaxSize: " << libtbag::util::toUpperByteText(recv_max_size) << NEW_LINE;
    }

    nng_duration reconnect_min;
    if (isSuccess(getReconnectTimeMin(&reconnect_min))) {
        ss << "ReconnectTimeMin: " << getUpperTimeTextByMilliseconds(reconnect_min) << NEW_LINE;
    }

    nng_duration reconnect_max;
    if (isSuccess(getReconnectTimeMax(&reconnect_max))) {
        ss << "ReconnectTimeMax: " << getUpperTimeTextByMilliseconds(reconnect_max) << NEW_LINE;
    }

    std::string socket_name;
    if (isSuccess(getSocketName(socket_name))) {
        ss << "SocketName: " << socket_name << NEW_LINE;
    }

    int ttl_size;
    if (isSuccess(getMaxTTL(&ttl_size))) {
        ss << "MaxTTL: " << ttl_size << "hops" << NEW_LINE;
    }

    bool raw_mode;
    if (isSuccess(getRaw(&raw_mode))) {
        ss << "Raw mode: " << (raw_mode ? "Enable" : "Disable") << NEW_LINE;
    }

    std::string url;
    if (isSuccess(getUrl(url))) {
        ss << "URL: " << url << NEW_LINE;
    }

    std::string protocol;
    if (isSuccess(getProtocolName(protocol))) {
        ss << "ProtocolName: " << protocol << NEW_LINE;
    }

    std::string peer_protocol;
    if (isSuccess(getPeerProtocolName(peer_protocol))) {
        ss << "PeerProtocolName: " << peer_protocol << NEW_LINE;
    }

    return ss.str();
}

void NngSocket::dumpState()
{
    nng_stat * stats;
    nng_stats_get(&stats);
    nng_stats_dump(stats);
    nng_stats_free(stats);
}

std::vector<NngSocket::State> NngSocket::getState()
{
    std::vector<State> result;
    nng_stat * stats;
    nng_stats_get(&stats);
    // TODO
    nng_stats_free(stats);
    return result;
}

} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

