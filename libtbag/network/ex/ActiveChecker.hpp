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
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/util/Version.hpp>

#include <memory>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

// Forward declaration.
namespace uvpp { class Loop; }

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

public:
    ActiveChecker(Loop & loop, std::string const & ip, int port, Response const & response);
    ActiveChecker(Loop & loop, std::string const & ip, int port, int broadcast_port, uint64_t timeout, Request const & request);
    virtual ~ActiveChecker();

public:
    void close();

public:
    virtual void onResponse(EndPointType type, std::string const & ip, int port);
    virtual void onClose();
};

} // namespace ex
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_EX_ACTIVECHECKER_HPP__

