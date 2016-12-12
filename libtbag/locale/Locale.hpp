/**
 * @file   Locale.hpp
 * @brief  Locale utilities.
 * @author zer0
 * @date   2016-12-05
 *
 * @remarks
 *  Locale format:
 *  @code
 *   [language[_country][.codeset][@modifier]]
 *  @endcode
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOCALE_LOCALE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOCALE_LOCALE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <string>
#include <locale>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace locale {

TBAG_API char * setMinimalCLocale();
TBAG_API char * setSystemDefaultLocale();
TBAG_API char * setLocale(char * name);
TBAG_API char * getGlobalLocaleName();

char const LOCALE_COUNTRY  = '_';
char const LOCALE_ENCODING = '.';
char const LOCALE_MODIFIER = '@';

struct LocaleNameIndex
{
    std::size_t country;  ///< "_"
    std::size_t encoding; ///< "."
    std::size_t modifier; ///< "@"
};

TBAG_API LocaleNameIndex getLocaleIndex(std::string const & name);

TBAG_API std::string getLanguage(std::string const & name);
TBAG_API std::string  getCountry(std::string const & name);
TBAG_API std::string getEncoding(std::string const & name);
TBAG_API std::string getModifier(std::string const & name);

TBAG_API std::string getLanguage(std::locale const & locale);
TBAG_API std::string  getCountry(std::locale const & locale);
TBAG_API std::string getEncoding(std::locale const & locale);
TBAG_API std::string getModifier(std::locale const & locale);

/**
 * Check the encoding name of UTF-8.
 *
 * @see http://demo.icu-project.org/icu-bin/convexp
 */
TBAG_API bool isUtf8EncodingName(std::string const & name);
TBAG_API bool isUtf8EncodingName(std::locale const & locale);

TBAG_API std::string getGlobalEncodingName();
TBAG_API bool isUtf8GloablEncoding();

namespace icu {
TBAG_API std::string getDefaultLocaleName();
TBAG_API std::vector<std::string> getAvailableLocale();
} // namespace icu

} // namespace locale

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOCALE_LOCALE_HPP__

