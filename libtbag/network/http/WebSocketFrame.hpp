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

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

/**
 * @def TBAG_WEB_SOCKET_STATUS_CODE_MAP
 *
 * WebSocket Status Codes
 * Reference: https://tools.ietf.org/html/rfc6455#section-7.4
 */
#ifndef TBAG_WEB_SOCKET_STATUS_CODE_MAP
#define TBAG_WEB_SOCKET_STATUS_CODE_MAP(_TBAG_XX) \
    _TBAG_XX(1000, NORMAL_CLOSURE         , "Normal closure"         ) \
    _TBAG_XX(1001, GOING_AWAY             , "Going away"             ) \
    _TBAG_XX(1002, PROTOCOL_ERROR         , "Protocol error"         ) \
    _TBAG_XX(1003, CANNOT_ACCEPT          , "Cannot accept"          ) \
    _TBAG_XX(1004, RESERVED               , "Reserved"               ) \
    _TBAG_XX(1005, NO_STATUS              , "No status"              ) \
    _TBAG_XX(1006, ABNORMAL_CLOSE         , "Abnormal close"         ) \
    _TBAG_XX(1007, INVALID_PAYLOAD        , "Invalid payload"        ) \
    _TBAG_XX(1008, POLICY_VIOLATION       , "Policy violation"       ) \
    _TBAG_XX(1009, MESSAGE_TOO_BIG        , "Message too big"        ) \
    _TBAG_XX(1010, EXTENSION_REQUIRED     , "Extension required"     ) \
    _TBAG_XX(1011, INTERNAL_ENDPOINT_ERROR, "Internal endpoint error") \
    _TBAG_XX(1015, TLS_HANDSHAKE          , "TLS handshake"          ) \
    _TBAG_XX(3000, UNKNOWN_ERROR          , "Unknown error"          ) \
    _TBAG_XX(3001, CLIENT_TIMER_ERROR     , "Client timer error"     ) \
    /* END */
#endif

TBAG_CONSTEXPR uint16_t const TBAG_UNKNOWN_WEBSOCKET_STATUS_CODE = 0;

/**
 * List of WebSocket status code.
 *
 * @author zer0
 * @date 2017-07-07
 */
enum class WebSocketStatusCode : uint16_t
{
    WSSC_UNKNOWN = TBAG_UNKNOWN_WEBSOCKET_STATUS_CODE,
#define _TBAG_XX(num, name, str) WSSC_##name = num,
    TBAG_WEB_SOCKET_STATUS_CODE_MAP(_TBAG_XX)
#undef _TBAG_XX
};

TBAG_API WebSocketStatusCode getWsStatusCode(uint16_t code) TBAG_NOEXCEPT;
TBAG_API char const * getWsStatusCodeName(WebSocketStatusCode code) TBAG_NOEXCEPT;
TBAG_API char const * getWsStatusCodeReason(WebSocketStatusCode code) TBAG_NOEXCEPT;
TBAG_API uint16_t getWsStatusCodeNumber(WebSocketStatusCode code) TBAG_NOEXCEPT;

/**
 * WebSocket close result structure.
 *
 * @author zer0
 * @date 2017-07-07
 */
struct WsCloseResult
{
    uint16_t    code;
    std::string reason;

    WsCloseResult() : code(0), reason()
    { /* EMPTY. */ }

    WsCloseResult(uint16_t c, std::string const & r) : code(c), reason(r)
    { /* EMPTY. */ }

    WsCloseResult(WebSocketStatusCode s) : code(getWsStatusCodeNumber(s)), reason(getWsStatusCodeReason(s))
    { /* EMPTY. */ }

    ~WsCloseResult()
    { /* EMPTY. */ }

    void set(WebSocketStatusCode s)
    {
        code = getWsStatusCodeNumber(s);
        reason = getWsStatusCodeReason(s);
    }
};

// WebSocket - Reserved Status Code Ranges
// Reference: https://tools.ietf.org/html/rfc6455#section-7.4.2

/** Status codes in the range 0-999 are not used. */
inline bool isWsStatusCodeNotUsed(uint16_t code) TBAG_NOEXCEPT
{
    return 0 <= COMPARE_AND(code) <= 999;
}

/**
 * Status codes in the range 1000-2999 are reserved for definition by
 * this protocol, its future revisions, and extensions specified in a
 * permanent and readily available public specification.
 */
inline bool isWsStatusCodePublicSpecification(uint16_t code) TBAG_NOEXCEPT
{
    return 1000 <= COMPARE_AND(code) <= 2999;
}

/**
 * Status codes in the range 3000-3999 are reserved for use by
 * libraries, frameworks, and applications. These status codes are
 * registered directly with IANA. The interpretation of these codes
 * is undefined by this protocol.
 */
inline bool isWsStatusCodeIANA(uint16_t code) TBAG_NOEXCEPT
{
    return 3000 <= COMPARE_AND(code) <= 3999;
}

/**
 * Status codes in the range 4000-4999 are reserved for private use
 * and thus can't be registered.  Such codes can be used by prior
 * agreements between WebSocket applications.  The interpretation of
 * these codes is undefined by this protocol.
 */
inline bool isWsStatusCodeUserDefined(uint16_t code) TBAG_NOEXCEPT
{
    return 4000 <= COMPARE_AND(code) <= 4999;
}

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

enum class PayloadBit : uint8_t
{
    PL_BIT_7  =  7,
    PL_BIT_16 = 16,
    PL_BIT_64 = 64,
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
    using WsBuffer = std::vector<uint8_t>;

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
    WsBuffer payload;

public:
    WebSocketFrame();
    WebSocketFrame(WebSocketFrame const & obj);
    WebSocketFrame(WebSocketFrame && obj);
    virtual ~WebSocketFrame();

public:
    WebSocketFrame & operator =(WebSocketFrame const & obj);
    WebSocketFrame & operator =(WebSocketFrame && obj);

public:
    inline uint8_t const * getPayloadDataPtr() const TBAG_NOEXCEPT
    { return payload.data(); }

    inline std::size_t getPayloadSize() const TBAG_NOEXCEPT
    { return static_cast<std::size_t>(payload_length); }

    inline std::string toText() const
    { return std::string(payload.data(), payload.data() + payload_length); }

    inline WsBuffer toBinary() const
    { return WsBuffer(payload.data(), payload.data() + payload_length); }

public:
    void clear();

public:
    Err execute(uint8_t const * data, std::size_t size);

public:
    std::size_t calculateWriteBufferSize() const;
    std::size_t copyTo(uint8_t * data, std::size_t data_size) const;
    std::size_t copyTo(WsBuffer & buffer) const;

public:
    void setHeader(bool f, bool r1, bool r2, bool r3, OpCode op, uint32_t key = 0) TBAG_NOEXCEPT;
    void setData(uint8_t const * data, std::size_t size) TBAG_NOEXCEPT;

public:
    Err build(bool fin, bool rsv1, bool rsv2, bool rsv3, OpCode opcode,
              uint8_t const * data = nullptr, std::size_t size = 0,
              uint32_t key = 0);

public:
    Err text(std::string const & str, uint32_t key, bool continuation = false, bool finish = true);
    Err text(std::string const & str, bool continuation = false, bool finish = true);

    Err binary(WsBuffer const & buffer, uint32_t key, bool continuation = false, bool finish = true);
    Err binary(WsBuffer const & buffer, bool continuation = false, bool finish = true);

// Control Frames.
public:
    /**
     * Closing the connection.
     */
    Err close(uint32_t key);
    Err close(uint16_t code, std::string const & reason);
    Err close(WebSocketStatusCode code);

    uint16_t getStatusCode() const;
    std::string getReason() const;

    WsCloseResult getCloseResult() const;

    /**
     * Pings: The Heartbeat of WebSockets.
     *
     * @remarks
     *  for pings and pongs, the max payload length is 125.
     */
    Err ping(uint8_t const * data, std::size_t size, uint32_t key = 0);
    Err ping(std::string const & str, uint32_t key = 0);

    /**
     * Pongs: The Heartbeat of WebSockets.
     *
     * @remarks
     *  for pings and pongs, the max payload length is 125.
     */
    Err pong(uint8_t const * data, std::size_t size, uint32_t key = 0);
    Err pong(std::string const & str, uint32_t key = 0);

public:
    std::string toDebugString() const;

public:
    /**
     * @remarks
     *  The length of the "Payload data", in bytes: if 0-125, that is the
     *  payload length.  If 126, the following 2 bytes interpreted as a
     *  16-bit unsigned integer are the payload length.  If 127, the
     *  following 8 bytes interpreted as a 64-bit unsigned integer (the
     *  most significant bit MUST be 0) are the payload length.  Multibyte
     *  length quantities are expressed in network byte order.  Note that
     *  in all cases, the minimal number of bytes MUST be used to encode
     *  the length, for example, the length of a 124-byte-long string
     *  can't be encoded as the sequence 126, 0, 124.  The payload length
     *  is the length of the "Extension data" + the length of the
     *  "Application data".  The length of the "Extension data" may be
     *  zero, in which case the payload length is the length of the
     *  "Application data".
     *
     * @see <https://tools.ietf.org/html/rfc6455#section-5.2>
     */
    static PayloadBit getPayloadBit(uint8_t payload_length_7bit) TBAG_NOEXCEPT;
    static PayloadBit getPayloadBitWithPayloadLength(uint64_t payload_length) TBAG_NOEXCEPT;

    static uint8_t getPayloadDataByteIndex(PayloadBit payload_bit, bool is_mask) TBAG_NOEXCEPT;
    static uint8_t getMaskingKeyByteIndex(PayloadBit payload_bit) TBAG_NOEXCEPT;

    static uint32_t getMaskingKey(uint8_t const * data) TBAG_NOEXCEPT;

    static std::string getPayloadData(uint32_t mask, std::string const & data);
    static WsBuffer getPayloadData(uint32_t mask, WsBuffer const & data);
    static WsBuffer getPayloadData(uint32_t mask, uint8_t const * data, std::size_t size);
    static void updatePayloadData(uint32_t mask, uint8_t * result, std::size_t size);
};

// ------------------------
// Miscellaneous utilities.
// ------------------------

TBAG_API char const * getOpCodeName(OpCode code) TBAG_NOEXCEPT;

TBAG_API bool existsWebSocketVersion13(std::string const & versions);

TBAG_API std::string getUpgradeWebSocketKey(std::string const & base64_key);
TBAG_API std::string generateRandomWebSocketKey();

TBAG_API Err updateRequestWebSocket(HttpBuilder & request, std::string const & key);
TBAG_API Err updateResponseWebSocket(HttpParser const & request, HttpBuilder & response);

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_WEBSOCKETFRAME_HPP__

