/**
 * @file   Convert.cpp
 * @brief  Convert class implementation.
 * @author zer0
 * @date   2016-07-06
 */

#include <libtbag/text/Convert.hpp>

#include <cassert>

#include <unicode/ucnv.h>
#include <unicode/udat.h>
#include <unicode/udata.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace text    {
namespace convert {

std::vector<std::string> getAvailableConverterNames()
{
    std::vector<std::string> result;
    int32_t count = ucnv_countAvailable();
    for (int i = 0; i < count; ++i) {
        result.push_back(ucnv_getAvailableName(i));
    }
    return result;
}

std::string convertFromUtf8(std::string const & utf8
                          , std::string const & to_charset) throw (ConvertException)
{
    UErrorCode status = U_ZERO_ERROR;
    UConverter * converter = ucnv_open(to_charset.c_str(), &status);

    if (U_FAILURE(status)) {
        throw ConvertException(u_errorName(status));
    }

    assert(converter != nullptr);
    status = U_ZERO_ERROR;

    icu::UnicodeString unicode = icu::UnicodeString::fromUTF8(icu::StringPiece(utf8.c_str()));
    int32_t result_size = ucnv_fromUChars(converter, nullptr, 0, unicode.getBuffer(), unicode.length(), &status);

    if (status != U_BUFFER_OVERFLOW_ERROR) {
        throw ConvertException(u_errorName(status));
    }

    assert(result_size > 0);
    status = U_ZERO_ERROR;

    std::string result;
    result.resize(result_size);
    result_size = ucnv_fromUChars(converter, &result[0], result.size(), unicode.getBuffer(), unicode.length(), &status);

    if (U_FAILURE(status)) {
        throw ConvertException(u_errorName(status));
    }

    ucnv_close(converter);
    return result;
}

std::string convertToUtf8(std::string const & from_string
                        , std::string const & from_charset) throw (ConvertException)
{
    UErrorCode status = U_ZERO_ERROR;
    UConverter * converter = ucnv_open(from_charset.c_str(), &status);

    if (U_FAILURE(status)) {
        throw ConvertException(u_errorName(status));
    }

    assert(converter != nullptr);
    status = U_ZERO_ERROR;

    int32_t result_size = ucnv_toUChars(converter, nullptr, 0, from_string.c_str(), from_string.size(), &status);

    if (status != U_BUFFER_OVERFLOW_ERROR) {
        throw ConvertException(u_errorName(status));
    }

    assert(result_size > 0);
    status = U_ZERO_ERROR;

    std::vector<UChar> unicode_buffer;
    unicode_buffer.resize(result_size + 1);
    unicode_buffer.at(result_size) = '\0';
    result_size = ucnv_toUChars(converter, &unicode_buffer[0], unicode_buffer.size(), from_string.c_str(), from_string.size(), &status);

    if (U_FAILURE(status)) {
        throw ConvertException(u_errorName(status));
    }

    ucnv_close(converter);

    icu::UnicodeString unicode(&unicode_buffer[0]);
    std::string result;
    unicode.toUTF8String(result);

    return result;
}

} // namespace convert
} // namespace text

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

