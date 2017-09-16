/**
 * @file   TpotPacket.cpp
 * @brief  TpotPacket class implementation.
 * @author zer0
 * @date   2017-09-06
 */

#if defined(TBAG_COMP_MSVC)
# if defined(min)
TBAG_PUSH_MACRO(min);
# undef min
# define __RESTORE_MIN__
# endif // defined(min)
# if defined(max)
TBAG_PUSH_MACRO(max);
# undef max
# define __RESTORE_MAX__
# endif // defined(max)
#endif // defined(TBAG_COMP_MSVC)

// FlatBuffers generated files.
#include <libtbag/proto/fbs/tpot_generated.h>

#if defined(TBAG_COMP_MSVC)
# if defined(__RESTORE_MIN__)
TBAG_POP_MACRO(min);
# undef __RESTORE_MIN__
# endif // defined(__RESTORE_MIN__)
# if defined(__RESTORE_MAX__)
TBAG_POP_MACRO(max);
# undef __RESTORE_MAX__
# endif // defined(__RESTORE_MAX__)
#endif // defined(TBAG_COMP_MSVC)

#include <libtbag/proto/TpotPacket.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/Type.hpp>

#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace proto {

// -------------------------------------------
// TpotPacketBuilder::Internal implementation.
// -------------------------------------------

using namespace proto::fbs;

class TpotPacketBuilder::Internal
{
public:
    using FlatBufferBuilder      = flatbuffers::FlatBufferBuilder;

    using HeaderOffset           = flatbuffers::Offset<tpot::Header>;

    using PairOffset             = flatbuffers::Offset<tpot::Pair>;
    using PairOffsetVector       = flatbuffers::Vector<PairOffset>;
    using PairOffsetVectorOffset = flatbuffers::Offset<PairOffsetVector>;

    using ProcessInfoOffset             = flatbuffers::Offset<tpot::ProcessInfo>;
    using ProcessInfoOffsetVector       = flatbuffers::Vector<ProcessInfoOffset>;
    using ProcessInfoOffsetVectorOffset = flatbuffers::Offset<ProcessInfoOffsetVector>;

    // @formatter:off
    STATIC_ASSERT_CHECK_IS_EQUALS(tpot::ResultCode_SUCCESS      , static_cast<int>(TpotPacketTypes::ResultCode::RC_SUCCESS));
    STATIC_ASSERT_CHECK_IS_EQUALS(tpot::ResultCode_UNKNOWN_ERROR, static_cast<int>(TpotPacketTypes::ResultCode::RC_UNKNOWN_ERROR));
    STATIC_ASSERT_CHECK_IS_EQUALS(tpot::ResultCode_EXECUTE_ERROR, static_cast<int>(TpotPacketTypes::ResultCode::RC_EXECUTE_ERROR));
    STATIC_ASSERT_CHECK_IS_EQUALS(tpot::ResultCode_NOT_EXISTS   , static_cast<int>(TpotPacketTypes::ResultCode::RC_NOT_EXISTS));
    STATIC_ASSERT_CHECK_IS_EQUALS(tpot::ResultCode_KILL_ERROR   , static_cast<int>(TpotPacketTypes::ResultCode::RC_KILL_ERROR));
    // @formatter:on

private:
    TpotPacketBuilder * _parent;

private:
    FlatBufferBuilder _builder;

public:
    Internal(TpotPacketBuilder * parent, std::size_t capacity) : _parent(parent), _builder(capacity, nullptr)
    {
        // EMPTY.
    }

    ~Internal()
    {
        // EMPTY.
    }

public:
    inline FlatBufferBuilder       & atBuilder()       TBAG_NOEXCEPT { return _builder; }
    inline FlatBufferBuilder const & atBuilder() const TBAG_NOEXCEPT { return _builder; }

    uint8_t *  point() const { return _builder.GetBufferPointer(); }
    std::size_t size() const { return _builder.GetSize(); }

    void clear()
    {
        _builder.Clear();
    }

    template <typename T>
    void finish(flatbuffers::Offset<T> root, char const * file_identifier = nullptr)
    {
        _builder.Finish(root, file_identifier);
    }

public:
    PairOffset createPair(util::Pair const & pair)
    {
        return tpot::CreatePair(_builder, _builder.CreateString(pair.key), _builder.CreateString(pair.val));
    }

    PairOffsetVectorOffset createPairs(util::Pairs const & pairs)
    {
        std::vector<PairOffset> infos_offset;
        for (auto & pair : pairs) {
            infos_offset.push_back(createPair(pair));
        }
        return _builder.CreateVector(infos_offset);
    }

public:
    HeaderOffset createHeader(util::Header const & header)
    {
        return tpot::CreateHeader(_builder, header.id, static_cast<tpot::ResultCode>(header.code), createPairs(header.infos));
    }

public:
    ProcessInfoOffset createProcessInfo(util::ProcessInfo const & proc)
    {
        return tpot::CreateProcessInfo(_builder, proc.pid, proc.active, createPairs(proc.infos));
    }

    ProcessInfoOffsetVectorOffset createProcessInfos(std::vector<util::ProcessInfo> const & procs)
    {
        std::vector<ProcessInfoOffset> procs_offset;
        for (auto & proc : procs) {
            procs_offset.push_back(createProcessInfo(proc));
        }
        return _builder.CreateVector(procs_offset);
    }
};

// ---------------------------------
// TpotPacketBuilder implementation.
// ---------------------------------

TpotPacketBuilder::TpotPacketBuilder(std::size_t capacity) : _internal(new Internal(this, capacity))
{
    assert(static_cast<bool>(_internal));
}

TpotPacketBuilder::~TpotPacketBuilder()
{
    // EMPTY.
}

uint8_t * TpotPacketBuilder::point() const
{
    assert(static_cast<bool>(_internal));
    return _internal->point();
}

std::size_t TpotPacketBuilder::size() const
{
    assert(static_cast<bool>(_internal));
    return _internal->size();
}

Err TpotPacketBuilder::buildVersionRequest(util::Header const & header)
{
    assert(static_cast<bool>(_internal));
    auto & builder = _internal->atBuilder();
    _internal->clear();
    auto packet = tpot::CreateTpotPacket(builder, _internal->createHeader(header), tpot::AnyPacket_VersionRequest,
                                         tpot::CreateVersionRequest(builder).Union());
    _internal->finish(packet);
    return Err::E_SUCCESS;
}

Err TpotPacketBuilder::buildVersionResponse(util::Header const & header, unsigned major, unsigned minor, util::Pairs const & features)
{
    assert(static_cast<bool>(_internal));
    auto & builder = _internal->atBuilder();
    _internal->clear();
    tpot::Version version(major, minor);
    auto packet = tpot::CreateTpotPacket(builder, _internal->createHeader(header), tpot::AnyPacket_VersionResponse,
                                         tpot::CreateVersionResponse(builder, &version, _internal->createPairs(features)).Union());
    _internal->finish(packet);
    return Err::E_SUCCESS;
}

Err TpotPacketBuilder::buildVersionResponse(util::Header const & header, util::Version const & version, util::Pairs const & features)
{
    return buildVersionResponse(header, version.getMajor(), version.getMinor(), features);
}

Err TpotPacketBuilder::buildEchoRequest(util::Header const & header, std::string const & message)
{
    assert(static_cast<bool>(_internal));
    auto & builder = _internal->atBuilder();
    _internal->clear();
    auto packet = tpot::CreateTpotPacket(builder, _internal->createHeader(header), tpot::AnyPacket_EchoRequest,
                                         tpot::CreateEchoRequest(builder, builder.CreateString(message)).Union());
    _internal->finish(packet);
    return Err::E_SUCCESS;
}

Err TpotPacketBuilder::buildEchoResponse(util::Header const & header, std::string const & message)
{
    assert(static_cast<bool>(_internal));
    auto & builder = _internal->atBuilder();
    _internal->clear();
    auto packet = tpot::CreateTpotPacket(builder, _internal->createHeader(header), tpot::AnyPacket_EchoResponse,
                                         tpot::CreateEchoResponse(builder, builder.CreateString(message)).Union());
    _internal->finish(packet);
    return Err::E_SUCCESS;
}

Err TpotPacketBuilder::buildLoginRequest(util::Header const & header, std::string const & id, std::string const & pw)
{
    assert(static_cast<bool>(_internal));
    auto & builder = _internal->atBuilder();
    _internal->clear();
    auto packet = tpot::CreateTpotPacket(builder, _internal->createHeader(header), tpot::AnyPacket_LoginRequest,
                                         tpot::CreateLoginRequest(builder, builder.CreateString(id), builder.CreateString(pw)).Union());
    _internal->finish(packet);
    return Err::E_SUCCESS;
}

Err TpotPacketBuilder::buildLoginResponse(util::Header const & header, std::string const & key)
{
    assert(static_cast<bool>(_internal));
    auto & builder = _internal->atBuilder();
    _internal->clear();
    auto packet = tpot::CreateTpotPacket(builder, _internal->createHeader(header), tpot::AnyPacket_LoginResponse,
                                         tpot::CreateLoginResponse(builder, builder.CreateString(key)).Union());
    _internal->finish(packet);
    return Err::E_SUCCESS;
}

Err TpotPacketBuilder::buildLogoutRequest(util::Header const & header)
{
    assert(static_cast<bool>(_internal));
    auto & builder = _internal->atBuilder();
    _internal->clear();
    auto packet = tpot::CreateTpotPacket(builder, _internal->createHeader(header), tpot::AnyPacket_LogoutRequest,
                                         tpot::CreateLogoutRequest(builder).Union());
    _internal->finish(packet);
    return Err::E_SUCCESS;
}

Err TpotPacketBuilder::buildLogoutResponse(util::Header const & header)
{
    assert(static_cast<bool>(_internal));
    auto & builder = _internal->atBuilder();
    _internal->clear();
    auto packet = tpot::CreateTpotPacket(builder, _internal->createHeader(header), tpot::AnyPacket_LogoutResponse,
                                         tpot::CreateLogoutResponse(builder).Union());
    _internal->finish(packet);
    return Err::E_SUCCESS;
}

Err TpotPacketBuilder::buildExecRequest(util::Header const & header,
                                        std::string const & file,
                                        std::vector<std::string> const & args,
                                        std::vector<std::string> const & envs,
                                        std::string const & cwd,
                                        std::string const & input)
{
    assert(static_cast<bool>(_internal));
    auto & builder = _internal->atBuilder();
    _internal->clear();
    auto packet = tpot::CreateTpotPacket(builder, _internal->createHeader(header), tpot::AnyPacket_ExecRequest,
                                         tpot::CreateExecRequest(builder,
                                                                 builder.CreateString(file),
                                                                 builder.CreateVectorOfStrings(args),
                                                                 builder.CreateVectorOfStrings(envs),
                                                                 builder.CreateString(cwd),
                                                                 builder.CreateString(input)).Union());
    _internal->finish(packet);
    return Err::E_SUCCESS;
}

Err TpotPacketBuilder::buildExecRequest(util::Header const & header, util::ExecParam const & param)
{
    return buildExecRequest(header, param.file, param.args, param.envs, param.cwd, param.input);
}

Err TpotPacketBuilder::buildExecResponse(util::Header const & header, int pid)
{
    assert(static_cast<bool>(_internal));
    auto & builder = _internal->atBuilder();
    _internal->clear();
    auto packet = tpot::CreateTpotPacket(builder, _internal->createHeader(header), tpot::AnyPacket_ExecResponse,
                                         tpot::CreateExecResponse(builder, pid).Union());
    _internal->finish(packet);
    return Err::E_SUCCESS;
}

Err TpotPacketBuilder::buildProcessListRequest(util::Header const & header)
{
    assert(static_cast<bool>(_internal));
    auto & builder = _internal->atBuilder();
    _internal->clear();
    auto packet = tpot::CreateTpotPacket(builder, _internal->createHeader(header), tpot::AnyPacket_ProcessListRequest,
                                         tpot::CreateProcessListRequest(builder).Union());
    _internal->finish(packet);
    return Err::E_SUCCESS;
}

Err TpotPacketBuilder::buildProcessListResponse(util::Header const & header, std::vector<util::ProcessInfo> const & procs)
{
    assert(static_cast<bool>(_internal));
    auto & builder = _internal->atBuilder();
    _internal->clear();
    auto packet = tpot::CreateTpotPacket(builder, _internal->createHeader(header), tpot::AnyPacket_ProcessListResponse,
                                         tpot::CreateProcessListResponse(builder, _internal->createProcessInfos(procs)).Union());
    _internal->finish(packet);
    return Err::E_SUCCESS;
}

Err TpotPacketBuilder::buildProcessKillRequest(util::Header const & header, int pid, int signum)
{
    assert(static_cast<bool>(_internal));
    auto & builder = _internal->atBuilder();
    _internal->clear();
    auto packet = tpot::CreateTpotPacket(builder, _internal->createHeader(header), tpot::AnyPacket_ProcessKillRequest,
                                         tpot::CreateProcessKillRequest(builder, pid, signum).Union());
    _internal->finish(packet);
    return Err::E_SUCCESS;
}

Err TpotPacketBuilder::buildProcessKillResponse(util::Header const & header)
{
    assert(static_cast<bool>(_internal));
    auto & builder = _internal->atBuilder();
    _internal->clear();
    auto packet = tpot::CreateTpotPacket(builder, _internal->createHeader(header), tpot::AnyPacket_ProcessKillResponse,
                                         tpot::CreateProcessKillResponse(builder).Union());
    _internal->finish(packet);
    return Err::E_SUCCESS;
}

Err TpotPacketBuilder::buildProcessRemoveRequest(util::Header const & header, int pid)
{
    assert(static_cast<bool>(_internal));
    auto & builder = _internal->atBuilder();
    _internal->clear();
    auto packet = tpot::CreateTpotPacket(builder, _internal->createHeader(header), tpot::AnyPacket_ProcessRemoveRequest,
                                         tpot::CreateProcessRemoveRequest(builder, pid).Union());
    _internal->finish(packet);
    return Err::E_SUCCESS;
}

Err TpotPacketBuilder::buildProcessRemoveResponse(util::Header const & header)
{
    assert(static_cast<bool>(_internal));
    auto & builder = _internal->atBuilder();
    _internal->clear();
    auto packet = tpot::CreateTpotPacket(builder, _internal->createHeader(header), tpot::AnyPacket_ProcessRemoveResponse,
                                         tpot::CreateProcessRemoveResponse(builder).Union());
    _internal->finish(packet);
    return Err::E_SUCCESS;
}

// ------------------------------------------
// TpotPacketParser::Internal implementation.
// ------------------------------------------

class TpotPacketParser::Internal
{
public:
    using PairOffset       = flatbuffers::Offset<tpot::Pair>;
    using PairOffsetVector = flatbuffers::Vector<PairOffset>;

    using ProcessInfoOffset       = flatbuffers::Offset<tpot::ProcessInfo>;
    using ProcessInfoOffsetVector = flatbuffers::Vector<ProcessInfoOffset>;

    using StringOffset       = flatbuffers::Offset<flatbuffers::String>;
    using StringOffsetVector = flatbuffers::Vector<StringOffset>;

private:
    TpotPacketParser * _parent;

public:
    Internal(TpotPacketParser * parent) : _parent(parent)
    {
        // EMPTY.
    }

    ~Internal()
    {
        // EMPTY.
    }

public:
    Err parse(char const * buffer, std::size_t size, void * arg)
    {
        using namespace flatbuffers;
        Verifier verifier((uint8_t const *)buffer, size);
        if (tpot::VerifyTpotPacketBuffer(verifier) == false) {
            return Err::E_PARSING;
        }
        auto tpot_packet = tpot::GetTpotPacket(buffer);
        auto type = tpot_packet->packet_type();

        if (tpot::AnyPacket_MIN < COMPARE_AND(type) <= tpot::AnyPacket_MAX) {
            auto header = tpot_packet->header();
            auto packet = tpot_packet->packet();
            if (VerifyAnyPacket(verifier, packet, type) == false) {
                return Err::E_PARSING;
            }

            // @formatter:off
            switch (type) {
            case tpot::AnyPacket_VersionRequest:        onVersionRequest       (header, (tpot::VersionRequest        const *)packet, arg); break;
            case tpot::AnyPacket_VersionResponse:       onVersionResponse      (header, (tpot::VersionResponse       const *)packet, arg); break;
            case tpot::AnyPacket_EchoRequest:           onEchoRequest          (header, (tpot::EchoRequest           const *)packet, arg); break;
            case tpot::AnyPacket_EchoResponse:          onEchoResponse         (header, (tpot::EchoResponse          const *)packet, arg); break;
            case tpot::AnyPacket_LoginRequest:          onLoginRequest         (header, (tpot::LoginRequest          const *)packet, arg); break;
            case tpot::AnyPacket_LoginResponse:         onLoginResponse        (header, (tpot::LoginResponse         const *)packet, arg); break;
            case tpot::AnyPacket_LogoutRequest:         onLogoutRequest        (header, (tpot::LogoutRequest         const *)packet, arg); break;
            case tpot::AnyPacket_LogoutResponse:        onLogoutResponse       (header, (tpot::LogoutResponse        const *)packet, arg); break;
            case tpot::AnyPacket_ExecRequest:           onExecRequest          (header, (tpot::ExecRequest           const *)packet, arg); break;
            case tpot::AnyPacket_ExecResponse:          onExecResponse         (header, (tpot::ExecResponse          const *)packet, arg); break;
            case tpot::AnyPacket_ProcessListRequest:    onProcessListRequest   (header, (tpot::ProcessListRequest    const *)packet, arg); break;
            case tpot::AnyPacket_ProcessListResponse:   onProcessListResponse  (header, (tpot::ProcessListResponse   const *)packet, arg); break;
            case tpot::AnyPacket_ProcessKillRequest:    onProcessKillRequest   (header, (tpot::ProcessKillRequest    const *)packet, arg); break;
            case tpot::AnyPacket_ProcessKillResponse:   onProcessKillResponse  (header, (tpot::ProcessKillResponse   const *)packet, arg); break;
            case tpot::AnyPacket_ProcessRemoveRequest:  onProcessRemoveRequest (header, (tpot::ProcessRemoveRequest  const *)packet, arg); break;
            case tpot::AnyPacket_ProcessRemoveResponse: onProcessRemoveResponse(header, (tpot::ProcessRemoveResponse const *)packet, arg); break;
            default:
                TBAG_INACCESSIBLE_BLOCK_ASSERT();
            }
            // @formatter:on
            return Err::E_SUCCESS;
        }
        return Err::E_PARSING;
    }

public:
    util::Pairs createPairs(PairOffsetVector const * pairs)
    {
        util::Pairs result;
        auto const END = pairs->end();
        for (auto itr = pairs->begin(); itr != END; ++itr) {
            result.emplace_back(itr->key()->str(), itr->val()->str());
        }
        return result;
    }

    util::Header createHeader(tpot::Header const * header)
    {
        return util::Header(header->id(), header->code(), createPairs(header->infos()));
    }

    std::vector<util::ProcessInfo> createProcessInfos(ProcessInfoOffsetVector const * pairs)
    {
        std::vector<util::ProcessInfo> result;
        auto const END = pairs->end();
        for (auto itr = pairs->begin(); itr != END; ++itr) {
            result.emplace_back(itr->pid(), itr->active(), createPairs(itr->infos()));
        }
        return result;
    }

    std::vector<std::string> createStrings(StringOffsetVector const * strings)
    {
        std::vector<std::string> result;
        auto const END = strings->end();
        for (auto itr = strings->begin(); itr != END; ++itr) {
            result.emplace_back(itr->str());
        }
        return result;
    }

protected:
    void onVersionRequest(tpot::Header const * header, tpot::VersionRequest const * packet, void * arg)
    {
        assert(_parent != nullptr);
        _parent->onVersionRequest(createHeader(header), arg);
    }

    void onVersionResponse(tpot::Header const * header, tpot::VersionResponse const * packet, void * arg)
    {
        assert(_parent != nullptr);
        util::Version const VERSION(packet->version()->major(), packet->version()->minor());
        _parent->onVersionResponse(createHeader(header), VERSION, createPairs(packet->features()), arg);
    }

    void onEchoRequest(tpot::Header const * header, tpot::EchoRequest const * packet, void * arg)
    {
        assert(_parent != nullptr);
        _parent->onEchoRequest(createHeader(header), packet->msg()->str(), arg);
    }

    void onEchoResponse(tpot::Header const * header, tpot::EchoResponse const * packet, void * arg)
    {
        assert(_parent != nullptr);
        _parent->onEchoResponse(createHeader(header), packet->msg()->str(), arg);
    }

    void onLoginRequest(tpot::Header const * header, tpot::LoginRequest const * packet, void * arg)
    {
        assert(_parent != nullptr);
        _parent->onLoginRequest(createHeader(header), packet->id()->str(), packet->pw()->str(), arg);
    }

    void onLoginResponse(tpot::Header const * header, tpot::LoginResponse const * packet, void * arg)
    {
        assert(_parent != nullptr);
        _parent->onLoginResponse(createHeader(header), packet->key()->str(), arg);
    }

    void onLogoutRequest(tpot::Header const * header, tpot::LogoutRequest const * packet, void * arg)
    {
        assert(_parent != nullptr);
        _parent->onLogoutRequest(createHeader(header), arg);
    }

    void onLogoutResponse(tpot::Header const * header, tpot::LogoutResponse const * packet, void * arg)
    {
        assert(_parent != nullptr);
        _parent->onLogoutResponse(createHeader(header), arg);
    }

    void onExecRequest(tpot::Header const * header, tpot::ExecRequest const * packet, void * arg)
    {
        auto itr = packet->args();
        assert(_parent != nullptr);
        util::ExecParam params;
        params.file  = packet->file()->str();
        params.args  = createStrings(packet->args());
        params.envs  = createStrings(packet->envs());
        params.cwd   = packet->cwd()->str();
        params.input = packet->input()->str();
        _parent->onExecRequest(createHeader(header), params, arg);
    }

    void onExecResponse(tpot::Header const * header, tpot::ExecResponse const * packet, void * arg)
    {
        assert(_parent != nullptr);
        _parent->onExecResponse(createHeader(header), packet->pid(), arg);
    }

    void onProcessListRequest(tpot::Header const * header, tpot::ProcessListRequest const * packet, void * arg)
    {
        assert(_parent != nullptr);
        _parent->onProcessListRequest(createHeader(header), arg);
    }

    void onProcessListResponse(tpot::Header const * header, tpot::ProcessListResponse const * packet, void * arg)
    {
        assert(_parent != nullptr);
        _parent->onProcessListResponse(createHeader(header), createProcessInfos(packet->procs()), arg);
    }

    void onProcessKillRequest(tpot::Header const * header, tpot::ProcessKillRequest const * packet, void * arg)
    {
        assert(_parent != nullptr);
        _parent->onProcessKillRequest(createHeader(header), packet->pid(), packet->signum(), arg);
    }

    void onProcessKillResponse(tpot::Header const * header, tpot::ProcessKillResponse const * packet, void * arg)
    {
        assert(_parent != nullptr);
        _parent->onProcessKillResponse(createHeader(header), arg);
    }

    void onProcessRemoveRequest(tpot::Header const * header, tpot::ProcessRemoveRequest const * packet, void * arg)
    {
        assert(_parent != nullptr);
        _parent->onProcessRemoveRequest(createHeader(header), packet->pid(), arg);
    }

    void onProcessRemoveResponse(tpot::Header const * header, tpot::ProcessRemoveResponse const * packet, void * arg)
    {
        assert(_parent != nullptr);
        _parent->onProcessRemoveResponse(createHeader(header), arg);
    }
};

// --------------------------------
// TpotPacketParser implementation.
// --------------------------------

TpotPacketParser::TpotPacketParser() : _internal(new Internal(this))
{
    assert(static_cast<bool>(_internal));
}

TpotPacketParser::~TpotPacketParser()
{
    // EMPTY.
}

Err TpotPacketParser::parse(char const * buffer, std::size_t size, void * arg)
{
    assert(static_cast<bool>(_internal));
    return _internal->parse(buffer, size, arg);
}

// --------------------------
// TpotPacket implementation.
// --------------------------

TpotPacket::TpotPacket(std::size_t capacity) : TpotPacketBuilder(capacity), TpotPacketParser()
{
    // EMPTY.
}

TpotPacket::~TpotPacket()
{
    // EMPTY.
}

} // namespace proto

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

