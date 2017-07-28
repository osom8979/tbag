/**
 * @file   ActiveChecker.hpp
 * @brief  ActiveChecker class prototype.
 * @author zer0
 * @date   2017-07-27
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_EX_ACTIVECHECKER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_EX_ACTIVECHECKER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/Err.hpp>

#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/util/Version.hpp>

#include <cstdint>
#include <memory>
#include <string>
#include <functional>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace ex      {

/**
 * ActiveChecker class prototype.
 *
 * @author zer0
 * @date   2017-07-27
 */
class TBAG_API ActiveChecker : private Noncopyable
{
public:
    using Loop    = libtbag::uvpp::Loop;
    using Version = libtbag::util::Version;

public:
    class Internal;
    friend class Internal;
    using UniqueInternal = std::unique_ptr<Internal>;

public:
    enum class EndPointType : int
    {
        EPT_UNKNOWN = 0,
        EPT_AMERICANO,
        EPT_ROASTER,
        EPT_MUGCUP,
    };

    TBAG_CONSTEXPR static char const * const VERSION_KEY       = "version";
    TBAG_CONSTEXPR static char const * const RECV_IP_KEY       = "recv_ip";
    TBAG_CONSTEXPR static char const * const RECV_PORT_KEY     = "recv_port";
    TBAG_CONSTEXPR static char const * const SERVER_TYPE_KEY   = "server_type";
    TBAG_CONSTEXPR static char const * const SERVER_IP_KEY     = "server_ip";
    TBAG_CONSTEXPR static char const * const SERVER_PORT_KEY   = "server_port";

    struct TBAG_API Request
    {
        Version     version;
        std::string recv_ip;
        int         recv_port;

        Request();
        ~Request();

        std::string toJsonString() const;
        bool fromJsonString(std::string const & json);
    };

    struct TBAG_API Response
    {
        Version      version;
        EndPointType server_type;
        std::string  server_ip;
        int          server_port;

        Response();
        ~Response();

        std::string toJsonString() const;
        bool fromJsonString(std::string const & json);
    };

private:
    UniqueInternal _internal;
    bool _is_init;

public:
    ActiveChecker();

    /**
     * Initialize server socket.
     */
    ActiveChecker(Loop & loop, std::string const & ip, int port, Response const & response);

    /**
     * Initialize client socket.
     */
    ActiveChecker(Loop & loop, std::string const & ip, int port,
                  uint64_t timeout, Request const & request);
    ActiveChecker(Loop & loop, std::string const & ip, int port,
                  uint64_t timeout, Version const & version);

    virtual ~ActiveChecker();

public:
    inline bool isInit() const TBAG_NOEXCEPT { return _is_init; }

public:
    std::string getRecvIp() const;
    int getRecvPort() const;

public:
    bool initServer(Loop & loop, std::string const & ip, int port, Response const & response);
    bool initClient(Loop & loop, std::string const & ip, int port,
                    uint64_t timeout, Request const & request);
    bool initClient(Loop & loop, std::string const & ip, int port,
                    uint64_t timeout, Version const & version);

public:
    void setJsonString(std::string const & json);
    bool broadcast(int port);
    void close();

public:
    virtual void onResponse(EndPointType type, std::string const & ip, int port);
    virtual void onTimeout();
    virtual void onClose();
};

/**
 * FunctionalActiveChecker class prototype.
 *
 * @author zer0
 * @date   2017-07-28
 */
class FunctionalActiveChecker : public ActiveChecker
{
public:
    using EndPointType = ActiveChecker::EndPointType;
    using Request      = ActiveChecker::Request;
    using Response     = ActiveChecker::Response;
    using Loop         = ActiveChecker::Loop;
    using Version      = ActiveChecker::Version;

    using OnResponse = std::function<void(EndPointType, std::string const &, int)>;
    using OnTimeout  = std::function<void(void)>;
    using OnClose    = std::function<void(void)>;

private:
    OnResponse _on_response_cb;
    OnTimeout  _on_timeout_cb;
    OnClose    _on_close_cb;

public:
    FunctionalActiveChecker() : ActiveChecker()
    {
        // EMPTY.
    }

    FunctionalActiveChecker(Loop & loop, std::string const & ip, int port, Response const & response)
            : ActiveChecker(loop, ip, port, response)
    {
        // EMPTY.
    }

    FunctionalActiveChecker(Loop & loop, std::string const & ip, int port,
                            uint64_t timeout, Request const & request)
            : ActiveChecker(loop, ip, port, timeout, request)
    {
        // EMPTY.
    }

    FunctionalActiveChecker(Loop & loop, std::string const & ip, int port,
                            uint64_t timeout, Version const & version)
            : ActiveChecker(loop, ip, port, timeout, version)
    {
        // EMPTY.
    }

public:
    void setOnResponse(OnResponse const & cb) { _on_response_cb = cb; }
    void setOnTimeout (OnTimeout  const & cb) {  _on_timeout_cb = cb; }
    void setOnClose   (OnClose    const & cb) {    _on_close_cb = cb; }

protected:
    virtual void onResponse(EndPointType type, std::string const & ip, int port) override
    {
        if (static_cast<bool>(_on_response_cb)) {
            _on_response_cb(type, ip, port);
        }
    }

    virtual void onTimeout() override
    {
        if (static_cast<bool>(_on_timeout_cb)) {
            _on_timeout_cb();
        }
    }

    virtual void onClose() override
    {
        if (static_cast<bool>(_on_close_cb)) {
            _on_close_cb();
        }
    }
};

/**
 * FuncActiveChecker typedef.
 *
 * @author zer0
 * @date   2017-07-28
 */
using FuncActiveChecker = FunctionalActiveChecker;

} // namespace ex
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_EX_ACTIVECHECKER_HPP__

