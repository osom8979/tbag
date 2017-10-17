/**
 * @file   Zip.cpp
 * @brief  Zip class implementation.
 * @author zer0
 * @date   2016-11-17
 */

#include <libtbag/archive/Zip.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/filesystem/File.hpp>

#include <cassert>
#include <cstring>

#include <zlib.h>
#include <zip.h>
#include <unzip.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace archive {

Err coding(char const * input, std::size_t size, util::Buffer & output, int level)
{
    bool const ENCODE = (level != TBAG_ZIP_DECODE_LEVEL);

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
        return Err::E_EINIT;
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

        ::memcpy(in, input + read_index, stream.avail_in);
        read_index += stream.avail_in;

        do {
            // Update output buffer.
            stream.avail_out = BUFFER_SIZE;
            stream.next_out  = out;

            if (ENCODE) {
                result = ::deflate(&stream, flush);
            } else {
                result = ::inflate(&stream, flush);
            }
            assert(result != Z_STREAM_ERROR);

            std::size_t dsize = BUFFER_SIZE - stream.avail_out;
            output.insert(output.end(), out, out + dsize);
        } while (stream.avail_out == 0);
        assert(stream.avail_in == 0);

    } while (flush != Z_FINISH);
    assert(result == Z_STREAM_END);

    if (ENCODE) {
        ::deflateEnd(&stream);
    } else {
        ::inflateEnd(&stream);
    }

    return Err::E_SUCCESS;
}

Err encode(char const * input, std::size_t size, util::Buffer & output, int level)
{
    if (level < TBAG_ZIP_MIN_ENCODE_LEVEL || level > TBAG_ZIP_MAX_ENCODE_LEVEL) {
        level = Z_DEFAULT_COMPRESSION;
    }
    return coding(input, size, output, level);
}

Err decode(char const * input, std::size_t size, util::Buffer & output)
{
    return coding(input, size, output, TBAG_ZIP_DECODE_LEVEL);
}

Err zip(std::string const & path, std::string const & output_dir)
{
    return Err::E_ENOSYS;
}

Err unzip(std::string const & path, std::string const & output_dir)
{
    unzFile uf = unzOpen(path.c_str());
    if (uf == nullptr) {
        return Err::E_EOPEN;
    }

    if (unzGoToFirstFile(uf) != UNZ_OK) {
        unzClose(uf);
        return Err::E_EINDEX;
    }

    std::size_t const MAX_PATH_LENGTH = filesystem::details::MAX_PATH_LENGTH;
    std::size_t const MAX_COMMENT     = filesystem::details::MAX_PATH_LENGTH;

    char filename[MAX_PATH_LENGTH] = {0,};
    char  comment[MAX_COMMENT] = {0,};

    std::size_t const INPUT_BUFFER = 2048;
    Bytef in[INPUT_BUFFER] = {0,};

    unz_file_info info = {0,};

    filesystem::Path const OUTPUT_DIRECTORY(output_dir);
    do {
        unzGetCurrentFileInfo(uf, &info, filename, MAX_PATH_LENGTH, nullptr, 0, comment, MAX_COMMENT);

        // filename
        // info.compressed_size
        // info.uncompressed_size

        auto node_path = OUTPUT_DIRECTORY / filename;

        if (info.compressed_size == 0 && info.uncompressed_size == 0) {
            // Directory.
            node_path.createDir();

        } else {
            // Regular file.
            if (unzOpenCurrentFile(uf) == UNZ_OK) {
                filesystem::File node_file(node_path);
                int node_offset = 0;
                int read_size   = 0;
                int write_size  = 0;

                do {
                    read_size = unzReadCurrentFile(uf, (void*)in, INPUT_BUFFER);
                    write_size = node_file.write((char const *)in, (std::size_t)read_size, node_offset);
                    if (write_size == read_size) {
                        node_offset += write_size;
                    } else {
                        break; // read & write error.
                    }
                } while (read_size != 0);
                node_file.close();
            }
            unzCloseCurrentFile(uf);
        }
    } while (unzGoToNextFile(uf) == UNZ_OK);
    unzClose(uf);

    return Err::E_SUCCESS;
}

} // namespace archive

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

