/**
 * @file   Zip.cpp
 * @brief  Zip class implementation.
 * @author zer0
 * @date   2016-11-17
 */

#include <libtbag/archive/Zip.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/filesystem/File.hpp>

#include <ctime>
#include <cstring>
#include <cassert>

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

Err zip(std::vector<std::string> const & files,
        std::string const & output_path,
        std::vector<std::string> const & names,
        std::vector<std::string> const & comments,
        std::string const & global_comment)
{
    filesystem::Path const OUTPUT_FILE_PATH(output_path);
    if (OUTPUT_FILE_PATH.exists()) {
        return Err::E_EEXIST;
    }

    zipFile zf = zipOpen(output_path.c_str(), APPEND_STATUS_CREATE);
    if (zf == nullptr) {
        return Err::E_EOPEN;
    }

    zip_fileinfo info = {0,};
    time_t time = 0;
    ::time(&time);
    struct tm * tdata = localtime(&time);
    info.tmz_date.tm_hour = (uInt)tdata->tm_hour;
    info.tmz_date.tm_mday = (uInt)tdata->tm_mday;
    info.tmz_date.tm_min  = (uInt)tdata->tm_min;
    info.tmz_date.tm_mon  = (uInt)tdata->tm_mon;
    info.tmz_date.tm_sec  = (uInt)tdata->tm_sec;
    info.tmz_date.tm_year = (uInt)tdata->tm_year;

    std::size_t const INPUT_BUFFER = 2048;
    Bytef in[INPUT_BUFFER] = {0,};

    auto    file_itr =    files.begin();
    auto    name_itr =    names.begin();
    auto comment_itr = comments.begin();

    auto const    FILE_END =    files.end();
    auto const    NAME_END =    names.end();
    auto const COMMENT_END = comments.end();

    std::string    file_cursor;
    std::string    name_cursor;
    std::string comment_cursor;

    while (file_itr != FILE_END) {
        file_cursor = *file_itr;

        if (name_itr == NAME_END) {
            name_cursor = file_cursor;
        } else {
            name_cursor = *name_itr;
        }

        if (comment_itr == COMMENT_END) {
            comment_cursor.clear();
        } else {
            comment_cursor = *comment_itr;
        }

        // Create file list.
        int const OPEN_RESULT = zipOpenNewFileInZip(zf, name_cursor.c_str(), &info,
                                                    nullptr, 0, nullptr, 0, comment_cursor.c_str(),
                                                    Z_DEFLATED, Z_DEFAULT_COMPRESSION);

        if (OPEN_RESULT == 0) {
            do {
                filesystem::File file(file_cursor);
                if (file.isOpen() == false) {
                    break;
                }

                int total_size = 0;
                int read_size = 0;

                do {
                    read_size = file.read((char*)in, INPUT_BUFFER, total_size);
                    total_size += read_size;
                    zipWriteInFileInZip(zf, (void const *)in, (unsigned)read_size);
                } while (read_size == 0);

                file.close();
            } while(false);
            zipCloseFileInZip(zf);
        }

        ++file_itr;
        if (name_itr != NAME_END) {
            ++name_itr;
        }
        if (comment_itr != COMMENT_END) {
            ++comment_itr;
        }
    }

    zipClose(zf, global_comment.c_str());
    return Err::E_SUCCESS;
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
    if (OUTPUT_DIRECTORY.exists() == false) {
        return Err::E_ENOENT;
    }

    do {
        unzGetCurrentFileInfo(uf, &info, filename, MAX_PATH_LENGTH, nullptr, 0, comment, MAX_COMMENT);

        filesystem::Path node_path;
        if (filesystem::Path(filename).isAbsolute()) {
            node_path = filename;
        } else {
            node_path = OUTPUT_DIRECTORY / filename;
        }

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

