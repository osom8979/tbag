/**
 * @file   WsFrame.hpp
 * @brief  WsFrame class prototype.
 * @author zer0
 * @date   2017-10-01
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_WS_WSFRAME_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_WS_WSFRAME_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>

#include <libtbag/util/BufferInfo.hpp>
#include <libtbag/network/http/HttpCommon.hpp>
#include <libtbag/network/http/ws/WsOpCode.hpp>
#include <libtbag/network/http/ws/WsStatus.hpp>
#include <libtbag/network/http/ws/WsPyloadBit.hpp>
#include <libtbag/network/http/ws/WsUtils.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {
namespace ws      {

/**
 * WsFrame class prototype.
 *
 * @author zer0
 * @date   2017-06-11
 * @date   2017-08-07 (Rename: WebSocketFrame -> WsFrame)
 * @date   2017-10-01 (Move namespace: libtbag::network::http -> libtbag::network::http::ws)
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
struct TBAG_API WsFrame
{
public:
    TBAG_CONSTEXPR static std::size_t const MINIMUM_BUFFER_SIZE = WEBSOCKET_MINIMUM_HEADER_BYTE_SIZE;

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
    util::Buffer payload;

public:
    WsFrame();
    virtual ~WsFrame();

public:
    void swap(WsFrame & frame);

public:
    inline char const * getPayloadPtr() const TBAG_NOEXCEPT { return payload.data(); }
    inline std::size_t getPayloadSize() const TBAG_NOEXCEPT { return static_cast<std::size_t>(payload_length); }

    inline std::string toPayloadString() const
    { return std::string(payload.data(), payload.data() + payload_length); }

    inline util::Buffer toPayloadBinary() const
    { return util::Buffer(payload.data(), payload.data() + payload_length); }

public:
    void clear();

public:
    Err execute(char const * data, std::size_t size, std::size_t * read_size = nullptr);

public:
    std::size_t copyTo(char * buffer, std::size_t data_size) const;
    std::size_t copyTo(util::Buffer & buffer) const;

public:
    void setHeader(bool f, bool r1, bool r2, bool r3, WsOpCode op, uint32_t key = 0) TBAG_NOEXCEPT;
    void setData(char const * data, std::size_t size);

public:
    void set(bool fin, bool rsv1, bool rsv2, bool rsv3, WsOpCode opcode,
             char const * data = nullptr, std::size_t size = 0, uint32_t key = 0);

public:
    void text(char const * buffer, std::size_t size, uint32_t key, bool finish = true);
    void text(char const * buffer, std::size_t size, bool finish = true);
    void text(std::string const & str, uint32_t key, bool finish = true);
    void text(std::string const & str, bool finish = true);

public:
    void binary(char const * buffer, std::size_t size, uint32_t key, bool finish = true);
    void binary(char const * buffer, std::size_t size, bool finish = true);
    void binary(util::Buffer const & buffer, uint32_t key, bool finish = true);
    void binary(util::Buffer const & buffer, bool finish = true);

public:
    /**
     * Closing the connection.
     */
    void close(uint32_t key);
    void close(uint16_t code, std::string const & reason);
    void close(WsStatusCode code);

    WsStatus getWsStatus() const;

public:
    /**
     * Pings: The Heartbeat of WebSockets.
     *
     * @remarks
     *  for pings and pongs, the max payload length is 125.
     */
    void ping(char const * data, std::size_t size, uint32_t key = 0);
    void ping(std::string const & str, uint32_t key = 0);

public:
    /**
     * Pongs: The Heartbeat of WebSockets.
     *
     * @remarks
     *  for pings and pongs, the max payload length is 125.
     */
    void pong(char const * data, std::size_t size, uint32_t key = 0);
    void pong(std::string const & str, uint32_t key = 0);

public:
    std::string toDebugString(bool print_payload = false) const;
};

} // namespace ws
} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_WS_WSFRAME_HPP__

