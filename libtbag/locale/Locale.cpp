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

