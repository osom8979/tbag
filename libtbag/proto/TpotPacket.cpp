/**
 * @file   TpotPacket.cpp
 * @brief  TpotPacket class implementation.
 * @author zer0
 * @date   2017-09-06
 */

#include <libtbag/proto/TpotPacket.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/id/generator/TimeId.hpp>
#include <libtbag/debug/Assert.hpp>

#include <flatbuffers/idl.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace proto {

TpotPacket::TpotPacket(std::size_t capacity) : _builder(capacity, nullptr)
{
    // EMPTY.
}

TpotPacket::~TpotPacket()
{
    // EMPTY.
}

void TpotPacket::clear()
{
    _builder.Clear();
}

Err TpotPacket::buildPacketVersionRequest(uint64_t id, Code code)
{
    using namespace proto::fbs::tpot;
    Header header(id, code);
    _builder.Clear();
    auto packet = CreateTpotPacket(
            _builder, &header, AnyPacket_PacketVersionRequest,
            CreatePacketVersionRequest(_builder).Union());
    _builder.Finish(packet);
    return Err::E_SUCCESS;
}

Err TpotPacket::buildPacketVersionResponse(unsigned major, unsigned minor, uint64_t id, Code code)
{
    using namespace proto::fbs::tpot;
    Header header(id, code);
    PacketVersion packet_version(major, minor);
    _builder.Clear();
    auto packet = CreateTpotPacket(
            _builder, &header, AnyPacket_PacketVersionResponse,
            CreatePacketVersionResponse(_builder, &packet_version).Union());
    _builder.Finish(packet);
    return Err::E_SUCCESS;
}

Err TpotPacket::buildExecRequest(std::string const & file,
                                 std::vector<std::string> const & args,
                                 std::vector<std::string> const & envs,
                                 std::string const & cwd,
                                 std::string const & input,
                                 uint64_t id, Code code)
{
    using namespace proto::fbs::tpot;
    Header header(id, code);
    _builder.Clear();
    auto packet = CreateTpotPacket(
            _builder, &header, AnyPacket_ExecRequest,
            CreateExecRequest(_builder,
                              _builder.CreateString(file),
                              _builder.CreateVectorOfStrings(args),
                              _builder.CreateVectorOfStrings(envs),
                              _builder.CreateString(cwd),
                              _builder.CreateString(input)).Union());
    _builder.Finish(packet);
    return Err::E_SUCCESS;
}

Err TpotPacket::buildExecResponse(int pid, uint64_t id, Code code)
{
    using namespace proto::fbs::tpot;
    Header header(id, code);
    _builder.Clear();
    auto packet = CreateTpotPacket(
            _builder, &header, AnyPacket_ExecResponse,
            CreateExecResponse(_builder, pid).Union());
    _builder.Finish(packet);
    return Err::E_SUCCESS;
}

Err TpotPacket::buildHeartbitRequest(std::string const & echo, uint64_t id, Code code)
{
    using namespace proto::fbs::tpot;
    Header header(id, code);
    _builder.Clear();
    auto packet = CreateTpotPacket(
            _builder, &header, AnyPacket_HeartbitRequest,
            CreateHeartbitRequest(_builder, _builder.CreateString(echo)).Union());
    _builder.Finish(packet);
    return Err::E_SUCCESS;
}

Err TpotPacket::buildHeartbitResponse(std::string const & echo, uint64_t id, Code code)
{
    using namespace proto::fbs::tpot;
    Header header(id, code);
    _builder.Clear();
    auto packet = CreateTpotPacket(
            _builder, &header, AnyPacket_HeartbitResponse,
            CreateHeartbitResponse(_builder, _builder.CreateString(echo)).Union());
    _builder.Finish(packet);
    return Err::E_SUCCESS;
}

Err TpotPacket::buildListRequest(uint64_t id, Code code)
{
    using namespace proto::fbs::tpot;
    Header header(id, code);
    _builder.Clear();
    auto packet = CreateTpotPacket(
            _builder, &header, AnyPacket_ListRequest,
            CreateListRequest(_builder).Union());
    _builder.Finish(packet);
    return Err::E_SUCCESS;
}

Err TpotPacket::buildListResponse(std::vector<ProcessInfo> const & procs, uint64_t id, Code code)
{
    using namespace proto::fbs::tpot;
    Header header(id, code);
    _builder.Clear();
    auto packet = CreateTpotPacket(
            _builder, &header, AnyPacket_ListResponse,
            CreateListResponse(_builder, _builder.CreateVectorOfStructs(procs)).Union());
    _builder.Finish(packet);
    return Err::E_SUCCESS;
}

Err TpotPacket::buildKillRequest(int pid, uint64_t id, Code code)
{
    using namespace proto::fbs::tpot;
    Header header(id, code);
    _builder.Clear();
    auto packet = CreateTpotPacket(
            _builder, &header, AnyPacket_KillRequest,
            CreateKillRequest(_builder, pid).Union());
    _builder.Finish(packet);
    return Err::E_SUCCESS;
}

Err TpotPacket::buildKillResponse(uint64_t id, Code code)
{
    using namespace proto::fbs::tpot;
    Header header(id, code);
    _builder.Clear();
    auto packet = CreateTpotPacket(
            _builder, &header, AnyPacket_KillResponse,
            CreateKillResponse(_builder).Union());
    _builder.Finish(packet);
    return Err::E_SUCCESS;
}

Err TpotPacket::parse(char const * buffer, std::size_t size, void * arg)
{
    using namespace flatbuffers;
    using namespace proto::fbs::tpot;
    Verifier verifier((uint8_t const *)buffer, size);
    if (VerifyTpotPacketBuffer(verifier) == false) {
        return Err::E_PARSING;
    }
    auto tpot_packet = GetTpotPacket(buffer);
    auto type = tpot_packet->packet_type();

    if (AnyPacket_MIN < COMPARE_AND(type) <= AnyPacket_MAX) {
        auto header = tpot_packet->header();
        auto packet = tpot_packet->packet();
        if (VerifyAnyPacket(verifier, packet, type) == false) {
            return Err::E_PARSING;
        }

        // @formatter:off
        switch (type) {
        case AnyPacket_PacketVersionRequest: onPacketVersionRequest (*header, *(PacketVersionRequest *)packet, arg); break;
        case AnyPacket_PacketVersionResponse:onPacketVersionResponse(*header, *(PacketVersionResponse*)packet, arg); break;
        case AnyPacket_ExecRequest:          onExecRequest          (*header, *(ExecRequest          *)packet, arg); break;
        case AnyPacket_ExecResponse:         onExecResponse         (*header, *(ExecResponse         *)packet, arg); break;
        case AnyPacket_HeartbitRequest:      onHeartbitRequest      (*header, *(HeartbitRequest      *)packet, arg); break;
        case AnyPacket_HeartbitResponse:     onHeartbitResponse     (*header, *(HeartbitResponse     *)packet, arg); break;
        case AnyPacket_ListRequest:          onListRequest          (*header, *(ListRequest          *)packet, arg); break;
        case AnyPacket_ListResponse:         onListResponse         (*header, *(ListResponse         *)packet, arg); break;
        case AnyPacket_KillRequest:          onKillRequest          (*header, *(KillRequest          *)packet, arg); break;
        case AnyPacket_KillResponse:         onKillResponse         (*header, *(KillResponse         *)packet, arg); break;
        default:
            TBAG_INACCESSIBLE_BLOCK_ASSERT();
        }
        // @formatter:on
        return Err::E_SUCCESS;
    }
    return Err::E_PARSING;
}

uint8_t * TpotPacket::point() const
{
    return _builder.GetBufferPointer();
}

std::size_t TpotPacket::size() const
{
    return _builder.GetSize();
}

uint64_t TpotPacket::genId()
{
    return static_cast<uint64_t>(id::generator::genTimeId());
}

} // namespace proto

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

