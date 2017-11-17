/**
 * @file   ZipBase.cpp
 * @brief  ZipBase class implementation.
 * @author zer0
 * @date   2017-11-15
 */

#include <libtbag/archive/ZipBase.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/archive/Zip.hpp>
#include <libtbag/encrypt/Base64.hpp>
#include <libtbag/filesystem/File.hpp>
#include <libtbag/filesystem/Path.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace archive {

bool encodeZipBase64(char const * input, std::size_t size, std::string & output)
{
    util::Buffer zip_buffer;
    Err const ZIP_CODE = archive::encode(input, size, zip_buffer, archive::TBAG_ZIP_MAX_ENCODE_LEVEL);
    if (isFailure(ZIP_CODE)) {
        tDLogE("encodeZipBase64() Zip encode {} error", getErrName(ZIP_CODE));
        return false;
    }

    std::string base64_string;
    bool const BASE64_RESULT = encrypt::encodeBase64(zip_buffer, base64_string);
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
    bool const BASE64_RESULT = encrypt::decodeBase64(input, debase64_buffer);
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

bool encodeZipBase64File(std::string const & input_file_path, std::string & output)
{
    util::Buffer read_buffer;
    Err const READ_CODE = filesystem::readFile(input_file_path, read_buffer);
    if (isFailure(READ_CODE)) {
        tDLogE("encodeZipBase64File() Read file {} error: {}", getErrName(READ_CODE), input_file_path);
        return false;
    }
    return encodeZipBase64(read_buffer, output);
}

bool decodeZipBase64File(std::string const & input, std::string const & output_file_path)
{
    util::Buffer write_buffer;
    bool const DECODE_RESULT = decodeZipBase64(input, write_buffer);
    if (DECODE_RESULT == false) {
        return false;
    }

    Err const WRITE_CODE = filesystem::writeFile(output_file_path, write_buffer);
    if (isFailure(WRITE_CODE)) {
        tDLogE("decodeZipBase64File() Write file {} error: {}", getErrName(WRITE_CODE), output_file_path);
        return false;
    }
    return true;
}

} // namespace archive

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

