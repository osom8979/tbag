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
#include <libtbag/Unit.hpp>

#include <cstdint>
#include <vector>
#include <string>
#include <functional>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace proto {

/**
 * TpotPacket class prototype.
 *
 * @author zer0
 * @date   2017-09-06
 */
class TBAG_API TpotPacket : private Noncopyable
{
public:
    using FlatBuilder = flatbuffers::FlatBufferBuilder;

    using Code   = proto::fbs::tpot::ResultCode;
    using Header = proto::fbs::tpot::Header;

    using PacketVersionRequest  = proto::fbs::tpot::PacketVersionRequest;
    using PacketVersionResponse = proto::fbs::tpot::PacketVersionResponse;
    using ExecRequest           = proto::fbs::tpot::ExecRequest;
    using ExecResponse          = proto::fbs::tpot::ExecResponse;
    using HeartbitRequest       = proto::fbs::tpot::HeartbitRequest;
    using HeartbitResponse      = proto::fbs::tpot::HeartbitResponse;
    using ListRequest           = proto::fbs::tpot::ListRequest;
    using ListResponse          = proto::fbs::tpot::ListResponse;
    using KillRequest           = proto::fbs::tpot::KillRequest;
    using KillResponse          = proto::fbs::tpot::KillResponse;

public:
    TBAG_CONSTEXPR static std::size_t const DEFAULT_BUILDER_CAPACITY = 1 * MEGA_BYTE_TO_BYTE;
    TBAG_CONSTEXPR static char const * const DEFAULT_ECHO_MESSAGE = "TPOT";

private:
    FlatBuilder _builder;

public:
    TpotPacket(std::size_t capacity = DEFAULT_BUILDER_CAPACITY);
    virtual ~TpotPacket();

public:
    void clear();

public:
    Err buildPacketVersionRequest (Code code = proto::fbs::tpot::ResultCode_SUCCESS);
    Err buildPacketVersionResponse(unsigned major = LIBTBAG_VERSION_PACKET_MAJOR,
                                   unsigned minor = LIBTBAG_VERSION_PACKET_MINOR,
                                   Code code = proto::fbs::tpot::ResultCode_SUCCESS);

    Err buildExecRequest (std::string const & file,
                          std::vector<std::string> const & args = std::vector<std::string>(),
                          std::vector<std::string> const & envs = std::vector<std::string>(),
                          std::string const & cwd = std::string(),
                          std::string const & input = std::string(),
                          Code code = proto::fbs::tpot::ResultCode_SUCCESS);
    Err buildExecResponse(int pid, Code code = proto::fbs::tpot::ResultCode_SUCCESS);

    Err buildHeartbitRequest (std::string const & echo = std::string(DEFAULT_ECHO_MESSAGE),
                              Code code = proto::fbs::tpot::ResultCode_SUCCESS);
    Err buildHeartbitResponse(std::string const & echo = std::string(DEFAULT_ECHO_MESSAGE),
                              Code code = proto::fbs::tpot::ResultCode_SUCCESS);

    Err buildListRequest (Code code = proto::fbs::tpot::ResultCode_SUCCESS);
    Err buildListResponse(std::vector<int> const & pids, Code code = proto::fbs::tpot::ResultCode_SUCCESS);

    Err buildKillRequest (int pid, Code code = proto::fbs::tpot::ResultCode_SUCCESS);
    Err buildKillResponse(Code code = proto::fbs::tpot::ResultCode_SUCCESS);

public:
    std::string toJsonString();

public:
    Err parse(char const * buffer, std::size_t size);

protected:
    virtual void onPacketVersionRequest (Header const & header, PacketVersionRequest  const & packet) { /* EMPTY. */ }
    virtual void onPacketVersionResponse(Header const & header, PacketVersionResponse const & packet) { /* EMPTY. */ }
    virtual void onExecRequest          (Header const & header, ExecRequest           const & packet) { /* EMPTY. */ }
    virtual void onExecResponse         (Header const & header, ExecResponse          const & packet) { /* EMPTY. */ }
    virtual void onHeartbitRequest      (Header const & header, HeartbitRequest       const & packet) { /* EMPTY. */ }
    virtual void onHeartbitResponse     (Header const & header, HeartbitResponse      const & packet) { /* EMPTY. */ }
    virtual void onListRequest          (Header const & header, ListRequest           const & packet) { /* EMPTY. */ }
    virtual void onListResponse         (Header const & header, ListResponse          const & packet) { /* EMPTY. */ }
    virtual void onKillRequest          (Header const & header, KillRequest           const & packet) { /* EMPTY. */ }
    virtual void onKillResponse         (Header const & header, KillResponse          const & packet) { /* EMPTY. */ }

public:
    uint8_t * point() const;
    std::size_t size() const;
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
    using Code                  = TpotPacket::Code;
    using Header                = TpotPacket::Header;
    using PacketVersionRequest  = TpotPacket::PacketVersionRequest;
    using PacketVersionResponse = TpotPacket::PacketVersionResponse;
    using ExecRequest           = TpotPacket::ExecRequest;
    using ExecResponse          = TpotPacket::ExecResponse;
    using HeartbitRequest       = TpotPacket::HeartbitRequest;
    using HeartbitResponse      = TpotPacket::HeartbitResponse;
    using ListRequest           = TpotPacket::ListRequest;
    using ListResponse          = TpotPacket::ListResponse;
    using KillRequest           = TpotPacket::KillRequest;
    using KillResponse          = TpotPacket::KillResponse;
    // @formatter:on

public:
    // @formatter:off
    using OnPacketVersionRequest  = std::function<void(Header const &, PacketVersionRequest  const &)>;
    using OnPacketVersionResponse = std::function<void(Header const &, PacketVersionResponse const &)>;
    using OnExecRequest           = std::function<void(Header const &, ExecRequest           const &)>;
    using OnExecResponse          = std::function<void(Header const &, ExecResponse          const &)>;
    using OnHeartbitRequest       = std::function<void(Header const &, HeartbitRequest       const &)>;
    using OnHeartbitResponse      = std::function<void(Header const &, HeartbitResponse      const &)>;
    using OnListRequest           = std::function<void(Header const &, ListRequest           const &)>;
    using OnListResponse          = std::function<void(Header const &, ListResponse          const &)>;
    using OnKillRequest           = std::function<void(Header const &, KillRequest           const &)>;
    using OnKillResponse          = std::function<void(Header const &, KillResponse          const &)>;
    // @formatter:on

private:
    // @formatter:off
    OnPacketVersionRequest  _packet_version_request_cb;
    OnPacketVersionResponse _packet_version_response_cb;
    OnExecRequest           _exec_request_cb;
    OnExecResponse          _exec_response_cb;
    OnHeartbitRequest       _heartbit_request_cb;
    OnHeartbitResponse      _heartbit_response_cb;
    OnListRequest           _list_request_cb;
    OnListResponse          _list_response_cb;
    OnKillRequest           _kill_request_cb;
    OnKillResponse          _kill_response_cb;
    // @formatter:on

public:
    FunctionalTpotPacket(std::size_t capacity = DEFAULT_BUILDER_CAPACITY) : TpotPacket(capacity)
    { /* EMPTY. */ }
    virtual ~FunctionalTpotPacket()
    { /* EMPTY. */ }

public:
    // @formatter:off
    void setOnPacketVersionRequest (OnPacketVersionRequest  const & cb) { _packet_version_request_cb    = cb; }
    void setOnPacketVersionResponse(OnPacketVersionResponse const & cb) { _packet_version_response_cb   = cb; }
    void setOnExecRequest          (OnExecRequest           const & cb) { _exec_request_cb              = cb; }
    void setOnExecResponse         (OnExecResponse          const & cb) { _exec_response_cb             = cb; }
    void setOnHeartbitRequest      (OnHeartbitRequest       const & cb) { _heartbit_request_cb          = cb; }
    void setOnHeartbitResponse     (OnHeartbitResponse      const & cb) { _heartbit_response_cb         = cb; }
    void setOnListRequest          (OnListRequest           const & cb) { _list_request_cb              = cb; }
    void setOnListResponse         (OnListResponse          const & cb) { _list_response_cb             = cb; }
    void setOnKillRequest          (OnKillRequest           const & cb) { _kill_request_cb              = cb; }
    void setOnKillResponse         (OnKillResponse          const & cb) { _kill_response_cb             = cb; }
    // @formatter:on

public:
    // @formatter:off
    virtual void onPacketVersionRequest (Header const & header, PacketVersionRequest  const & packet) { if (static_cast<bool>(_packet_version_request_cb )) { _packet_version_request_cb (header, packet); } }
    virtual void onPacketVersionResponse(Header const & header, PacketVersionResponse const & packet) { if (static_cast<bool>(_packet_version_response_cb)) { _packet_version_response_cb(header, packet); } }
    virtual void onExecRequest          (Header const & header, ExecRequest           const & packet) { if (static_cast<bool>(_exec_request_cb           )) { _exec_request_cb           (header, packet); } }
    virtual void onExecResponse         (Header const & header, ExecResponse          const & packet) { if (static_cast<bool>(_exec_response_cb          )) { _exec_response_cb          (header, packet); } }
    virtual void onHeartbitRequest      (Header const & header, HeartbitRequest       const & packet) { if (static_cast<bool>(_heartbit_request_cb       )) { _heartbit_request_cb       (header, packet); } }
    virtual void onHeartbitResponse     (Header const & header, HeartbitResponse      const & packet) { if (static_cast<bool>(_heartbit_response_cb      )) { _heartbit_response_cb      (header, packet); } }
    virtual void onListRequest          (Header const & header, ListRequest           const & packet) { if (static_cast<bool>(_list_request_cb           )) { _list_request_cb           (header, packet); } }
    virtual void onListResponse         (Header const & header, ListResponse          const & packet) { if (static_cast<bool>(_list_response_cb          )) { _list_response_cb          (header, packet); } }
    virtual void onKillRequest          (Header const & header, KillRequest           const & packet) { if (static_cast<bool>(_kill_request_cb           )) { _kill_request_cb           (header, packet); } }
    virtual void onKillResponse         (Header const & header, KillResponse          const & packet) { if (static_cast<bool>(_kill_response_cb          )) { _kill_response_cb          (header, packet); } }
    // @formatter:on
};

} // namespace proto

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PROTO_TPOTPACKET_HPP__

