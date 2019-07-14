/**
 * @file   ZipBase64.hpp
 * @brief  ZipBase64 class prototype.
 * @author zer0
 * @date   2017-12-01
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_ARCHIVE_EX_ZIPBASE64_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_ARCHIVE_EX_ZIPBASE64_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/archive/Zip.hpp>
#include <libtbag/util/BufferInfo.hpp>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace archive {
namespace ex      {

TBAG_API Err encodeZipBase64(char const * input, std::size_t size, std::string & output,
                             int level = TBAG_ZIP_DEFAULT_ENCODE_LEVEL,
                             CompressionMethod method = CompressionMethod::CM_ZLIB);
TBAG_API Err encodeZipBase64(util::Buffer const & input, std::string & output,
                             int level = TBAG_ZIP_DEFAULT_ENCODE_LEVEL,
                             CompressionMethod method = CompressionMethod::CM_ZLIB);
TBAG_API Err decodeZipBase64(std::string const & input, util::Buffer & output);

TBAG_API Err encodeZipBase64(std::string const & input, std::string & output,
                             int level = TBAG_ZIP_DEFAULT_ENCODE_LEVEL,
                             CompressionMethod method = CompressionMethod::CM_ZLIB);
TBAG_API Err decodeZipBase64(std::string const & input, std::string & output);

TBAG_API Err encodeZipBase64FromFile(std::string const & input_file_path, std::string & output,
                                     int level = TBAG_ZIP_DEFAULT_ENCODE_LEVEL,
                                     CompressionMethod method = CompressionMethod::CM_ZLIB);
TBAG_API Err decodeZipBase64FromFile(std::string const & input_file_path, util::Buffer & output);

TBAG_API Err encodeZipBase64ToFile(util::Buffer const & input, std::string const & output_file_path,
                                   int level = TBAG_ZIP_DEFAULT_ENCODE_LEVEL,
                                   CompressionMethod method = CompressionMethod::CM_ZLIB);
TBAG_API Err decodeZipBase64ToFile(std::string  const & input, std::string const & output_file_path);

TBAG_API Err encodeZipBase64FileToFile(std::string const & input_file_path, std::string const & output_file_path,
                                       int level = TBAG_ZIP_DEFAULT_ENCODE_LEVEL,
                                       CompressionMethod method = CompressionMethod::CM_ZLIB);
TBAG_API Err decodeZipBase64FileToFile(std::string const & input_file_path, std::string const & output_file_path);

} // namespace ex
} // namespace archive

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ARCHIVE_EX_ZIPBASE64_HPP__

