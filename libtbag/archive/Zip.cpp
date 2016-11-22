/**
 * @file   Zip.cpp
 * @brief  Zip class implementation.
 * @author zer0
 * @date   2016-11-17
 */

#include <libtbag/archive/Zip.hpp>

#include <cassert>
#include <cstring>

#include <fstream>
#include <iostream>

#include <zlib.h>
#include <zip.h>
#include <unzip.h>

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
    return coding(output, input, size, DECODE_LEVEL);
}

Zip::ResultCode Zip::zip(std::string const & file, std::string const & dir)
{
    return ResultCode::FAILURE;
}

Zip::ResultCode Zip::unzip(std::string const & file, std::string const & dir)
{
    std::cout << "1";
    unzFile uf = unzOpen(file.c_str());
    if (uf == nullptr) {
        return ResultCode::OPEN_ERROR;
    }
    std::cout << "2";

    if (unzGoToFirstFile(uf) != UNZ_OK) {
        unzClose(uf);
        return ResultCode::GO_TO_FIRST_FILE_ERROR;
    }
    std::cout << "3";

    std::size_t const MAX_PATH    = 256;
    std::size_t const MAX_COMMENT = 256;

    char filename[MAX_PATH] = {0,};
    char  comment[MAX_COMMENT] = {0,};

    unz_file_info info;
    unzGetCurrentFileInfo(uf, &info, filename, MAX_PATH, nullptr, 0, comment, MAX_COMMENT);

    // Filename: filename
    // Comment: comment
    // Compressed size: info.compressed_size
    // Uncompressed size: info.uncompressed_size

    std::cout<<"filename:"<< filename <<" Comment:"<<comment<<std::endl;
    std::cout<<" compressed_size:"<< info.compressed_size<<" uncompressed_size:"<< info.uncompressed_size <<std::endl;

    if (unzOpenCurrentFile(uf) != UNZ_OK) {
        unzClose(uf);
        return ResultCode::OPEN_CURRENT_FILE_ERROR;
    }

    const std::size_t BUFFER =1024;
    Bytef in[BUFFER] = {0,};
    std::size_t readsize = 0;

    std::ofstream op;
    op.open(filename, std::ios_base::binary);

    do {
        readsize = unzReadCurrentFile(uf,(void*) in, BUFFER);
        op.write((char const *) in, readsize);
    } while (readsize != 0);

    op.close();
    unzCloseCurrentFile(uf);
    unzClose(uf);

    return ResultCode::FAILURE;
}

} // namespace archive

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

