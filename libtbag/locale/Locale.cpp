/**
 * @file   Locale.cpp
 * @brief  Locale utilities implementation.
 * @author zer0
 * @date   2016-12-05
 */

#include <libtbag/locale/Locale.hpp>
#include <libtbag/log/Log.hpp>

#include <clocale>

#include <unicode/uloc.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace locale {

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

char * getLocaleName()
{
    // Win32: GetSystemDefaultLocaleName
    return setlocale(LC_ALL, NULL);
}

LocaleNameIndex getLocaleIndex(std::string const & name)
{
    LocaleNameIndex result;
    result.modifier = name.rfind(LOCALE_MODIFIER);
    result.encoding = name.rfind(LOCALE_ENCODING, result.modifier);
    result.country  = name.rfind(LOCALE_COUNTRY, result.encoding);
    return result;
}

std::string getLanguage(std::string const & name)
{
    LocaleNameIndex i = getLocaleIndex(name);
    if (i.country != std::string::npos) {
        return name.substr(0, i.country);
    }
    if (i.encoding != std::string::npos) {
        return name.substr(0, i.encoding);
    }
    if (i.modifier != std::string::npos) {
        return name.substr(0, i.modifier);
    }
    return name;
}

std::string getCountry(std::string const & name)
{
    LocaleNameIndex i = getLocaleIndex(name);
    if (i.country == std::string::npos) {
        return std::string();
    }

    std::size_t const INDEX = i.country + 1;
    if (i.encoding != std::string::npos) {
        return name.substr(INDEX, i.encoding - INDEX);
    }
    if (i.modifier != std::string::npos) {
        return name.substr(INDEX, i.modifier - INDEX);
    }
    return name.substr(INDEX);
}

std::string getEncoding(std::string const & name)
{
    LocaleNameIndex i = getLocaleIndex(name);
    if (i.encoding == std::string::npos) {
        return std::string();
    }

    std::size_t const INDEX = i.encoding + 1;
    if (i.modifier != std::string::npos) {
        return name.substr(INDEX, i.modifier - INDEX);
    }
    return name.substr(INDEX);
}

std::string getModifier(std::string const & name)
{
    LocaleNameIndex i = getLocaleIndex(name);
    if (i.modifier == std::string::npos) {
        return std::string();
    }
    return name.substr(i.modifier + 1);
}

namespace icu {

// Gets ICU's default locale.
std::string getDefaultLocaleName()
{
    return uloc_getDefault();
}

std::vector<std::string> getAvailableLocale()
{
    int32_t const COUNT = uloc_countAvailable();
    std::vector<std::string> result;
    for (int i = 0; i < COUNT; i++) {
        result.push_back(uloc_getAvailable(i));
    }
    return result;
}

} // namespace icu

} // namespace locale

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

