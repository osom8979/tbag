/**
 * @file   WebSocketFrame.hpp
 * @brief  WebSocketFrame class prototype.
 * @author zer0
 * @date   2017-06-11
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_WEBSOCKETFRAME_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_WEBSOCKETFRAME_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>

#include <cstdint>
#include <vector>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

/**
 * WebSocketFrame class prototype.
 *
 * @author zer0
 * @date   2017-06-11
 *
 * @remarks
 *    0                   1                   2                   3
 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *   +-+-+-+-+-------+-+-------------+-------------------------------+
 *   |F|R|R|R| opcode|M| Payload len |    Extended payload length    |
 *   |I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
 *   |N|V|V|V|       |S|             |   (if payload len==126/127)   |
 *   | |1|2|3|       |K|             |                               |
 *   +-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
 *   |     Extended payload length continued, if payload len == 127  |
 *   + - - - - - - - - - - - - - - - +-------------------------------+
 *   |                               |Masking-key, if MASK set to 1  |
 *   +-------------------------------+-------------------------------+
 *   | Masking-key (continued)       |          Payload Data         |
 *   +-------------------------------- - - - - - - - - - - - - - - - +
 *   :                     Payload Data continued ...                :
 *   + - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
 *   |                     Payload Data continued ...                |
 *   +---------------------------------------------------------------+
 */
class TBAG_API WebSocketFrame
{
public:
    using Buffer = std::vector<uint8_t>;

public:
    enum class OpCode : uint8_t
    {
        CONTINUATION_FRAME           = 0x0,
        TEXT_FRAME                   = 0x1,
        BINARY_FRAME                 = 0x2,
        RESERVED_NON_CONTROL_FRAME_1 = 0x3,
        RESERVED_NON_CONTROL_FRAME_2 = 0x4,
        RESERVED_NON_CONTROL_FRAME_3 = 0x5,
        RESERVED_NON_CONTROL_FRAME_4 = 0x6,
        RESERVED_NON_CONTROL_FRAME_5 = 0x7,
        CONNECTION_CLOSE             = 0x8,
        DENOTES_PING                 = 0x9,
        DENOTES_PONG                 = 0xA,
        RESERVED_CONTROL_FRAME_1     = 0xB,
        RESERVED_CONTROL_FRAME_2     = 0xC,
        RESERVED_CONTROL_FRAME_3     = 0xD,
        RESERVED_CONTROL_FRAME_4     = 0xE,
        RESERVED_CONTROL_FRAME_5     = 0xF,
    };

private:
    Buffer _buffer;

public:
    WebSocketFrame();
    WebSocketFrame(WebSocketFrame const & obj);
    WebSocketFrame(WebSocketFrame && obj);
    virtual ~WebSocketFrame();

public:
    WebSocketFrame & operator =(WebSocketFrame const & obj);
    WebSocketFrame & operator =(WebSocketFrame && obj);

public:
    inline bool empty() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_buffer.empty()))
    { return _buffer.empty(); }
    inline std::size_t size() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_buffer.size()))
    { return _buffer.size(); }

public:
    /**
     * Indicates that this is the final fragment in a message.
     * The first fragment MAY also be the final fragment.
     */
    bool fin() const;

    /** MUST be 0 unless an extension is negotiated that defines meanings for non-zero values. */
    bool rsv1() const;

    /** MUST be 0 unless an extension is negotiated that defines meanings for non-zero values. */
    bool rsv2() const;

    /** MUST be 0 unless an extension is negotiated that defines meanings for non-zero values. */
    bool rsv3() const;

    /** Defines the interpretation of the "Payload data". */
    OpCode opcode() const;

    /** Defines whether the "Payload data" is masked. */
    bool mask() const;

    enum class PayloadBit : uint8_t
    {
        PL_BIT_7  =  7,
        PL_BIT_16 = 16,
        PL_BIT_64 = 64,
    };

    /** bit size of payload length. */
    PayloadBit payloadLengthBitSize() const;

    /** payload length. */
    uint64_t payloadLength() const;

    /** byte index of Masking-key. */
    uint8_t maskingKeyByteIndex() const;

    /** Masking-key, if MASK set to 1. */
    uint32_t maskingKey() const;

    /** byte index of payload data. */
    uint8_t payloadDataByteIndex() const;

    /** Payload data. */
    void const * payloadData() const;

public:
    Err readData(std::vector<uint8_t> & result);
};

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_WEBSOCKETFRAME_HPP__

