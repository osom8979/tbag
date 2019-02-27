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
#include <vector>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace archive {

TBAG_CONSTEXPR int const TBAG_ZIP_MIN_ENCODE_LEVEL      = 1;
TBAG_CONSTEXPR int const TBAG_ZIP_DEFAULT_ENCODE_LEVEL  = 6;
TBAG_CONSTEXPR int const TBAG_ZIP_MAX_ENCODE_LEVEL      = 9;

TBAG_API Err encode(char const * input, std::size_t size, util::Buffer & output,
                    int level = TBAG_ZIP_DEFAULT_ENCODE_LEVEL);
TBAG_API Err decode(char const * input, std::size_t size, util::Buffer & output);

TBAG_API Err zip(std::vector<std::string> const & files,
                 std::string const & output_path,
                 std::vector<std::string> const & names = std::vector<std::string>(),
                 std::vector<std::string> const & comments = std::vector<std::string>(),
                 std::string const & global_comment = std::string());
TBAG_API Err unzip(std::string const & path, std::string const & output_dir);

} // namespace archive

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ARCHIVE_ZIP_HPP__

