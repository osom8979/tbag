/**
 * @file   MsgPacket.cpp
 * @brief  MsgPacket class implementation.
 * @author zer0
 * @date   2018-11-14
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

/**
 * A FLATBUFFERS_TRACK_VERIFIER_BUFFER_SIZE macro is required to use the Verifier::GetComputedSize() method.
 *
 * @see <https://github.com/google/flatbuffers/issues/4604>
 */
#define FLATBUFFERS_TRACK_VERIFIER_BUFFER_SIZE

// FlatBuffers generated files.
#include <flatbuffers/flatbuffers.h>
#include <flatbuffers/idl.h>
#include <libtbag/proto/fbs/msg_generated.h>
#include <libtbag/proto/fbs/msg_t2s.h>

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

#include <libtbag/proto/MsgPacket.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace proto {

using namespace proto::fbs;

using FlatBufferBuilder = flatbuffers::FlatBufferBuilder;
using FlatBufferParser  = flatbuffers::Parser;

using StringOffset       = flatbuffers::Offset<flatbuffers::String>;
using StringOffsetVector = flatbuffers::Vector<StringOffset>;

/**
 * FlatBuffer builder implementation.
 *
 * @author zer0
 * @date   2018-11-14
 */
struct MsgPacketBuilder::Impl
{
public:
    FlatBufferBuilder builder;
    FlatBufferParser  parser;

public:
    Impl(std::size_t capacity) : builder(capacity, nullptr)
    {
        if (parser.Parse(__get_text_to_cpp11_string__msg__()) == false) {
            tDLogA("MsgPacketBuilder::Impl() Parse fail.");
            throw std::bad_alloc();
        }
    }

    ~Impl()
    {
        // EMPTY.
    }

public:
    uint8_t * point() const
    {
        return builder.GetBufferPointer();
    }

    std::size_t size() const
    {
        return builder.GetSize();
    }

    void clear()
    {
        builder.Clear();
    }

    Err assign(uint8_t const * buffer, std::size_t size)
    {
        clear();
        builder.PushFlatBuffer(buffer, size);
        return Err::E_SUCCESS;
    }

    std::string toJsonString() const
    {
        std::string result;
        if (flatbuffers::GenerateText(parser, builder.GetBufferPointer(), &result)) {
            return result;
        }
        return std::string();
    }

    template <typename T>
    void finish(flatbuffers::Offset<T> root, char const * file_identifier = nullptr)
    {
        builder.Finish(root, file_identifier);
    }

    Err build(MqMsg const & msg)
    {
        return build((int32_t)msg.event, (int8_t const *)msg.data(), msg.size());
    }

    Err build(int32_t event, int8_t const * data, std::size_t size)
    {
        using namespace libtbag::proto::fbs::msg;
        clear();
        finish(CreateMsgPacket(builder, event, builder.CreateVector(data, size)));
        return Err::E_SUCCESS;
    }
};

// --------------------------------
// MsgPacketBuilder implementation.
// --------------------------------

MsgPacketBuilder::MsgPacketBuilder(std::size_t capacity) : _impl(std::make_unique<Impl>(capacity))
{
    assert(static_cast<bool>(_impl));
}

MsgPacketBuilder::~MsgPacketBuilder()
{
    // EMPTY.
}

uint8_t * MsgPacketBuilder::point() const
{
    assert(static_cast<bool>(_impl));
    return _impl->point();
}

std::size_t MsgPacketBuilder::size() const
{
    assert(static_cast<bool>(_impl));
    return _impl->size();
}

Err MsgPacketBuilder::assign(uint8_t const * buffer, std::size_t size)
{
    assert(static_cast<bool>(_impl));
    return _impl->assign(buffer, size);
}

Err MsgPacketBuilder::assign(Buffer const & buffer)
{
    return assign((uint8_t const *)buffer.data(), buffer.size());
}

std::string MsgPacketBuilder::toJsonString() const
{
    assert(static_cast<bool>(_impl));
    return _impl->toJsonString();
}

Err MsgPacketBuilder::build(MqMsg const & msg)
{
    assert(static_cast<bool>(_impl));
    return _impl->build(msg);
}

Err MsgPacketBuilder::build(int32_t event, int8_t const * data, std::size_t size)
{
    assert(static_cast<bool>(_impl));
    return _impl->build(event, data, size);
}

MsgPacketBuilder::Buffer MsgPacketBuilder::toBuffer() const
{
    return Buffer(point(), point() + size());
}

// --------------------------------
// MsgPacketParser implementation.
// --------------------------------

MsgPacketParser::MsgPacketParser()
{
    // EMPTY.
}

MsgPacketParser::~MsgPacketParser()
{
    // EMPTY.
}

Err MsgPacketParser::parse(char const * buffer, std::size_t size, MqMsg * msg, std::size_t * computed_size)
{
    using namespace flatbuffers;
    using namespace libtbag::proto::fbs::msg;

    Verifier verifier((uint8_t const *)buffer, size);
    if (!VerifyMsgPacketBuffer(verifier)) {
        return Err::E_PARSING;
    }

#if defined(FLATBUFFERS_TRACK_VERIFIER_BUFFER_SIZE)
    if (computed_size != nullptr) {
        *computed_size = verifier.GetComputedSize();
    }
#endif

    if (msg != nullptr) {
        auto const * PACKET = GetMsgPacket(buffer);
        msg->event = (MqEvent)PACKET->event();
        msg->buffer.assign(PACKET->data()->begin(), PACKET->data()->end());
    }
    return Err::E_SUCCESS;
}

Err MsgPacketParser::parse(Buffer const & buffer, MqMsg * msg, std::size_t * computed_size)
{
    return parse(buffer.data(), buffer.size(), msg, computed_size);
}

// -------------------------
// MsgPacket implementation.
// -------------------------

MsgPacket::MsgPacket(std::size_t capacity) : MsgPacketBuilder(capacity)
{
    // EMPTY.
}

MsgPacket::~MsgPacket()
{
    // EMPTY.
}

} // namespace proto

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

