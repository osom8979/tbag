/**
 * @file   ZipBase64.cpp
 * @brief  ZipBase64 class implementation.
 * @author zer0
 * @date   2017-12-01
 */

#include <libtbag/archive/ex/ZipBase64.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/archive/Zip.hpp>
#include <libtbag/crypto/Base64.hpp>
#include <libtbag/filesystem/File.hpp>
#include <libtbag/filesystem/Path.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace archive {
namespace ex      {

bool encodeZipBase64(char const * input, std::size_t size, std::string & output)
{
    util::Buffer zip_buffer;
    Err const ZIP_CODE = archive::encode(input, size, zip_buffer, archive::TBAG_ZIP_MAX_ENCODE_LEVEL);
    if (isFailure(ZIP_CODE)) {
        tDLogE("encodeZipBase64() Zip encode {} error", getErrName(ZIP_CODE));
        return false;
    }

    std::string base64_string;
    bool const BASE64_RESULT = crypto::encodeBase64(zip_buffer, base64_string);
    if (BASE64_RESULT == false) {
        tDLogE("encodeZipBase64() Base64 encode error");
        return false;
    }

    output.swap(base64_string);
    return true;
}

bool encodeZipBase64(util::Buffer const & input, std::string & output)
{
    return encodeZipBase64(input.data(), input.size(), output);
}

bool decodeZipBase64(std::string const & input, util::Buffer & output)
{
    util::Buffer debase64_buffer;
    bool const BASE64_RESULT = crypto::decodeBase64(input, debase64_buffer);
    if (BASE64_RESULT == false) {
        tDLogE("decodeZipBase64() Base64 decode error");
        return false;
    }

    util::Buffer result;
    Err const ZIP_CODE = archive::decode(debase64_buffer.data(), debase64_buffer.size(), result);
    if (isFailure(ZIP_CODE)) {
        tDLogE("decodeZipBase64() Zip decode {} error", getErrName(ZIP_CODE));
        return false;
    }

    output.swap(result);
    return true;
}

bool encodeZipBase64(std::string const & input, std::string & output)
{
    return encodeZipBase64(util::Buffer(input.begin(), input.end()), output);
}

bool decodeZipBase64(std::string const & input, std::string & output)
{
    util::Buffer buffer;
    bool const RESULT = decodeZipBase64(input, buffer);
    output.assign(buffer.begin(), buffer.end());
    return RESULT;
}

bool encodeZipBase64FromFile(std::string const & input_file_path, std::string & output)
{
    util::Buffer read_buffer;
    Err const READ_CODE = filesystem::readFile(input_file_path, read_buffer);
    if (isFailure(READ_CODE)) {
        tDLogE("encodeZipBase64FromFile() Read file {} error: {}", getErrName(READ_CODE), input_file_path);
        return false;
    }
    return encodeZipBase64(read_buffer, output);
}

bool decodeZipBase64FromFile(std::string const & input_file_path, util::Buffer & output)
{
    std::string base64_string;
    Err const READ_CODE = filesystem::readFile(input_file_path, base64_string);
    if (isFailure(READ_CODE)) {
        tDLogE("decodeZipBase64FromFile() Read file {} error: {}", getErrName(READ_CODE), input_file_path);
        return false;
    }
    return decodeZipBase64(base64_string, output);
}

bool encodeZipBase64ToFile(util::Buffer const & input, std::string const & output_file_path)
{
    std::string write_buffer;
    bool const ENCODE_RESULT = encodeZipBase64(input, write_buffer);
    if (ENCODE_RESULT == false) {
        return false;
    }

    Err const WRITE_CODE = filesystem::writeFile(output_file_path, write_buffer);
    if (isFailure(WRITE_CODE)) {
        tDLogE("encodeZipBase64ToFile() Write file {} error: {}", getErrName(WRITE_CODE), output_file_path);
        return false;
    }
    return true;
}

bool decodeZipBase64ToFile(std::string const & input, std::string const & output_file_path)
{
    util::Buffer write_buffer;
    bool const DECODE_RESULT = decodeZipBase64(input, write_buffer);
    if (DECODE_RESULT == false) {
        return false;
    }

    Err const WRITE_CODE = filesystem::writeFile(output_file_path, write_buffer);
    if (isFailure(WRITE_CODE)) {
        tDLogE("decodeZipBase64ToFile() Write file {} error: {}", getErrName(WRITE_CODE), output_file_path);
        return false;
    }
    return true;
}

bool encodeZipBase64FileToFile(std::string const & input_file_path, std::string const & output_file_path)
{
    if (filesystem::checkPrepareFiles(input_file_path, output_file_path) != E_SUCCESS) {
        return false;
    }

    std::string encode_string;
    if (encodeZipBase64FromFile(input_file_path, encode_string) == false) {
        return false;
    }

    Err const WRITE_CODE = filesystem::writeFile(output_file_path, encode_string);
    if (isFailure(WRITE_CODE)) {
        tDLogE("encodeZipBase64FileToFile() Write file {} error: {}", getErrName(WRITE_CODE), output_file_path);
        return false;
    }
    return true;
}

bool decodeZipBase64FileToFile(std::string const & input_file_path, std::string const & output_file_path)
{
    if (filesystem::checkPrepareFiles(input_file_path, output_file_path) != E_SUCCESS) {
        return false;
    }

    util::Buffer decode_buffer;
    if (decodeZipBase64FromFile(input_file_path, decode_buffer) == false) {
        return false;
    }

    Err const WRITE_CODE = filesystem::writeFile(output_file_path, decode_buffer);
    if (isFailure(WRITE_CODE)) {
        tDLogE("decodeZipBase64FileToFile() Write file {} error: {}", getErrName(WRITE_CODE), output_file_path);
        return false;
    }
    return true;
}

} // namespace ex
} // namespace archive

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

