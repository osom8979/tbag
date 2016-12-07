/**
 * @file   Tcp.cpp
 * @brief  Tcp class implementation.
 * @author zer0
 * @date   2016-11-04
 */

#include <libtbag/network/socket/Tcp.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/loop/UvEventLoop.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cassert>
#include <cstring>
#include <cctype>

#include <algorithm>

#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace socket  {

//    uv_tcp_t     _tcp;     // onRead, onAlloc (for read), onConnection (for server), onClose
//    uv_write_t   _write;   // onWrite, onClose
//    uv_connect_t _connect; // onConnect

//    virtual void onRead(/*uv_stream_t*/void * stream, ssize_t nread, /*uv_buf_t*/void const * buf) override
//    {
//        Tcp::ReadErrorCode code;
//        if (nread == UV_EOF) {
//            code = Tcp::ReadErrorCode::END_OF_FILE;
//        } else if (nread < 0){
//            code = Tcp::ReadErrorCode::UNKNOWN_ERROR;
//        } else {
//            code = Tcp::ReadErrorCode::SUCCESS;
//        }
//
//        if (_parent._callback != nullptr) {
//            uv_buf_t const * uv_buf = static_cast<uv_buf_t const *>(buf);
//            _parent._callback->onRead(code, uv_buf->base, static_cast<std::size_t>(nread));
//        }
//    }

//    virtual void onWrite(/*uv_write_t*/void * req, int status) override
//    {
//        Tcp::WriteErrorCode code;
//        if (status == 0) {
//            code = Tcp::WriteErrorCode::SUCCESS;
//        } else {
//            code = Tcp::WriteErrorCode::UNKNOWN_ERROR;
//        }
//
//        if (_parent._callback != nullptr) {
//            _parent._callback->onWrite(code);
//        }
//    }

// ------------------------
// TcpPimpl implementation.
// ------------------------

/**
 * Pointer to implementation of @c uv_tcp_t.
 *
 * @author zer0
 * @date   2016-11-03
 * @date   2016-11-07 (Refactoring this class)
 *
 * @remarks
 *  Use the libuv.
 */
struct Tcp::TcpPimpl
{
    uv_tcp_t tcp;

    TcpPimpl()
    {
        ::memset(&tcp, 0x00, sizeof(tcp));
    }

    ~TcpPimpl()
    {
        // EMPTY.
    }

    inline uv_tcp_t * get() TBAG_NOEXCEPT
    { return &tcp; }
    inline uv_tcp_t const * get() const TBAG_NOEXCEPT
    { return &tcp; }

    inline std::string getPeerName() const
    { return getPeerName(&tcp); }
    inline std::string getSocketName() const
    { return getSocketName(&tcp); }

public:
    static std::string getIpName(sockaddr_in * address)
    {
        char name[16] = {0,}; // e.g. 255.255.255.255
        if (::uv_ip4_name(address, name, sizeof(name)) == 0) {
            return std::string(name);
        }
        return std::string();
    }

    static std::string getIpName(sockaddr_in6 * address)
    {
        char name[40] = {0,}; // e.g. 2001:0db8:85a3:08d3:1319:8a2e:0370:7334
        if (::uv_ip6_name(address, name, sizeof(name)) == 0) {
            return std::string(name);
        }
        return std::string();
    }

    static std::string getPeerName(uv_tcp_t const * handle)
    {
        sockaddr addr = {0,};
        int length = sizeof(addr);

        if (::uv_tcp_getpeername(handle, &addr, &length) == 0) {
            if (addr.sa_family == AF_INET) {
                return getIpName((sockaddr_in*)&addr);
            } else if (addr.sa_family == AF_INET6) {
                return getIpName((sockaddr_in6*)&addr);
            }
        }
        return std::string();
    }

    static std::string getSocketName(uv_tcp_t const * handle)
    {
        sockaddr addr = {0,};
        int length = sizeof(addr);

        if (::uv_tcp_getsockname(handle, &addr, &length) == 0) {
            if (addr.sa_family == AF_INET) {
                return getIpName((sockaddr_in*)&addr);
            } else if (addr.sa_family == AF_INET6) {
                return getIpName((sockaddr_in6*)&addr);
            }
        }
        return std::string();
    }
};

// -------------------
// Tcp implementation.
// -------------------

Tcp::Tcp() : _tcp(new TcpPimpl())
{
    assert(_tcp != nullptr);
}

Tcp::~Tcp()
{
    // EMPTY.
}

bool Tcp::init(libtbag::loop::UvEventLoop & loop)
{
    int const ERROR_CODE = ::uv_tcp_init(static_cast<uv_loop_t*>(loop.getNative()), _tcp->get());
    if (ERROR_CODE != 0) {
        __tbag_error_f("Tcp init error: {}", ERROR_CODE);
        return false;
    }
    return true;
}

void * Tcp::getNative()
{
    return _tcp->get();
}

void const * Tcp::getNative() const
{
    return _tcp->get();
}

std::string Tcp::getPeerName() const
{
    return _tcp->getPeerName();
}

std::string Tcp::getSocketName() const
{
    return _tcp->getSocketName();
}

bool Tcp::isIpv4(std::string const & address)
{
    auto tokens = string::splitTokens(address, ".");
    if (tokens.size() != 3) {
        return false;
    }

    for (auto cursor : tokens) {
        if (!std::all_of(cursor.begin(), cursor.end(), ::isdigit)) {
            return false;
        }
    }
    return true;
}

bool isIpv6(std::string const & address)
{
    return !Tcp::isIpv4(address);
}

} // namespace socket
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

