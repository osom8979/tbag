/**
 * @file   GzipBase64.hpp
 * @brief  GzipBase64 class prototype.
 * @author zer0
 * @date   2019-07-11
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_ARCHIVE_EX_GZIPBASE64_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_ARCHIVE_EX_GZIPBASE64_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/util/BufferInfo.hpp>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace archive {
namespace ex      {

TBAG_API bool encodeGzipBase64(char const * input, std::size_t size, std::string & output);
TBAG_API bool encodeGzipBase64(util::Buffer const & input, std::string & output);
TBAG_API bool decodeGzipBase64(std::string  const & input, util::Buffer & output);

TBAG_API bool encodeGzipBase64(std::string const & input, std::string & output);
TBAG_API bool decodeGzipBase64(std::string const & input, std::string & output);

TBAG_API bool encodeGzipBase64FromFile(std::string const & input_file_path, std::string & output);
TBAG_API bool decodeGzipBase64FromFile(std::string const & input_file_path, util::Buffer & output);

TBAG_API bool encodeGzipBase64ToFile(util::Buffer const & input, std::string const & output_file_path);
TBAG_API bool decodeGzipBase64ToFile(std::string  const & input, std::string const & output_file_path);

TBAG_API bool encodeGzipBase64FileToFile(std::string const & input_file_path, std::string const & output_file_path);
TBAG_API bool decodeGzipBase64FileToFile(std::string const & input_file_path, std::string const & output_file_path);

} // namespace ex
} // namespace archive

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ARCHIVE_EX_GZIPBASE64_HPP__

