/**
 * @file   Convert.hpp
 * @brief  Convert class prototype.
 * @author zer0
 * @date   2016-07-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOCALE_CONVERT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOCALE_CONVERT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <exception>
#include <string>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace locale  {

TBAG_API std::vector<std::string> getAvailableConverterNames();

/** Convert from UTF-8 string. */
TBAG_API bool convertFromUtf8(std::string const & utf8, std::string const & to_charset, std::string & result);

/** Convert to UTF-8 string. */
TBAG_API bool convertToUtf8(std::string const & from_string, std::string const & from_charset, std::string & result);

TBAG_API std::string convertUtf8ToNative(std::string const & utf8);
TBAG_API std::string convertNativeToUtf8(std::string const & native);

} // namespace locale

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#ifndef SOURCE_FILE_UTF8_PATH
#define SOURCE_FILE_UTF8_PATH ::libtbag::locale::convertNativeToUtf8(std::string(__FILE__))
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOCALE_CONVERT_HPP__

