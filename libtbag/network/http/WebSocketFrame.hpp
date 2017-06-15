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
    using Buffer = std::vector<uint8_t>;

private:
    /**
     * Indicates that this is the final fragment in a message.
     * The first fragment MAY also be the final fragment.
     */
    bool _fin;

    /** MUST be 0 unless an extension is negotiated that defines meanings for non-zero values. */
    bool _rsv1;

    /** MUST be 0 unless an extension is negotiated that defines meanings for non-zero values. */
    bool _rsv2;

    /** MUST be 0 unless an extension is negotiated that defines meanings for non-zero values. */
    bool _rsv3;

    /** Defines the interpretation of the "Payload data". */
    OpCode _opcode;

    /** Defines whether the "Payload data" is masked. */
    bool _mask;

    /** payload length. */
    uint64_t _payload_length;

    /** Masking-key, if MASK set to 1. */
    uint32_t _masking_key;

private:
    Buffer _payload_buffer;

public:
    inline uint8_t const * getPayloadDataPointer() const TBAG_NOEXCEPT
    { return _payload_buffer.data(); }

    inline std::size_t getPayloadDataSize() const TBAG_NOEXCEPT
    { return static_cast<std::size_t>(_payload_length); }

public:
    WebSocketFrame();
    WebSocketFrame(WebSocketFrame const & obj);
    WebSocketFrame(WebSocketFrame && obj);
    virtual ~WebSocketFrame();

public:
    WebSocketFrame & operator =(WebSocketFrame const & obj);
    WebSocketFrame & operator =(WebSocketFrame && obj);

public:
    inline bool getFin() const TBAG_NOEXCEPT { return _fin; }
    inline void setFin(bool flag) TBAG_NOEXCEPT { _fin = flag; }

    inline bool getRsv1() const TBAG_NOEXCEPT { return _rsv1; }
    inline void setRsv1(bool flag) TBAG_NOEXCEPT { _rsv1 = flag; }

    inline bool getRsv2() const TBAG_NOEXCEPT { return _rsv2; }
    inline void setRsv2(bool flag) TBAG_NOEXCEPT { _rsv2 = flag; }

    inline bool getRsv3() const TBAG_NOEXCEPT { return _rsv3; }
    inline void setRsv3(bool flag) TBAG_NOEXCEPT { _rsv3 = flag; }

    inline OpCode getOpCode() const TBAG_NOEXCEPT { return _opcode; }
    inline void setOpCode(OpCode code) TBAG_NOEXCEPT { _opcode = code; }

    inline bool getMask() const TBAG_NOEXCEPT { return _mask; }
    inline void setMask(bool flag) TBAG_NOEXCEPT { _mask = flag; }

    inline uint64_t getPayloadLength() const TBAG_NOEXCEPT { return _payload_length; }
    inline void setPayloadLength(uint64_t size) TBAG_NOEXCEPT { _payload_length = size; }

    inline uint32_t getMaskingKey() const TBAG_NOEXCEPT { return _masking_key; }
    inline void setMaskingKey(uint32_t key) TBAG_NOEXCEPT { _masking_key = key; }

public:
    Err execute(uint8_t const * data, std::size_t size);

public:
    std::size_t calculateWriteBufferSize() const;
    std::size_t write(uint8_t * data, std::size_t data_size);
    std::size_t write(Buffer & buffer);

public:
    bool updateRequest(bool fin, bool rsv1, bool rsv2, bool rsv3, OpCode opcode, uint32_t masking_key, uint8_t const * data, std::size_t size);
    bool updateResponse(bool fin, bool rsv1, bool rsv2, bool rsv3, OpCode opcode, uint8_t const * data, std::size_t size);
};

// ----------
// Utilities.
// ----------

TBAG_API uint8_t getPayloadDataByteIndex(PayloadBit payload_bit, bool is_mask) TBAG_NOEXCEPT;
TBAG_API uint8_t getMaskingKeyByteIndex(PayloadBit payload_bit) TBAG_NOEXCEPT;

TBAG_API PayloadBit getPayloadBit(uint8_t payload_length_7bit) TBAG_NOEXCEPT;
TBAG_API PayloadBit getPayloadBitWithPayloadLength(uint64_t payload_length) TBAG_NOEXCEPT;
TBAG_API uint32_t getMaskingKey(uint8_t const * data) TBAG_NOEXCEPT;

TBAG_API std::string getPayloadData(uint32_t mask, std::string const & data);
TBAG_API std::vector<uint8_t> getPayloadData(uint32_t mask, std::vector<uint8_t> const & data);
TBAG_API std::vector<uint8_t> getPayloadData(uint32_t mask, uint8_t const * data, std::size_t size);
TBAG_API void updatePayloadData(uint32_t mask, uint8_t * result, std::size_t size);

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_WEBSOCKETFRAME_HPP__

