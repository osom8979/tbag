/**
 * @file   Locale.cpp
 * @brief  Locale utilities implementation.
 * @author zer0
 * @date   2016-12-05
 */

#include <libtbag/locale/Locale.hpp>
#include <libtbag/log/Log.hpp>

#include <clocale>
#include <algorithm>

#include <unicode/uloc.h>

#if defined(__PLATFORM_WINDOWS__)
#include <Windows.h>
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace locale {

TBAG_CONSTEXPR static int const LOCALE_ALL      = 0;
TBAG_CONSTEXPR static int const LOCALE_COLLATE  = 1;
TBAG_CONSTEXPR static int const LOCALE_CTYPE    = 2;
TBAG_CONSTEXPR static int const LOCALE_MONETARY = 3;
TBAG_CONSTEXPR static int const LOCALE_NUMERIC  = 4;
TBAG_CONSTEXPR static int const LOCALE_TIME     = 5;
TBAG_CONSTEXPR static int const LOCALE_MESSAGES = 6;

char * setMinimalCLocale()
{
    return setlocale(LC_ALL, "C");
}

char * setSystemDefaultLocale()
{
    return setlocale(LC_ALL, "");
}

char * setLocale(char * name)
{
    return setlocale(LC_ALL, name);
}

char * getGlobalLocaleName()
{
    // Win32: GetSystemDefaultLocaleName
    return setlocale(LC_ALL, NULL);
}

std::string getFirstGlobalLocaleName(std::string const & name)
{
    // Example (in CLion):
    // "ko_KR.UTF-8/ko_KR.UTF-8/C/C/C/C"
    return name.substr(0, name.find(LOCALE_SEPARATOR));
}

LocaleNameIndex getLocaleIndex(std::string const & name)
{
    std::string const FIRST = getFirstGlobalLocaleName(name);
    LocaleNameIndex result;
    result.modifier = FIRST.rfind(LOCALE_MODIFIER);
    result.encoding = FIRST.rfind(LOCALE_ENCODING, result.modifier);
    result.country  = FIRST.rfind(LOCALE_COUNTRY, result.encoding);
    return result;
}

// @formatter:off
#if defined(__PLATFORM_WINDOWS__)
std::string getDefaultEncoding() TBAG_NOEXCEPT { return std::to_string(::GetACP()); }
#else
std::string getDefaultEncoding() TBAG_NOEXCEPT { return std::string("UTF-8"); }
#endif
// @formatter:on

std::string getLanguage(std::string const & name)
{
    std::string const FIRST = getFirstGlobalLocaleName(name);
    LocaleNameIndex i = getLocaleIndex(FIRST);
    if (i.country != std::string::npos) {
        return FIRST.substr(0, i.country);
    }
    if (i.encoding != std::string::npos) {
        return FIRST.substr(0, i.encoding);
    }
    if (i.modifier != std::string::npos) {
        return FIRST.substr(0, i.modifier);
    }
    return FIRST;
}

std::string getCountry(std::string const & name)
{
    std::string const FIRST = getFirstGlobalLocaleName(name);
    LocaleNameIndex i = getLocaleIndex(FIRST);
    if (i.country == std::string::npos) {
        return std::string();
    }

    std::size_t const INDEX = i.country + 1;
    if (i.encoding != std::string::npos) {
        return FIRST.substr(INDEX, i.encoding - INDEX);
    }
    if (i.modifier != std::string::npos) {
        return FIRST.substr(INDEX, i.modifier - INDEX);
    }
    return FIRST.substr(INDEX);
}

std::string getEncoding(std::string const & name)
{
    std::string const FIRST = getFirstGlobalLocaleName(name);
    LocaleNameIndex i = getLocaleIndex(FIRST);
    if (i.encoding == std::string::npos) {
        return std::string();
    }

    std::size_t const INDEX = i.encoding + 1;
    if (i.modifier != std::string::npos) {
        return FIRST.substr(INDEX, i.modifier - INDEX);
    }
    return FIRST.substr(INDEX);
}

std::string getModifier(std::string const & name)
{
    std::string const FIRST = getFirstGlobalLocaleName(name);
    LocaleNameIndex i = getLocaleIndex(FIRST);
    if (i.modifier == std::string::npos) {
        return std::string();
    }
    return FIRST.substr(i.modifier + 1);
}

// @formatter:off
std::string getLanguage(std::locale const & locale) { return getLanguage(locale.name()); }
std::string  getCountry(std::locale const & locale) { return  getCountry(locale.name()); }
std::string getEncoding(std::locale const & locale) { return getEncoding(locale.name()); }
std::string getModifier(std::locale const & locale) { return getModifier(locale.name()); }
// @formatter:on

std::string getGlobalEncodingName()
{
    if (isWindowsPlatform()) {
        return std::string("windows-") + getEncoding(getGlobalLocaleName());
    }
    return getEncoding(getGlobalLocaleName());
}

bool isUtf8EncodingName(std::string const & name)
{
    std::string encoding = getEncoding(name);
    std::transform(encoding.begin(), encoding.end(), encoding.begin(), ::tolower);
    return (encoding == "utf-8"             ||
            encoding == "ibm-1208"          ||
            encoding == "ibm-1209"          ||
            encoding == "ibm-5304"          ||
            encoding == "ibm-5305"          ||
            encoding == "ibm-13496"         ||
            encoding == "ibm-13497"         ||
            encoding == "ibm-17592"         ||
            encoding == "ibm-17593"         ||
            encoding == "windows-65001"     ||
            encoding == "cp1208"            ||
            encoding == "x-utf_8j"          ||
            encoding == "unicode-1-1-utf-8" ||
            encoding == "unicode-2-0-utf-8");
}

bool isUtf8EncodingName(std::locale const & locale)
{
    return isUtf8EncodingName(locale.name());
}

bool isUtf8GloablEncodingName()
{
    return isUtf8EncodingName(getGlobalEncodingName());
}

// ----------
// BEGIN icu.
// ----------

std::string getDefaultIcuLocaleName()
{
    return uloc_getDefault();
}

std::string getDefaultIcuCountry()
{
    // Retrieves the ISO Country Code.
    //return uloc_getCountry();
    return "";
}

std::string getDefaultIcuLanguage()
{
    // Retrieves the ISO Language.
    //return uloc_getLanguage();
    return "";
}

std::string getDefaultIcuDisplayCountry()
{
    // Shows the name of the country suitable for displaying information to the user.
    //return uloc_getDisplayCountry();
    return "";
}

std::string getDefaultIcuDisplayLanguage()
{
    // Shows the name of the language suitable for displaying to the user.
    //return uloc_getDisplayLanguage();
    return "";
}

int32_t getAvailableIcuLocaleCount()
{
    return uloc_countAvailable();
}

std::vector<std::string> getAvailableIcuLocale()
{
    int32_t const COUNT = uloc_countAvailable();
    std::vector<std::string> result;
    for (int i = 0; i < COUNT; i++) {
        result.push_back(uloc_getAvailable(i));
    }
    return result;
}

} // namespace locale

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

