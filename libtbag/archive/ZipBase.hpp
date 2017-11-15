/**
 * @file   ZipBase.hpp
 * @brief  ZipBase class prototype.
 * @author zer0
 * @date   2017-11-15
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_ARCHIVE_ZIPBASE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_ARCHIVE_ZIPBASE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/util/BufferInfo.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace archive {

TBAG_API bool encodeZipBase64(std::string const & input, std::string & output);
TBAG_API bool decodeZipBase64(std::string const & input, std::string & output);

TBAG_API bool encodeZipBase64(util::Buffer const & input, std::string & output);
TBAG_API bool decodeZipBase64(std::string const & input, util::Buffer & output);

TBAG_API bool encodeZipBase64File(std::string const & input_file_path, std::string & output);
TBAG_API bool decodeZipBase64File(std::string const & input, std::string const & output_file_path);

} // namespace archive

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ARCHIVE_ZIPBASE_HPP__

