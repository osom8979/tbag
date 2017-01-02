/**
 * @file   DatagramTcp.hpp
 * @brief  DatagramTcp class prototype.
 * @author zer0
 * @date   2017-01-02
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DATAGRAMTCP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DATAGRAMTCP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/network/CommonTcp.hpp>
#include <libtbag/container/CircularBuffer.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {

/**
 * DatagramTcp class prototype.
 *
 * @author zer0
 * @date   2017-01-02
 */
class TBAG_API DatagramTcp : public CommonTcp
{
public:
    using Parent = CommonTcp;

public:
    using CircularBuffer = container::CircularBuffer<char>;

private:
    Buffer      _write_buffer;
    std::size_t _write_size;

private:
    Buffer      _temp_buffer;
    std::size_t _next_read_size;

private:
    CircularBuffer _data_buffer;

public:
    DatagramTcp();
    DatagramTcp(SharedTcp tcp);
    DatagramTcp(CallableTcp * tcp);
    virtual ~DatagramTcp();

public:
    void writeDatagram(char const * buffer, std::size_t size);
    std::size_t readNextDatagramSize();
    void clearNextDatagramSize();

public:
    WriteRequest * asyncWriteDatagram(char const * buffer, std::size_t size);
    std::size_t tryWriteDatagram(char const * buffer, std::size_t size, Err * result = nullptr);

// Event methods.
public:
    virtual binf onAlloc(std::size_t suggested_size) override;
    virtual void onRead(Err code, char const * buffer, std::size_t size) override;

public:
    virtual void onDatagramRead(Err code, char const * buffer, std::size_t size);
};

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DATAGRAMTCP_HPP__

