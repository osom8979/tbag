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

bool encodeZipBase64(util::Buffer const & input, std::string & output)
{
    tDLogD("encodeZipBase64() Input size: {}", input.size());

    util::Buffer zip_buffer;
    int const LEVEL = archive::TBAG_ZIP_MAX_ENCODE_LEVEL;
    Err const ZIP_CODE = archive::encode(input.data(), input.size(), zip_buffer, LEVEL);
    if (isFailure(ZIP_CODE)) {
        tDLogE("encodeZipBase64() Zip encode {} error", getErrName(ZIP_CODE));
        return false;
    }
    tDLogD("encodeZipBase64() Zip size: {}", zip_buffer.size());

    std::string base64_string;
    bool const BASE64_RESULT = encrypt::encodeBase64(zip_buffer, base64_string);
    if (BASE64_RESULT == false) {
        tDLogE("encodeZipBase64() Base64 encode error");
        return false;
    }
    tDLogD("encodeZipBase64() Base64 size: {}", base64_string.size());

    output.swap(base64_string);
    return true;
}

bool decodeZipBase64(std::string const & input, util::Buffer & output)
{
    tDLogD("decodeZipBase64() Input size: {}", input.size());

    util::Buffer zip_buffer;
    bool const BASE64_RESULT = encrypt::decodeBase64(input, zip_buffer);
    if (BASE64_RESULT == false) {
        tDLogE("decodeZipBase64() Base64 decode error");
        return false;
    }
    tDLogD("decodeZipBase64() Zip size: {}", zip_buffer.size());

    util::Buffer result;
    Err const ZIP_CODE = archive::decode(zip_buffer.data(), zip_buffer.size(), result);
    if (isFailure(ZIP_CODE)) {
        tDLogE("decodeZipBase64() Zip decode {} error", getErrName(ZIP_CODE));
        return false;
    }
    tDLogD("decodeZipBase64() Result size: {}", result.size());

    output.swap(result);
    return true;
}

bool encodeZipBase64File(std::string const & input_file_path, std::string & output)
{
    filesystem::Path input_path(input_file_path);
    if (input_path.exists() == false) {
        tDLogE("encodeZipBase64File() Not exists file: {}", input_file_path);
        return false;
    }

    filesystem::File in(input_file_path, filesystem::File::Flags().rdonly());
    std::size_t const FILE_SIZE = in.getState().size;
    tDLogD("encodeZipBase64File() File size: {}", FILE_SIZE);

    util::Buffer buffer(FILE_SIZE);
    int const READ_SIZE = in.read(buffer.data(), FILE_SIZE);
    tDLogD("encodeZipBase64File() File read size: {}", READ_SIZE);

    return encodeZipBase64(buffer, output);
}

bool decodeZipBase64File(std::string const & input, std::string const & output_file_path)
{
    util::Buffer buffer;
    bool const DECODE_RESULT = decodeZipBase64(input, buffer);
    if (DECODE_RESULT == false) {
        return false;
    }

    filesystem::Path output_path(output_file_path);
    if (output_path.exists()) {
        tDLogE("decodeZipBase64File() Exists file: {}", output_file_path);
        return false;
    }

    filesystem::File out(output_file_path);
    int const WRITE_SIZE = out.write(buffer.data(), buffer.size(), 0);
    tDLogD("encodeZipBase64File() File write size: {}", WRITE_SIZE);

    if (WRITE_SIZE <= 0) {
        tDLogE("decodeZipBase64File() Exists file: {}", output_file_path);
        return false;
    }
    return true;
}

} // namespace archive

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

