/**
 * @file   WebSocketFrame.hpp
 * @brief  WebSocketFrame class prototype.
 * @author zer0
 * @date   2017-06-11
 *
 * @see <https://tools.ietf.org/html/rfc6455>
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

#include <libtbag/network/http/HttpProperty.hpp>
#include <libtbag/network/http/HttpParser.hpp>
#include <libtbag/network/http/HttpBuilder.hpp>

#include <cstdint>
#include <vector>
#include <string>
#include <set>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

enum class OpCode : uint8_t
{
    OC_CONTINUATION_FRAME           = 0x0,
    OC_TEXT_FRAME                   = 0x1,
    OC_BINARY_FRAME                 = 0x2,
    OC_RESERVED_NON_CONTROL_FRAME_1 = 0x3,
    OC_RESERVED_NON_CONTROL_FRAME_2 = 0x4,
    OC_RESERVED_NON_CONTROL_FRAME_3 = 0x5,
    OC_RESERVED_NON_CONTROL_FRAME_4 = 0x6,
    OC_RESERVED_NON_CONTROL_FRAME_5 = 0x7,
    OC_CONNECTION_CLOSE             = 0x8,
    OC_DENOTES_PING                 = 0x9,
    OC_DENOTES_PONG                 = 0xA,
    OC_RESERVED_CONTROL_FRAME_1     = 0xB,
    OC_RESERVED_CONTROL_FRAME_2     = 0xC,
    OC_RESERVED_CONTROL_FRAME_3     = 0xD,
    OC_RESERVED_CONTROL_FRAME_4     = 0xE,
    OC_RESERVED_CONTROL_FRAME_5     = 0xF,
};

TBAG_CONSTEXPR char const * const OP_CODE_NAME_CONTINUE = "CONTINUE";
TBAG_CONSTEXPR char const * const OP_CODE_NAME_TEXT     = "TEXT";
TBAG_CONSTEXPR char const * const OP_CODE_NAME_BINARY   = "BINARY";
TBAG_CONSTEXPR char const * const OP_CODE_NAME_NCF1     = "NCF1";
TBAG_CONSTEXPR char const * const OP_CODE_NAME_NCF2     = "NCF2";
TBAG_CONSTEXPR char const * const OP_CODE_NAME_NCF3     = "NCF3";
TBAG_CONSTEXPR char const * const OP_CODE_NAME_NCF4     = "NCF4";
TBAG_CONSTEXPR char const * const OP_CODE_NAME_NCF5     = "NCF5";
TBAG_CONSTEXPR char const * const OP_CODE_NAME_CLOSE    = "CLOSE";
TBAG_CONSTEXPR char const * const OP_CODE_NAME_PING     = "PING";
TBAG_CONSTEXPR char const * const OP_CODE_NAME_PONG     = "PONG";
TBAG_CONSTEXPR char const * const OP_CODE_NAME_CF1      = "CF1";
TBAG_CONSTEXPR char const * const OP_CODE_NAME_CF2      = "CF2";
TBAG_CONSTEXPR char const * const OP_CODE_NAME_CF3      = "CF3";
TBAG_CONSTEXPR char const * const OP_CODE_NAME_CF4      = "CF4";
TBAG_CONSTEXPR char const * const OP_CODE_NAME_CF5      = "CF5";

enum class PayloadBit : uint8_t
{
    PL_BIT_7  =  7,
    PL_BIT_16 = 16,
    PL_BIT_64 = 64,
};

enum class WebSocketDirection
{
    WSD_REQUEST,
    WSD_RESPONSE,
};

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
    using Direction = WebSocketDirection;
    using Buffer = std::vector<uint8_t>;

public:
    /**
     * Indicates that this is the final fragment in a message.
     * The first fragment MAY also be the final fragment.
     */
    bool fin;

    /** MUST be 0 unless an extension is negotiated that defines meanings for non-zero values. */
    bool rsv1;

    /** MUST be 0 unless an extension is negotiated that defines meanings for non-zero values. */
    bool rsv2;

    /** MUST be 0 unless an extension is negotiated that defines meanings for non-zero values. */
    bool rsv3;

    /** Defines the interpretation of the "Payload data". */
    OpCode opcode;

    /** Defines whether the "Payload data" is masked. */
    bool mask;

    /** payload length. */
    uint64_t payload_length;

    /** Masking-key, if MASK set to 1. */
    uint32_t masking_key;

    /** Payload buffer. */
    Buffer payload;

public:
    WebSocketFrame();
    WebSocketFrame(WebSocketFrame const & obj);
    WebSocketFrame(WebSocketFrame && obj);
    virtual ~WebSocketFrame();

public:
    WebSocketFrame & operator =(WebSocketFrame const & obj);
    WebSocketFrame & operator =(WebSocketFrame && obj);

public:
    inline uint8_t const * getPayloadData() const TBAG_NOEXCEPT
    { return payload.data(); }

    inline std::size_t getPayloadSize() const TBAG_NOEXCEPT
    { return static_cast<std::size_t>(payload_length); }

public:
    Err execute(uint8_t const * data, std::size_t size);

public:
    std::size_t calculateWriteBufferSize() const;
    std::size_t copyTo(uint8_t * data, std::size_t data_size) const;
    std::size_t copyTo(Buffer & buffer) const;

public:
    void set(bool f, bool r1, bool r2, bool r3, OpCode op, uint32_t key = 0) TBAG_NOEXCEPT;
    void set(uint8_t const * data, std::size_t size) TBAG_NOEXCEPT;

public:
    Err updateRequest(bool fin, bool rsv1, bool rsv2, bool rsv3, OpCode opcode, uint32_t masking_key,
                      uint8_t const * data = nullptr, std::size_t size = 0);
    Err updateResponse(bool fin, bool rsv1, bool rsv2, bool rsv3, OpCode opcode,
                       uint8_t const * data = nullptr, std::size_t size = 0);

public:
    Err textRequest(uint32_t masking_key, std::string const & text, bool continuation = false, bool finish = true);
    Err textResponse(std::string const & text, bool continuation = false, bool finish = true);

    Err binaryRequest(uint32_t masking_key, Buffer const & buffer, bool continuation = false, bool finish = true);
    Err binaryResponse(Buffer const & buffer, bool continuation = false, bool finish = true);

// Control Frames.
public:
    /**
     * Closing the connection.
     */
    Err closeRequest();
    Err closeRequest(uint32_t masking_key);
    Err closeResponse(uint16_t status_code, std::string const & reason);

    /**
     * Pings: The Heartbeat of WebSockets.
     *
     * @remarks
     *  for pings and pongs, the max payload length is 125.
     */
    Err pingRequest(uint8_t const * data, std::size_t size);
    Err pingRequest(uint32_t masking_key, uint8_t const * data, std::size_t size);
    Err pingResponse(uint8_t const * data, std::size_t size);

    /**
     * Pongs: The Heartbeat of WebSockets.
     *
     * @remarks
     *  for pings and pongs, the max payload length is 125.
     */
    Err pongRequest(uint8_t const * data, std::size_t size);
    Err pongRequest(uint32_t masking_key, uint8_t const * data, std::size_t size);
    Err pongResponse(uint8_t const * data, std::size_t size);

public:
    std::string toDebugString() const;

public:
    static uint8_t getPayloadDataByteIndex(PayloadBit payload_bit, bool is_mask) TBAG_NOEXCEPT;
    static uint8_t getMaskingKeyByteIndex(PayloadBit payload_bit) TBAG_NOEXCEPT;

    static PayloadBit getPayloadBit(uint8_t payload_length_7bit) TBAG_NOEXCEPT;
    static PayloadBit getPayloadBitWithPayloadLength(uint64_t payload_length) TBAG_NOEXCEPT;
    static uint32_t getMaskingKey(uint8_t const * data) TBAG_NOEXCEPT;

    static std::string getPayloadData(uint32_t mask, std::string const & data);
    static std::vector<uint8_t> getPayloadData(uint32_t mask, std::vector<uint8_t> const & data);
    static std::vector<uint8_t> getPayloadData(uint32_t mask, uint8_t const * data, std::size_t size);
    static void updatePayloadData(uint32_t mask, uint8_t * result, std::size_t size);
};

// ------------------------
// Miscellaneous utilities.
// ------------------------

TBAG_API char const * const getOpCodeName(OpCode code) TBAG_NOEXCEPT;

TBAG_API bool existsWebSocketVersion13(std::string const & versions);

TBAG_API std::string getWebSocketProtocol(std::string const & protocols, std::set<std::string> const & accept_protocols);
TBAG_API std::string getWebSocketProtocolWithTbag(std::string const & protocols);
TBAG_API std::string getWebSocketProtocolValue(std::vector<std::string> const & protocols);

TBAG_API std::string getUpgradeWebSocketKey(std::string const & base64_key);
TBAG_API std::string getRandomWebSocketKey();

TBAG_API Err updateRequestWebSocket(HttpBuilder & request);
TBAG_API Err updateResponseWebSocket(HttpParser const & request, HttpBuilder & response);

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_WEBSOCKETFRAME_HPP__

