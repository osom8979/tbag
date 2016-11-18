/**
 * @file   Zip.cpp
 * @brief  Zip class implementation.
 * @author zer0
 * @date   2016-11-17
 */

#include <libtbag/archive/Zip.hpp>
#include <cassert>
#include <zlib.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace archive {

Zip::Zip()
{
    // EMPTY.
}

Zip::~Zip()
{
    // EMPTY.
}

Zip::ResultCode Zip::coding(Buffer & output, uint8_t const * input, std::size_t size, int level)
{
    bool const ENCODE = (level != DECODE_LEVEL);

    z_stream stream = {0,};
    stream.zalloc   = Z_NULL;
    stream.zfree    = Z_NULL;
    stream.opaque   = Z_NULL;

    int result = Z_OK;
    if (ENCODE) {
        result = deflateInit(&stream, level);
    } else {
        result = inflateInit(&stream);
    }

    if (result != Z_OK) {
        return ResultCode::FAILURE;
    }

    std::size_t const BUFFER_SIZE = 2048; // TODO: CHANGE PAGE SIZE.

    Bytef  in[BUFFER_SIZE] = {0,};
    Bytef out[BUFFER_SIZE] = {0,};

    std::size_t read_index = 0;
    int flush = Z_NO_FLUSH;

    do {
        // Update input buffer.
        stream.next_in = in;
        if (size - read_index > BUFFER_SIZE) {
            stream.avail_in = BUFFER_SIZE;
            flush = Z_NO_FLUSH;
        } else {
            stream.avail_in = static_cast<uInt>(size - read_index);
            flush = Z_FINISH; // EOF.
        }

        memcpy(in, input + read_index, stream.avail_in);
        read_index += stream.avail_in;

        do {
            // Update output buffer.
            stream.avail_out = BUFFER_SIZE;
            stream.next_out  = out;

            if (ENCODE) {
                result = deflate(&stream, flush);
            } else {
                result = inflate(&stream, flush);
            }
            assert(result != Z_STREAM_ERROR);

            std::size_t dsize = BUFFER_SIZE - stream.avail_out;
            output.insert(output.end(), out, out + dsize);
        } while (stream.avail_out == 0);
        assert(stream.avail_in == 0);

    } while (flush != Z_FINISH);
    assert(result == Z_STREAM_END);

    if (ENCODE) {
        deflateEnd(&stream);
    } else {
        inflateEnd(&stream);
    }

    return ResultCode::SUCCESS;
}

Zip::ResultCode Zip::encode(Buffer & output, uint8_t const * input, std::size_t size, int level)
{
    if (level < MIN_ENCODE_LEVEL || level > MAX_ENCODE_LEVEL) {
        level = Z_DEFAULT_COMPRESSION;
    }
    return coding(output, input, size, level);
}

Zip::ResultCode Zip::decode(Buffer & output, uint8_t const * input, std::size_t size)
{
    return coding(output, input, size);
}

} // namespace archive

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

