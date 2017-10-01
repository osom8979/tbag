/**
 * @file   WsFrame.hpp
 * @brief  WsFrame class prototype.
 * @author zer0
 * @date   2017-06-11
 * @date   2017-08-07 (Rename: WebSocketFrame -> WsFrame)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_WSFRAME_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_WSFRAME_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>

#include <libtbag/network/http/ws/WsCommon.hpp>
#include <libtbag/network/http/ws/WsStatus.hpp>
#include <libtbag/network/http/ws/WsOpCode.hpp>
#include <libtbag/network/http/ws/WsPyloadBit.hpp>
#include <libtbag/network/http/ws/WsUtils.hpp>

#include <libtbag/network/http/HttpProperty.hpp>
#include <libtbag/network/http/HttpParser.hpp>
#include <libtbag/network/http/HttpBuilder.hpp>
#include <libtbag/util/BufferInfo.hpp>

#include <cstdint>
#include <vector>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

using namespace libtbag::network::http::ws;

/**
 * WsFrame class prototype.
 *
 * @author zer0
 * @date   2017-06-11
 * @date   2017-08-07 (Rename: WebSocketFrame -> WsFrame)
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
class TBAG_API WsFrame
{
public:
    using Buffer = util::Buffer;

public:
    TBAG_CONSTEXPR static std::size_t const MINIMUM_BUFFER_SIZE = 2;

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
    WsOpCode opcode;

    /** Defines whether the "Payload data" is masked. */
    bool mask;

    /** payload length. */
    uint64_t payload_length;

    /** Masking-key, if MASK set to 1. */
    uint32_t masking_key;

    /** Payload buffer. */
    Buffer payload;

public:
    WsFrame();
    WsFrame(WsFrame const & obj);
    WsFrame(WsFrame && obj);
    virtual ~WsFrame();

public:
    WsFrame & operator =(WsFrame const & obj);
    WsFrame & operator =(WsFrame && obj);

public:
    inline char const * getPayloadDataPtr() const TBAG_NOEXCEPT
    { return payload.data(); }

    inline std::size_t getPayloadSize() const TBAG_NOEXCEPT
    { return static_cast<std::size_t>(payload_length); }

    inline std::string toText() const
    { return std::string(payload.data(), payload.data() + payload_length); }

    inline Buffer toBinary() const
    { return Buffer(payload.data(), payload.data() + payload_length); }

public:
    void clear();

public:
    Err execute(char const * data, std::size_t size, std::size_t * read_size = nullptr);

public:
    std::size_t calculateWriteBufferSize() const;
    std::size_t copyTo(char * data, std::size_t data_size) const;
    std::size_t copyTo(Buffer & buffer) const;

public:
    void setHeader(bool f, bool r1, bool r2, bool r3, WsOpCode op, uint32_t key = 0) TBAG_NOEXCEPT;
    void setData(char const * data, std::size_t size);

public:
    Err build(bool fin, bool rsv1, bool rsv2, bool rsv3, WsOpCode opcode,
              char const * data = nullptr, std::size_t size = 0,
              uint32_t key = 0);

public:
    Err text(char const * buffer, std::size_t size, uint32_t key, bool continuation = false, bool finish = true);
    Err text(char const * buffer, std::size_t size, bool continuation = false, bool finish = true);

    Err text(std::string const & str, uint32_t key, bool continuation = false, bool finish = true);
    Err text(std::string const & str, bool continuation = false, bool finish = true);

    Err binary(char const * buffer, std::size_t size, uint32_t key, bool continuation = false, bool finish = true);
    Err binary(char const * buffer, std::size_t size, bool continuation = false, bool finish = true);

    Err binary(Buffer const & buffer, uint32_t key, bool continuation = false, bool finish = true);
    Err binary(Buffer const & buffer, bool continuation = false, bool finish = true);

// Control Frames.
public:
    /**
     * Closing the connection.
     */
    Err close(uint32_t key);
    Err close(uint16_t code, std::string const & reason);
    Err close(WsStatusCode code);

    uint16_t getStatusCode() const;
    std::string getReason() const;

    WsStatus getCloseResult() const;

    /**
     * Pings: The Heartbeat of WebSockets.
     *
     * @remarks
     *  for pings and pongs, the max payload length is 125.
     */
    Err ping(char const * data, std::size_t size, uint32_t key = 0);
    Err ping(std::string const & str, uint32_t key = 0);

    /**
     * Pongs: The Heartbeat of WebSockets.
     *
     * @remarks
     *  for pings and pongs, the max payload length is 125.
     */
    Err pong(char const * data, std::size_t size, uint32_t key = 0);
    Err pong(std::string const & str, uint32_t key = 0);

public:
    std::string toDebugString() const;
};

// ------------------------
// Miscellaneous utilities.
// ------------------------

TBAG_API bool existsWebSocketVersion13(std::string const & versions);

TBAG_API Err updateRequestWebSocket(HttpBuilder & request, std::string const & key);
TBAG_API Err updateResponseWebSocket(HttpParser const & request, HttpBuilder & response);

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_WSFRAME_HPP__

