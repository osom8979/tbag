/**
 * @file   DatagramAdapter.cpp
 * @brief  DatagramAdapter class implementation.
 * @author zer0
 * @date   2017-01-02
 */

#include <libtbag/network/DatagramAdapter.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/network/CommonTcp.hpp>

#include <cstdlib>
#include <cstdint>
#include <limits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {

static std::size_t const NO_NEXT_READ_SIZE    = std::numeric_limits<std::size_t>::max();
static std::size_t const DATAGRAM_HEADER_SIZE = sizeof(uint32_t);

DatagramAdapter::DatagramAdapter(Callback * callback) : _write_size(0), _next_read_size(NO_NEXT_READ_SIZE), _callback(callback)
{
    // EMPTY.
}

DatagramAdapter::~DatagramAdapter()
{
    // EMPTY.
}

DatagramAdapter::binf DatagramAdapter::writeDatagram(char const * buffer, std::size_t size)
{
    // Realloc with read buffer.
    if (_write_buffer.size() < size) {
        _write_buffer.resize(DATAGRAM_HEADER_SIZE + size);
    }

    uint32_t host_byte_size    = (uint32_t)size;
    uint32_t network_byte_size = htonl(host_byte_size);

    ::memcpy(&_write_buffer[0], (char*)&network_byte_size, DATAGRAM_HEADER_SIZE);
    ::memcpy(&_write_buffer[DATAGRAM_HEADER_SIZE], buffer, size);

    binf result;
    result.buffer = &_write_buffer[0];
    result.size = size + DATAGRAM_HEADER_SIZE;
    return result;
}

std::size_t DatagramAdapter::readNextDatagramSize()
{
    // Section 01: Exists next_read_size.
    if (_next_read_size != NO_NEXT_READ_SIZE) {
        return _next_read_size;
    }

    // Section 02: Find the next_read_size in buffer.
    if (_data_buffer.size() < DATAGRAM_HEADER_SIZE) {
        return NO_NEXT_READ_SIZE;
    }

    // Section 03: Update next_read_size.
    uint32_t network_byte_size = 0;
    _data_buffer.pop((char*)&network_byte_size, DATAGRAM_HEADER_SIZE);
    _next_read_size = ntohl(network_byte_size);

    return _next_read_size;
}

void DatagramAdapter::clearNextDatagramSize()
{
    _next_read_size = NO_NEXT_READ_SIZE;
}

DatagramAdapter::WriteRequest * DatagramAdapter::asyncWrite(CommonTcp & tcp, char const * buffer, std::size_t size)
{
    binf buffer_info = writeDatagram(buffer, size);
    return tcp.asyncWrite(&buffer_info, 1);
}

std::size_t DatagramAdapter::tryWrite(CommonTcp & tcp, char const * buffer, std::size_t size, Err * result)
{
    binf buffer_info = writeDatagram(buffer, size);
    return tcp.tryWrite(&buffer_info, 1);
}

// ----------------------
// Event by-pass methods.
// ----------------------

void DatagramAdapter::bypassOnAlloc(std::size_t suggested_size)
{
    _data_buffer.resize(suggested_size * 2);
}

void DatagramAdapter::bypassOnRead(Err code, char const * buffer, std::size_t size)
{
    if (code != Err::SUCCESS) {
        if (_callback != nullptr) {
            _callback->onDatagramRead(code, buffer, size); // ERROR CASE.
        }
        return;
    }

    _data_buffer.push(buffer, size);

    while (true) {
        std::size_t const READ_SIZE = readNextDatagramSize();
        if (READ_SIZE == NO_NEXT_READ_SIZE) {
            break;
        }

        if (_data_buffer.size() >= READ_SIZE) {
            // Realloc with read buffer.
            if (_read_buffer.size() < READ_SIZE) {
                _read_buffer.resize(READ_SIZE);
            }

            std::size_t real_read_size = _data_buffer.pop(&_read_buffer[0], READ_SIZE);
            if (real_read_size != READ_SIZE) {
                code = Err::READ_ERROR;
            }

            if (_callback != nullptr) {
                _callback->onDatagramRead(code, &_read_buffer[0], real_read_size);
            }
            clearNextDatagramSize();
        } else {
            break;
        }
    }
}

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

