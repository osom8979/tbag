/**
 * @file   ZipBase64.cpp
 * @brief  ZipBase64 class implementation.
 * @author zer0
 * @date   2017-12-01
 */

#include <libtbag/archive/ex/ZipBase64.hpp>
#include <libtbag/filesystem/File.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/crypto/Base64.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace archive {
namespace ex      {

Err encodeZipBase64(char const * input, std::size_t size, std::string & output, int level, CompressionMethod method)
{
    libtbag::util::Buffer zip_buffer;
    auto const code = encode(input, size, zip_buffer, level, method);
    if (isFailure(code)) {
        return code;
    }
    std::string base64_string;
    if (libtbag::crypto::encodeBase64(zip_buffer, base64_string)) {
        output.swap(base64_string);
        return E_SUCCESS;
    }
    return E_ENCODE;
}

Err encodeZipBase64(util::Buffer const & input, std::string & output, int level, CompressionMethod method)
{
    return encodeZipBase64(input.data(), input.size(), output, level, method);
}

Err decodeZipBase64(std::string const & input, util::Buffer & output)
{
    libtbag::util::Buffer debase64_buffer;
    if (!libtbag::crypto::decodeBase64(input, debase64_buffer)) {
        return E_DECODE;
    }
    libtbag::util::Buffer result;
    auto const code = decode(debase64_buffer.data(), debase64_buffer.size(), result);
    if (isSuccess(code)) {
        output.swap(result);
    }
    return code;
}

Err encodeZipBase64(std::string const & input, std::string & output, int level, CompressionMethod method)
{
    return encodeZipBase64(util::Buffer(input.begin(), input.end()), output, level, method);
}

Err decodeZipBase64(std::string const & input, std::string & output)
{
    libtbag::util::Buffer buffer;
    auto const code = decodeZipBase64(input, buffer);
    if (isSuccess(code)) {
        output.assign(buffer.begin(), buffer.end());
    }
    return code;
}

Err encodeZipBase64FromFile(std::string const & input_file_path, std::string & output,
                            int level, CompressionMethod method)
{
    libtbag::util::Buffer read_buffer;
    auto const code = libtbag::filesystem::readFile(input_file_path, read_buffer);
    if (isFailure(code)) {
        return code;
    }
    return encodeZipBase64(read_buffer, output, level, method);
}

Err decodeZipBase64FromFile(std::string const & input_file_path, util::Buffer & output)
{
    std::string base64_string;
    auto const code = libtbag::filesystem::readFile(input_file_path, base64_string);
    if (isFailure(code)) {
        return code;
    }
    return decodeZipBase64(base64_string, output);
}

Err encodeZipBase64ToFile(util::Buffer const & input, std::string const & output_file_path,
                          int level, CompressionMethod method)
{
    std::string write_buffer;
    auto const code = encodeZipBase64(input, write_buffer, level, method);
    if (isFailure(code)) {
        return code;
    }
    return libtbag::filesystem::writeFile(output_file_path, write_buffer);
}

Err decodeZipBase64ToFile(std::string const & input, std::string const & output_file_path)
{
    libtbag::util::Buffer write_buffer;
    auto const code = decodeZipBase64(input, write_buffer);
    if (isFailure(code)) {
        return code;
    }
    return libtbag::filesystem::writeFile(output_file_path, write_buffer);
}

Err encodeZipBase64FileToFile(std::string const & input_file_path, std::string const & output_file_path,
                              int level, CompressionMethod method)
{
    auto const code1 = libtbag::filesystem::checkPrepareFiles(input_file_path, output_file_path);
    if (isFailure(code1)) {
        return code1;
    }
    std::string encode_string;
    auto const code2 = encodeZipBase64FromFile(input_file_path, encode_string, level, method);
    if (isFailure(code2)) {
        return code2;
    }
    return libtbag::filesystem::writeFile(output_file_path, encode_string);
}

Err decodeZipBase64FileToFile(std::string const & input_file_path, std::string const & output_file_path)
{
    auto const code1 = libtbag::filesystem::checkPrepareFiles(input_file_path, output_file_path);
    if (isFailure(code1)) {
        return code1;
    }
    libtbag::util::Buffer decode_buffer;
    auto const code2 = decodeZipBase64FromFile(input_file_path, decode_buffer);
    if (isFailure(code2)) {
        return code2;
    }
    return filesystem::writeFile(output_file_path, decode_buffer);
}

} // namespace ex
} // namespace archive

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

