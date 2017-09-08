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

#include <libtbag/proto/fbs/tpot_generated.h>
#include <libtbag/network/http/HttpProperty.hpp>
#include <libtbag/util/ProcInfo.hpp>
#include <libtbag/Unit.hpp>

#include <cstdint>
#include <vector>
#include <string>
#include <functional>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace proto {

struct TpotPacketTypes : private Noncopyable
{
    using FlatBuilder = flatbuffers::FlatBufferBuilder;

    using ResultCode  = proto::fbs::tpot::ResultCode;
    using Header      = proto::fbs::tpot::Header;
    using ProcessInfo = proto::fbs::tpot::ProcessInfo;

    using VersionRequest   = proto::fbs::tpot::VersionRequest;
    using VersionResponse  = proto::fbs::tpot::VersionResponse;
    using ExecRequest      = proto::fbs::tpot::ExecRequest;
    using ExecResponse     = proto::fbs::tpot::ExecResponse;
    using HeartbitRequest  = proto::fbs::tpot::HeartbitRequest;
    using HeartbitResponse = proto::fbs::tpot::HeartbitResponse;
    using ListRequest      = proto::fbs::tpot::ListRequest;
    using ListResponse     = proto::fbs::tpot::ListResponse;
    using KillRequest      = proto::fbs::tpot::KillRequest;
    using KillResponse     = proto::fbs::tpot::KillResponse;

    using ProcInfo = util::ProcInfo;

    TBAG_CONSTEXPR static std::size_t const DEFAULT_BUILDER_CAPACITY = 1 * MEGA_BYTE_TO_BYTE;
    TBAG_CONSTEXPR static char const * const DEFAULT_ECHO_MESSAGE = "TPOT";
};

/**
 * TpotPacketBuilder class prototype.
 *
 * @author zer0
 * @date   2017-09-08
 */
class TBAG_API TpotPacketBuilder : public TpotPacketTypes
{
private:
    FlatBuilder _builder;

public:
    TpotPacketBuilder(std::size_t capacity = DEFAULT_BUILDER_CAPACITY);
    virtual ~TpotPacketBuilder();

public:
    uint8_t * point() const;
    std::size_t size() const;

public:
    void clear();

public:
    Err buildVersionRequest(uint64_t id = genId(), ResultCode code = proto::fbs::tpot::ResultCode_SUCCESS);
    Err buildVersionResponse(unsigned major = LIBTBAG_VERSION_PACKET_MAJOR,
                             unsigned minor = LIBTBAG_VERSION_PACKET_MINOR,
                             uint64_t id = genId(), ResultCode code = proto::fbs::tpot::ResultCode_SUCCESS);

    Err buildExecRequest(std::string const & file,
                         std::vector<std::string> const & args = std::vector<std::string>(),
                         std::vector<std::string> const & envs = std::vector<std::string>(),
                         std::string const & cwd = std::string(),
                         std::string const & input = std::string(),
                         uint64_t id = genId(), ResultCode code = proto::fbs::tpot::ResultCode_SUCCESS);
    Err buildExecResponse(int pid, uint64_t id = genId(), ResultCode code = proto::fbs::tpot::ResultCode_SUCCESS);

    Err buildHeartbitRequest(std::string const & echo = std::string(DEFAULT_ECHO_MESSAGE),
                             uint64_t id = genId(), ResultCode code = proto::fbs::tpot::ResultCode_SUCCESS);
    Err buildHeartbitResponse(std::string const & echo = std::string(DEFAULT_ECHO_MESSAGE),
                              uint64_t id = genId(), ResultCode code = proto::fbs::tpot::ResultCode_SUCCESS);

    Err buildListRequest(uint64_t id = genId(), ResultCode code = proto::fbs::tpot::ResultCode_SUCCESS);
    Err buildListResponse(std::vector<ProcInfo> const & procs,
                          uint64_t id = genId(), ResultCode code = proto::fbs::tpot::ResultCode_SUCCESS);

    Err buildKillRequest(int pid, uint64_t id = genId(), ResultCode code = proto::fbs::tpot::ResultCode_SUCCESS);
    Err buildKillResponse(uint64_t id = genId(), ResultCode code = proto::fbs::tpot::ResultCode_SUCCESS);

public:
    static uint64_t genId();
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
    TpotPacketParser();
    virtual ~TpotPacketParser();

public:
    Err parse(char const * buffer, std::size_t size, void * arg = nullptr);

protected:
    virtual void onVersionRequest  (Header const & header, VersionRequest   const & packet, void * arg) { /* EMPTY. */ }
    virtual void onVersionResponse (Header const & header, VersionResponse  const & packet, void * arg) { /* EMPTY. */ }
    virtual void onExecRequest     (Header const & header, ExecRequest      const & packet, void * arg) { /* EMPTY. */ }
    virtual void onExecResponse    (Header const & header, ExecResponse     const & packet, void * arg) { /* EMPTY. */ }
    virtual void onHeartbitRequest (Header const & header, HeartbitRequest  const & packet, void * arg) { /* EMPTY. */ }
    virtual void onHeartbitResponse(Header const & header, HeartbitResponse const & packet, void * arg) { /* EMPTY. */ }
    virtual void onListRequest     (Header const & header, ListRequest      const & packet, void * arg) { /* EMPTY. */ }
    virtual void onListResponse    (Header const & header, ListResponse     const & packet, void * arg) { /* EMPTY. */ }
    virtual void onKillRequest     (Header const & header, KillRequest      const & packet, void * arg) { /* EMPTY. */ }
    virtual void onKillResponse    (Header const & header, KillResponse     const & packet, void * arg) { /* EMPTY. */ }
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

/**
 * FunctionalTpotPacket class prototype.
 *
 * @author zer0
 * @date   2017-09-06
 */
class FunctionalTpotPacket : public TpotPacket
{
public:
    // @formatter:off
    using ResultCode       = TpotPacket::ResultCode;
    using Header           = TpotPacket::Header;
    using VersionRequest   = TpotPacket::VersionRequest;
    using VersionResponse  = TpotPacket::VersionResponse;
    using ExecRequest      = TpotPacket::ExecRequest;
    using ExecResponse     = TpotPacket::ExecResponse;
    using HeartbitRequest  = TpotPacket::HeartbitRequest;
    using HeartbitResponse = TpotPacket::HeartbitResponse;
    using ListRequest      = TpotPacket::ListRequest;
    using ListResponse     = TpotPacket::ListResponse;
    using KillRequest      = TpotPacket::KillRequest;
    using KillResponse     = TpotPacket::KillResponse;
    // @formatter:on

public:
    // @formatter:off
    using OnVersionRequest   = std::function<void(Header const &, VersionRequest   const &, void*)>;
    using OnVersionResponse  = std::function<void(Header const &, VersionResponse  const &, void*)>;
    using OnExecRequest      = std::function<void(Header const &, ExecRequest      const &, void*)>;
    using OnExecResponse     = std::function<void(Header const &, ExecResponse     const &, void*)>;
    using OnHeartbitRequest  = std::function<void(Header const &, HeartbitRequest  const &, void*)>;
    using OnHeartbitResponse = std::function<void(Header const &, HeartbitResponse const &, void*)>;
    using OnListRequest      = std::function<void(Header const &, ListRequest      const &, void*)>;
    using OnListResponse     = std::function<void(Header const &, ListResponse     const &, void*)>;
    using OnKillRequest      = std::function<void(Header const &, KillRequest      const &, void*)>;
    using OnKillResponse     = std::function<void(Header const &, KillResponse     const &, void*)>;
    // @formatter:on

private:
    // @formatter:off
    OnVersionRequest   _version_request_cb;
    OnVersionResponse  _version_response_cb;
    OnExecRequest      _exec_request_cb;
    OnExecResponse     _exec_response_cb;
    OnHeartbitRequest  _heartbit_request_cb;
    OnHeartbitResponse _heartbit_response_cb;
    OnListRequest      _list_request_cb;
    OnListResponse     _list_response_cb;
    OnKillRequest      _kill_request_cb;
    OnKillResponse     _kill_response_cb;
    // @formatter:on

public:
    FunctionalTpotPacket(std::size_t capacity = DEFAULT_BUILDER_CAPACITY) : TpotPacket(capacity)
    { /* EMPTY. */ }
    virtual ~FunctionalTpotPacket()
    { /* EMPTY. */ }

public:
    // @formatter:off
    void setOnVersionRequest  (OnVersionRequest   const & cb) { _version_request_cb   = cb; }
    void setOnVersionResponse (OnVersionResponse  const & cb) { _version_response_cb  = cb; }
    void setOnExecRequest     (OnExecRequest      const & cb) { _exec_request_cb      = cb; }
    void setOnExecResponse    (OnExecResponse     const & cb) { _exec_response_cb     = cb; }
    void setOnHeartbitRequest (OnHeartbitRequest  const & cb) { _heartbit_request_cb  = cb; }
    void setOnHeartbitResponse(OnHeartbitResponse const & cb) { _heartbit_response_cb = cb; }
    void setOnListRequest     (OnListRequest      const & cb) { _list_request_cb      = cb; }
    void setOnListResponse    (OnListResponse     const & cb) { _list_response_cb     = cb; }
    void setOnKillRequest     (OnKillRequest      const & cb) { _kill_request_cb      = cb; }
    void setOnKillResponse    (OnKillResponse     const & cb) { _kill_response_cb     = cb; }
    // @formatter:on

public:
    // @formatter:off
    virtual void onVersionRequest  (Header const & header, VersionRequest   const & packet, void * arg) { if (static_cast<bool>(_version_request_cb  )) { _version_request_cb  (header, packet, arg); } }
    virtual void onVersionResponse (Header const & header, VersionResponse  const & packet, void * arg) { if (static_cast<bool>(_version_response_cb )) { _version_response_cb (header, packet, arg); } }
    virtual void onExecRequest     (Header const & header, ExecRequest      const & packet, void * arg) { if (static_cast<bool>(_exec_request_cb     )) { _exec_request_cb     (header, packet, arg); } }
    virtual void onExecResponse    (Header const & header, ExecResponse     const & packet, void * arg) { if (static_cast<bool>(_exec_response_cb    )) { _exec_response_cb    (header, packet, arg); } }
    virtual void onHeartbitRequest (Header const & header, HeartbitRequest  const & packet, void * arg) { if (static_cast<bool>(_heartbit_request_cb )) { _heartbit_request_cb (header, packet, arg); } }
    virtual void onHeartbitResponse(Header const & header, HeartbitResponse const & packet, void * arg) { if (static_cast<bool>(_heartbit_response_cb)) { _heartbit_response_cb(header, packet, arg); } }
    virtual void onListRequest     (Header const & header, ListRequest      const & packet, void * arg) { if (static_cast<bool>(_list_request_cb     )) { _list_request_cb     (header, packet, arg); } }
    virtual void onListResponse    (Header const & header, ListResponse     const & packet, void * arg) { if (static_cast<bool>(_list_response_cb    )) { _list_response_cb    (header, packet, arg); } }
    virtual void onKillRequest     (Header const & header, KillRequest      const & packet, void * arg) { if (static_cast<bool>(_kill_request_cb     )) { _kill_request_cb     (header, packet, arg); } }
    virtual void onKillResponse    (Header const & header, KillResponse     const & packet, void * arg) { if (static_cast<bool>(_kill_response_cb    )) { _kill_response_cb    (header, packet, arg); } }
    // @formatter:on
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

_TPOT_CREATE_PATH_STRUCTURE( VersionPath, "ver" ,    GET)
_TPOT_CREATE_PATH_STRUCTURE(    ExecPath, "exec",    PUT)
_TPOT_CREATE_PATH_STRUCTURE(HeartbitPath, "hbit",    GET)
_TPOT_CREATE_PATH_STRUCTURE(    ListPath, "list",    GET)
_TPOT_CREATE_PATH_STRUCTURE(    KillPath, "kill", DELETE)

#undef _TPOT_CREATE_PATH_STRUCTURE

TBAG_CONSTEXPR char const * const getAcceptKey  () TBAG_NOEXCEPT { return "Accept"; }
TBAG_CONSTEXPR char const * const getAcceptValue() TBAG_NOEXCEPT { return "application/octet-stream"; }

} // namespace proto

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PROTO_TPOTPACKET_HPP__

