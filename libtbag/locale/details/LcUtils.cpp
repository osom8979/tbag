/**
 * @file   LcUtils.cpp
 * @brief  LcUtils class implementation.
 * @author zer0
 * @date   2017-03-16
 */

#include <libtbag/locale/details/LcUtils.hpp>
#include <libtbag/log/Log.hpp>

#include <clocale>
#include <algorithm>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace locale  {
namespace details {

char * setMinimalCLocale()
{
    return setlocale(LC_ALL, "C");
}

char * setSystemDefaultLocale()
{
    return setlocale(LC_ALL, "");
}

char * setAllLocale(char * name)
{
    return setlocale(LC_ALL, name);
}

char * getGlobalLocaleName()
{
    // Win32: GetSystemDefaultLocaleName
    return setlocale(LC_ALL, NULL);
}

bool isUtf8EncodingName(std::string const & name)
{
    std::string encoding = name;
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

} // namespace details
} // namespace locale

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

