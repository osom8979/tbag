/**
 * @file   TpotPacket.hpp
 * @brief  TpotPacket class prototype.
 * @author zer0
 * @date   2017-09-06
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PROTO_TPOTPACKET_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PROTO_TPOTPACKET_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/Unit.hpp>

#include <libtbag/network/http/HttpProperty.hpp>
#include <libtbag/util/Structures.hpp>

#include <cstdint>
#include <vector>
#include <string>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace proto {

struct TpotPacketTypes : private Noncopyable
{
    enum class ResultCode : int
    {
        RC_SUCCESS = 0,
        RC_UNKNOWN_ERROR,
        RC_EXECUTE_ERROR,
        RC_NOT_EXISTS,
        RC_KILL_ERROR,
    };

    TBAG_CONSTEXPR static ResultCode const SUCCESS_CODE = ResultCode::RC_SUCCESS;
};

/**
 * TpotPacketBuilder class prototype.
 *
 * @author zer0
 * @date   2017-09-08
 */
class TBAG_API TpotPacketBuilder : public TpotPacketTypes
{
public:
    class Internal;
    friend class Internal;
    using UniqueInternal = std::unique_ptr<Internal>;

public:
    TBAG_CONSTEXPR static std::size_t const DEFAULT_BUILDER_CAPACITY = 1 * MEGA_BYTE_TO_BYTE;
    TBAG_CONSTEXPR static char const * const DEFAULT_ECHO_MESSAGE = "TPOT";

private:
    UniqueInternal _internal;

public:
    TpotPacketBuilder(std::size_t capacity = DEFAULT_BUILDER_CAPACITY);
    virtual ~TpotPacketBuilder();

public:
    uint8_t * point() const;
    std::size_t size() const;

public:
    Err buildVersionRequest(util::Header const & header);
    Err buildVersionResponse(util::Header const & header,
                             unsigned major = LIBTBAG_VERSION_PACKET_MAJOR,
                             unsigned minor = LIBTBAG_VERSION_PACKET_MINOR,
                             util::Pairs const & features = util::Pairs());

    Err buildEchoRequest(util::Header const & header, std::string const & message = DEFAULT_ECHO_MESSAGE);
    Err buildEchoResponse(util::Header const & header, std::string const & message = DEFAULT_ECHO_MESSAGE);

    Err buildLoginRequest(util::Header const & header, std::string const & id, std::string const & pw);
    Err buildLoginResponse(util::Header const & header, std::string const & key);

    Err buildLogoutRequest(util::Header const & header);
    Err buildLogoutResponse(util::Header const & header);

    Err buildExecRequest(util::Header const & header,
                         std::string const & file,
                         std::vector<std::string> const & args = std::vector<std::string>(),
                         std::vector<std::string> const & envs = std::vector<std::string>(),
                         std::string const & cwd = std::string(),
                         std::string const & input = std::string());
    Err buildExecResponse(util::Header const & header, int pid);

    Err buildProcessListRequest(util::Header const & header);
    Err buildProcessListResponse(util::Header const & header, std::vector<util::ProcessInfo> const & procs);

    Err buildProcessKillRequest(util::Header const & header, int pid, int signum);
    Err buildProcessKillResponse(util::Header const & header);

    Err buildProcessRemoveRequest(util::Header const & header, int pid);
    Err buildProcessRemoveResponse(util::Header const & header);
};

/**
 * TpotPacketParser class prototype.
 *
 * @author zer0
 * @date   2017-09-08
 */
class TBAG_API TpotPacketParser : public TpotPacketTypes
{
public:
    class Internal;
    friend class Internal;
    using UniqueInternal = std::unique_ptr<Internal>;

private:
    UniqueInternal _internal;

public:
    TpotPacketParser();
    virtual ~TpotPacketParser();

public:
    Err parse(char const * buffer, std::size_t size);

protected:
    virtual void onVersionRequest       (util::Header const & header) { /* EMPTY. */ }
    virtual void onVersionResponse      (util::Header const & header, unsigned major, unsigned minor, util::Pairs const & features) { /* EMPTY. */ }
    virtual void onEchoRequest          (util::Header const & header, std::string const & message) { /* EMPTY. */ }
    virtual void onEchoResponse         (util::Header const & header, std::string const & message) { /* EMPTY. */ }
    virtual void onLoginRequest         (util::Header const & header, std::string const & id, std::string const & pw) { /* EMPTY. */ }
    virtual void onLoginResponse        (util::Header const & header, std::string const & key) { /* EMPTY. */ }
    virtual void onLogoutRequest        (util::Header const & header) { /* EMPTY. */ }
    virtual void onLogoutResponse       (util::Header const & header) { /* EMPTY. */ }
    virtual void onExecRequest          (util::Header const & header,
                                         std::string const & file,
                                         std::vector<std::string> const & args,
                                         std::vector<std::string> const & envs,
                                         std::string const & cwd,
                                         std::string const & input) { /* EMPTY. */ }
    virtual void onExecResponse         (util::Header const & header, int pid) { /* EMPTY. */ }
    virtual void onProcessListRequest   (util::Header const & header) { /* EMPTY. */ }
    virtual void onProcessListResponse  (util::Header const & header, std::vector<util::ProcessInfo> const & procs) { /* EMPTY. */ }
    virtual void onProcessKillRequest   (util::Header const & header, int pid, int signum) { /* EMPTY. */ }
    virtual void onProcessKillResponse  (util::Header const & header) { /* EMPTY. */ }
    virtual void onProcessRemoveRequest (util::Header const & header, int pid) { /* EMPTY. */ }
    virtual void onProcessRemoveResponse(util::Header const & header) { /* EMPTY. */ }
};

/**
 * TpotPacket class prototype.
 *
 * @author zer0
 * @date   2017-09-06
 */
class TBAG_API TpotPacket : public TpotPacketBuilder, public TpotPacketParser
{
public:
    TpotPacket(std::size_t capacity = DEFAULT_BUILDER_CAPACITY);
    virtual ~TpotPacket();
};

// --------------------
// HTTP Path structure.
// --------------------

#ifndef _TPOT_CREATE_PATH_STRUCTURE
#define _TPOT_CREATE_PATH_STRUCTURE(class_name, path_name, method) \
    struct class_name : public libtbag::network::http::HttpMethod##method \
    { \
        TBAG_CONSTEXPR static char const * const getName() TBAG_NOEXCEPT { return     path_name; } \
        TBAG_CONSTEXPR static char const * const getPath() TBAG_NOEXCEPT { return "/" path_name; } \
    };
#endif

_TPOT_CREATE_PATH_STRUCTURE(      VersionPath,   "ver" ,    GET)
_TPOT_CREATE_PATH_STRUCTURE(         EchoPath,   "echo",    GET)
_TPOT_CREATE_PATH_STRUCTURE(        LoginPath,  "login",   POST)
_TPOT_CREATE_PATH_STRUCTURE(       LogoutPath, "logout",   POST)
_TPOT_CREATE_PATH_STRUCTURE(         ExecPath,   "exec",    PUT)
_TPOT_CREATE_PATH_STRUCTURE(  ProcessListPath,   "list",    GET)
_TPOT_CREATE_PATH_STRUCTURE(  ProcessKillPath,   "kill",   POST)
_TPOT_CREATE_PATH_STRUCTURE(ProcessRemovePath, "remove", DELETE)

#undef _TPOT_CREATE_PATH_STRUCTURE

TBAG_CONSTEXPR char const * const getAcceptKey  () TBAG_NOEXCEPT { return "Accept"; }
TBAG_CONSTEXPR char const * const getAcceptValue() TBAG_NOEXCEPT { return "application/octet-stream"; }

TBAG_CONSTEXPR int const DEFAULT_TPOT_SERVER_PORT_NUMBER = 7907; ///< "TPOT" -> "7907"
TBAG_CONSTEXPR uint64_t const DEFAULT_TPOT_TIMEOUT_MILLISEC = 15 * 1000;

} // namespace proto

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PROTO_TPOTPACKET_HPP__

