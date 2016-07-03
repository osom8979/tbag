/**
 * @file   Convert.hpp
 * @brief  Convert class prototype.
 * @author zer0
 * @date   2016-07-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TEXT_CONVERT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TEXT_CONVERT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/Exception.hpp>

#include <cassert>
#include <string>

#include <unicode/ucnv.h>
#include <unicode/udat.h>
#include <unicode/udata.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace text {

/**
 * Convert exception class.
 *
 * @author zer0
 * @date   2016-07-03
 */
class ConvertException : public std::exception
{
private:
    std::string message;

public:
    ConvertException(std::string const & msg) : message(msg) {
        // EMPTY.
    }

    virtual ~ConvertException() {
        // EMPTY.
    }

public:
    virtual const char * what() const noexcept override {
        return message.c_str();
    }
};

std::vector<std::string> getAvailableConverterNames()
{
    std::vector<std::string> result;
    int32_t count = ucnv_countAvailable();
    for (int i = 0; i < count; ++i) {
        result.push_back(ucnv_getAvailableName(i));
    }
    return result;
}

std::string convertFromUtf8(std::string const & utf8, std::string const & to_charset) throw (ConvertException)
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

std::string convertToUtf8(std::string const & from_string, std::string const & from_charset) throw (ConvertException)
{
    return "";
}

} // namespace text

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TEXT_CONVERT_HPP__

