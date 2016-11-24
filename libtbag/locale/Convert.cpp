/**
 * @file   Convert.cpp
 * @brief  Convert class implementation.
 * @author zer0
 * @date   2016-07-06
 */

#include <libtbag/locale/Convert.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>

#include <unicode/ucnv.h>
#include <unicode/udat.h>
#include <unicode/udata.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace locale  {

/**
 * UConverter initializer helper class.
 *
 * @author zer0
 * @date   2016-11-24
 */
struct Converter : public Noncopyable
{
    UConverter * converter;
    UErrorCode   status;

    Converter(std::string const & charset) : converter(nullptr), status(U_ZERO_ERROR)
    {
        converter = ucnv_open(charset.c_str(), &status);
    }

    ~Converter()
    {
        if (converter != nullptr) {
            ucnv_close(converter);
        }
    }

    inline UConverter * get()
    { return converter; }

    inline UErrorCode getStatus() const
    { return status; }
    inline int getIntegerStatus() const
    { return static_cast<int>(status); }
};

// --------------------
// Main implementation.
// --------------------

std::vector<std::string> getAvailableConverterNames()
{
    std::vector<std::string> result;
    int32_t count = ucnv_countAvailable();
    for (int i = 0; i < count; ++i) {
        result.push_back(ucnv_getAvailableName(i));
    }
    return result;
}

bool convertFromUtf8(std::string const & utf8, std::string const & to_charset, std::string & result)
{
    Converter converter(to_charset);

    if (U_FAILURE(converter.getStatus())) {
        __tbag_error_f("convertFromUtf8() error[{}] {}", converter.getIntegerStatus(), u_errorName(converter.getStatus()));
        return false;
    }

    assert(converter.get() != nullptr);

    icu::UnicodeString unicode = icu::UnicodeString::fromUTF8(icu::StringPiece(utf8.c_str()));
    UErrorCode status = U_ZERO_ERROR;

    int32_t result_size = ucnv_fromUChars(converter.get(), nullptr, 0, unicode.getBuffer(), unicode.length(), &status);

    if (status != U_BUFFER_OVERFLOW_ERROR) {
        __tbag_error_f("convertFromUtf8() error[{}] {}", static_cast<int>(status), u_errorName(status));
        return false;
    }

    assert(result_size > 0);
    status = U_ZERO_ERROR;

    result.resize(result_size);
    result_size = ucnv_fromUChars(converter.get(), &result[0], result.size(), unicode.getBuffer(), unicode.length(), &status);

    if (U_FAILURE(status)) {
        __tbag_error_f("convertFromUtf8() error[{}] {}", static_cast<int>(status), u_errorName(status));
        return false;
    }

    return true;
}

bool convertToUtf8(std::string const & from_string, std::string const & from_charset, std::string & result)
{
    Converter converter(from_charset);

    if (U_FAILURE(converter.getStatus())) {
        __tbag_error_f("convertToUtf8() error[{}] {}", converter.getIntegerStatus(), u_errorName(converter.getStatus()));
        return false;
    }

    assert(converter.get() != nullptr);

    UErrorCode status = U_ZERO_ERROR;
    int32_t result_size = ucnv_toUChars(converter.get(), nullptr, 0, from_string.c_str(), from_string.size(), &status);

    if (status != U_BUFFER_OVERFLOW_ERROR) {
        __tbag_error_f("convertToUtf8() error[{}] {}", static_cast<int>(status), u_errorName(status));
        return false;
    }

    assert(result_size > 0);
    status = U_ZERO_ERROR;

    std::vector<UChar> unicode_buffer;
    unicode_buffer.resize(result_size + 1);
    unicode_buffer.at(result_size) = '\0';
    result_size = ucnv_toUChars(converter.get(), &unicode_buffer[0], unicode_buffer.size(), from_string.c_str(), from_string.size(), &status);

    if (U_FAILURE(status)) {
        __tbag_error_f("convertToUtf8() error[{}] {}", static_cast<int>(status), u_errorName(status));
        return false;
    }

    icu::UnicodeString unicode(&unicode_buffer[0]);
    result.clear();
    unicode.toUTF8String(result);

    return true;
}

} // namespace locale

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

