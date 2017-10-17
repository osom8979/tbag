/**
 * @file   Zip.hpp
 * @brief  Zip class prototype.
 * @author zer0
 * @date   2016-11-17
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_ARCHIVE_ZIP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_ARCHIVE_ZIP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/util/BufferInfo.hpp>

#include <cstdint>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace archive {

static int const TBAG_ZIP_DECODE_LEVEL          = 0;
static int const TBAG_ZIP_MIN_ENCODE_LEVEL      = 1;
static int const TBAG_ZIP_DEFAULT_ENCODE_LEVEL  = 6;
static int const TBAG_ZIP_MAX_ENCODE_LEVEL      = 9;

TBAG_API Err coding(char const * input, std::size_t size, util::Buffer & output, int level = TBAG_ZIP_DECODE_LEVEL);
TBAG_API Err encode(char const * input, std::size_t size, util::Buffer & output, int level = TBAG_ZIP_DEFAULT_ENCODE_LEVEL);
TBAG_API Err decode(char const * input, std::size_t size, util::Buffer & output);

TBAG_API Err   zip(std::string const & path, std::string const & output_dir);
TBAG_API Err unzip(std::string const & path, std::string const & output_dir);

} // namespace archive

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ARCHIVE_ZIP_HPP__

