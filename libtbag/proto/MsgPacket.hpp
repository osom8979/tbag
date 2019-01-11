/**
 * @file   MsgPacket.hpp
 * @brief  MsgPacket class prototype.
 * @author zer0
 * @date   2018-11-14
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PROTO_MSGPACKET_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PROTO_MSGPACKET_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/Unit.hpp>
#include <libtbag/mq/details/MqCommon.hpp>
#include <libtbag/util/BufferInfo.hpp>

#include <cstdint>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace proto {

struct MsgPacketTypes : private Noncopyable
{
    using MqEvent = libtbag::mq::details::MqEvent;
    using MqMsg   = libtbag::mq::details::MqMsg;
    using Buffer  = libtbag::util::Buffer;

    TBAG_CONSTEXPR static MqEvent const ME_MSG = libtbag::mq::details::ME_MSG;
};

/**
 * MsgPacketBuilder class prototype.
 *
 * @author zer0
 * @date   2018-11-14
 */
class TBAG_API MsgPacketBuilder : public MsgPacketTypes
{
public:
    struct Impl;
    friend struct Impl;
    using UniqueImpl = std::unique_ptr<Impl>;

public:
    TBAG_CONSTEXPR static std::size_t const DEFAULT_BUILDER_CAPACITY = 1 * MEGA_BYTE_TO_BYTE;

private:
    UniqueImpl _impl;

public:
    MsgPacketBuilder(std::size_t capacity = DEFAULT_BUILDER_CAPACITY);
    ~MsgPacketBuilder();

public:
    uint8_t * point() const;
    std::size_t size() const;

public:
    Err assign(uint8_t const * buffer, std::size_t size);
    Err assign(Buffer const & buffer);

public:
    std::string toJsonString() const;

public:
    Err build(MqMsg const & msg);
    Err build(int32_t event, int8_t const * data, std::size_t size);

public:
    Buffer toBuffer() const;
};

/**
 * MsgPacketParser class prototype.
 *
 * @author zer0
 * @date   2018-11-14
 */
class TBAG_API MsgPacketParser : public MsgPacketTypes
{
public:
    MsgPacketParser();
    ~MsgPacketParser();

public:
    Err parse(char const * buffer, std::size_t size, MqMsg * msg, std::size_t * computed_size);
    Err parse(Buffer const & buffer, MqMsg * msg, std::size_t * computed_size);
};

/**
 * MsgPacket class prototype.
 *
 * @author zer0
 * @date   2018-11-14
 *
 * @translate{ko, 한국인을 울린 그 맛!}
 */
class TBAG_API MsgPacket : public MsgPacketBuilder, public MsgPacketParser
{
private:
    MqMsg _msg;

public:
    MsgPacket(std::size_t capacity = DEFAULT_BUILDER_CAPACITY);
    ~MsgPacket();

public:
    inline MqMsg       & msg()       TBAG_NOEXCEPT { return _msg; }
    inline MqMsg const & msg() const TBAG_NOEXCEPT { return _msg; }

public:
    Err parseAndUpdate(char const * buffer, std::size_t size, std::size_t * computed_size);
    Err parseAndUpdate(Buffer const & buffer, std::size_t * computed_size);
};

} // namespace proto

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PROTO_MSGPACKET_HPP__

