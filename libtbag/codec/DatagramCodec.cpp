/**
 * @file   DatagramCodec.cpp
 * @brief  DatagramCodec class implementation.
 * @author zer0
 * @date   2017-01-02
 * @date   2017-05-13 (Rename: DatagramAdapter -> DatagramCodec)
 */

#include <libtbag/codec/DatagramCodec.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace codec {

// -------------------------------
// DatagramEncoder implementation.
// -------------------------------

DatagramEncoder::DatagramEncoder()
{
    // EMPTY.
}

DatagramEncoder::~DatagramEncoder()
{
    // EMPTY.
}

bool DatagramEncoder::pushWriteBuffer(char const * buffer, Size size)
{
    Guard guard(_writers_mutex);

    SharedBuffer * shared = _writers.push();
    if (shared == nullptr || static_cast<bool>(shared) == false) {
        return false;
    }

    if ((*shared).get() == nullptr) {
        shared->reset(new (std::nothrow) Buffer(DATAGRAM_HEADER_SIZE + size));
        if ((*shared).get() == nullptr) {
            return false;
        }
    }

    Buffer & cursor = *(shared->get());
    cursor.resize(DATAGRAM_HEADER_SIZE + size);

    Size const NETWORK_BYTE_SIZE = toNetwork(static_cast<Size>(size));
    ::memcpy(&cursor[0], (char const *)&NETWORK_BYTE_SIZE, DATAGRAM_HEADER_SIZE);
    ::memcpy(&cursor[DATAGRAM_HEADER_SIZE], buffer, size);

    return true;
}

DatagramEncoder::Size DatagramEncoder::parseBufferSize(char const * buffer, Size size)
{
    if (size < DATAGRAM_HEADER_SIZE) {
        return NO_NEXT_READ_SIZE;
    }

    Size network_byte_size = 0;
    ::memcpy((char*)&network_byte_size, buffer, DATAGRAM_HEADER_SIZE);
    return toHost(network_byte_size);
}

// -------------------------------
// DatagramDecoder implementation.
// -------------------------------

DatagramDecoder::DatagramDecoder() : _next_read_size(NO_NEXT_READ_SIZE)
{
    // EMPTY.
}

DatagramDecoder::~DatagramDecoder()
{
    // EMPTY.
}

DatagramDecoder::Size DatagramDecoder::readNextDatagramSize()
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
    Size network_byte_size = 0;
    _data_buffer.pop((char*)&network_byte_size, DATAGRAM_HEADER_SIZE);
    _next_read_size = toHost(network_byte_size);

    return _next_read_size;
}

// ----------------------
// Event by-pass methods.
// ----------------------

void DatagramDecoder::alloc(Size suggested_size)
{
    Guard guard(_read_mutex);
    if (_data_buffer.capacity() < suggested_size) {
        _data_buffer.extendCapacity(suggested_size - _data_buffer.capacity());
    }
}

void DatagramDecoder::push(char const * buffer, Size size)
{
    Guard guard(_read_mutex);
    _data_buffer.extendPush(buffer, size);
}

bool DatagramDecoder::next(binf * result)
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

DatagramCodec::DatagramCodec()
{
    // EMPTY.
}

DatagramCodec::~DatagramCodec()
{
    // EMPTY.
}

} // namespace codec

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

