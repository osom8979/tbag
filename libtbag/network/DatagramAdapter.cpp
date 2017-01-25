/**
 * @file   DatagramAdapter.cpp
 * @brief  DatagramAdapter class implementation.
 * @author zer0
 * @date   2017-01-02
 */

#include <libtbag/network/DatagramAdapter.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {

// -----------------------------
// DatagramWrite implementation.
// -----------------------------

DatagramWrite::DatagramWrite()
{
    // EMPTY.
}

DatagramWrite::~DatagramWrite()
{
    // EMPTY.
}

bool DatagramWrite::pushWriteBuffer(char const * buffer, Size size)
{
    Guard guard(_writers_mutex);

    SharedBuffer * shared = _writers.push();
    if (shared == nullptr || static_cast<bool>(shared) == false) {
        return false;
    }

    Buffer & cursor = *(shared->get());

    // Realloc with read buffer.
    if (cursor.size() < size) {
        cursor.resize(DATAGRAM_HEADER_SIZE + size);
    }

    uint32_t network_byte_size = toNetwork((uint32_t)size/* Host byte size. */);

    ::memcpy(&cursor[0], (char*)&network_byte_size, DATAGRAM_HEADER_SIZE);
    ::memcpy(&cursor[DATAGRAM_HEADER_SIZE], buffer, size);

    return true;
}

// ----------------------------
// DatagramRead implementation.
// ----------------------------

DatagramRead::DatagramRead() : _next_read_size(NO_NEXT_READ_SIZE)
{
    // EMPTY.
}

DatagramRead::~DatagramRead()
{
    // EMPTY.
}

DatagramRead::Size DatagramRead::readNextDatagramSize()
{
    // WARNING: Don't use the read_mutex.

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
    _next_read_size = toHost(network_byte_size);

    return _next_read_size;
}

// ----------------------
// Event by-pass methods.
// ----------------------

void DatagramRead::alloc(Size suggested_size)
{
    Guard guard(_read_mutex);
    if (_data_buffer.max() < suggested_size) {
        _data_buffer.extendCapacity(suggested_size - _data_buffer.max());
    }
}

void DatagramRead::push(char const * buffer, Size size)
{
    Guard guard(_read_mutex);
    _data_buffer.extendPush(buffer, size);
}

bool DatagramRead::next(binf * result)
{
    Guard guard(_read_mutex);
    Size const READ_SIZE = readNextDatagramSize();
    if (READ_SIZE == NO_NEXT_READ_SIZE) {
        return false;
    }

    if (_data_buffer.size() < READ_SIZE) {
        return false;
    }

    // Realloc with read buffer.
    if (_read_buffer.size() < READ_SIZE) {
        _read_buffer.resize(READ_SIZE);
    }

    Size real_read_size = _data_buffer.pop(&_read_buffer[0], READ_SIZE);
    if (real_read_size != READ_SIZE) {
        return false;
    }

    if (result != nullptr) {
        result->buffer = &_read_buffer[0];
        result->size   = real_read_size;
    }

    // Clear next datagram size.
    _next_read_size = NO_NEXT_READ_SIZE;

    return true;
}

// -------------------------------
// DatagramAdapter implementation.
// -------------------------------

DatagramAdapter::DatagramAdapter()
{
    // EMPTY.
}

DatagramAdapter::~DatagramAdapter()
{
    // EMPTY.
}

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

