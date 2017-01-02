/**
 * @file   DatagramAdapter.hpp
 * @brief  DatagramAdapter class prototype.
 * @author zer0
 * @date   2017-01-02
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DATAGRAMADAPTER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DATAGRAMADAPTER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/debug/ErrorCode.hpp>
#include <libtbag/container/CircularBuffer.hpp>
#include <libtbag/uv/Request.hpp>

#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {

// Forward declaration.
class CommonTcp;

/**
 * DatagramAdapter class prototype.
 *
 * @author zer0
 * @date   2017-01-02
 */
class TBAG_API DatagramAdapter : public Noncopyable
{
public:
    using CircularBuffer = container::CircularBuffer<char>;
    using Buffer = std::vector<char>;
    using Size   = std::size_t;

    using WriteRequest = uv::WriteRequest;
    using binf         = uv::binf;

public:
    struct Callback
    {
        virtual void onDatagramRead(Err code, char const * buffer, std::size_t size) = 0;
    };

private:
    Buffer _write_buffer;
    Size   _write_size;

private:
    Buffer _read_buffer;
    Size   _next_read_size;

private:
    CircularBuffer _data_buffer;

private:
    Callback * _callback;

public:
    DatagramAdapter(Callback * callback = nullptr);
    virtual ~DatagramAdapter();

public:
    binf writeDatagram(char const * buffer, std::size_t size);
    std::size_t readNextDatagramSize();
    void clearNextDatagramSize();

public:
    WriteRequest * asyncWrite(CommonTcp & tcp, char const * buffer, std::size_t size);
    std::size_t tryWrite(CommonTcp & tcp, char const * buffer, std::size_t size, Err * result = nullptr);

// Event by-pass methods.
public:
    binf bypassOnAlloc(Buffer & buffer, std::size_t suggested_size);
    void bypassOnRead(Err code, char const * buffer, std::size_t size);
};

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DATAGRAMADAPTER_HPP__

