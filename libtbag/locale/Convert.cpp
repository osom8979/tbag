/**
 * @file   Convert.cpp
 * @brief  Convert class implementation.
 * @author zer0
 * @date   2016-07-06
 */

#include <libtbag/locale/Convert.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/locale/Locale.hpp>
#include <libtbag/Noncopyable.hpp>

#include <cassert>

#include <unicode/ucnv.h>
#include <unicode/udat.h>
#include <unicode/udata.h>

#if defined(TBAG_PLATFORM_WINDOWS)
# include <Windows.h>
#else
# include <libtbag/proxy/windows/Dummy.hpp>
using namespace ::libtbag::proxy::windows;
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace locale  {

// ----------------
namespace windows {
// ----------------

#if defined(TBAG_PLATFORM_WINDOWS)
static_assert(CP_ACP  == CODE_PAGE_ANSI, "Mismatched CP_ACP constants.");
static_assert(CP_UTF8 == CODE_PAGE_UTF8, "Mismatched CP_UTF8 constants.");
#endif

/**
 * Multi-Byte-String -> Wide-Char-String.
 *
 * @remarks
 *  ACP: The system default Windows ANSI code page.
 */
std::wstring mbsToWcs(std::string const & path, unsigned int code_page)
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(std::wstring());

    if (path.empty()) {
        tDLogE("Illegal argument: path is 0 length.");
        return std::wstring();
    }

    int const RESERVE_SIZE = MultiByteToWideChar(code_page, 0, &path[0], (int)path.size(), nullptr, 0);

    std::wstring result;
    if (RESERVE_SIZE == 0) {
        result.resize(path.size());
    } else {
        result.resize(static_cast<std::size_t>(RESERVE_SIZE + 1));
    }

    int const WRITTEN_LENGTH = MultiByteToWideChar(code_page, 0, &path[0], (int)path.size(), &result[0], (int)result.size());
    if (WRITTEN_LENGTH == 0) {
        // ERROR_INSUFFICIENT_BUFFER:    // A supplied buffer size was not large enough, or it was incorrectly set to NULL.
        // ERROR_INVALID_FLAGS:          // The values supplied for flags were not valid.
        // ERROR_INVALID_PARAMETER:      // Any of the parameter values was invalid.
        // ERROR_NO_UNICODE_TRANSLATION: // Invalid Unicode was found in a string.
        tDLogE("MultiByteToWideChar() ERROR: {}", GetLastError());
        return std::wstring();
    }

    result.resize(static_cast<std::size_t>(WRITTEN_LENGTH));
    return result;
}

/**
 * Wide-Char-String -> Multi-Byte-String.
 *
 * @remarks
 *  ACP: The system default Windows ANSI code page.
 */
std::string wcsToMbs(std::wstring const & path, unsigned int code_page)
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(std::string());

    if (path.empty()) {
        tDLogE("Illegal argument: path is 0 length.");
        return std::string();
    }

    int const RESERVE_SIZE = WideCharToMultiByte(code_page, 0, &path[0], (int)path.size(), nullptr, 0, nullptr, nullptr);
    std::string result;

    if (RESERVE_SIZE == 0) {
        result.resize(path.size());
    } else {
        result.resize(static_cast<std::size_t>(RESERVE_SIZE + 1));
    }

    int const WRITTEN_LENGTH = WideCharToMultiByte(code_page, 0, &path[0], (int)path.size(), &result[0], (int)result.size(), nullptr, nullptr);
    if (WRITTEN_LENGTH == 0) {
        // ERROR_INSUFFICIENT_BUFFER:    // A supplied buffer size was not large enough, or it was incorrectly set to NULL.
        // ERROR_INVALID_FLAGS:          // The values supplied for flags were not valid.
        // ERROR_INVALID_PARAMETER:      // Any of the parameter values was invalid.
        // ERROR_NO_UNICODE_TRANSLATION: // Invalid Unicode was found in a string.
        tDLogE("WideCharToMultiByte() ERROR: {}", GetLastError());
        return std::string();
    }

    result.resize(static_cast<std::size_t>(WRITTEN_LENGTH));
    return result;
}

// -------------------
} // namespace windows
// -------------------

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

std::string getWindowsCharsetName(int code_page)
{
    return std::string("windows-") + std::to_string(code_page);
}

bool convertFromUtf8(std::string const & utf8, std::string const & to_charset, std::string & result)
{
    Converter converter(to_charset);

    if (U_FAILURE(converter.getStatus())) {
        //tDLogE("convertFromUtf8() error[{}] {}", converter.getIntegerStatus(), u_errorName(converter.getStatus()));
        return false;
    }

    assert(converter.get() != nullptr);

    icu::UnicodeString unicode = icu::UnicodeString::fromUTF8(icu::StringPiece(utf8.c_str()));
    UErrorCode status = U_ZERO_ERROR;

    int32_t result_size = ucnv_fromUChars(converter.get(), nullptr, 0, unicode.getBuffer(), unicode.length(), &status);

    if (status != U_BUFFER_OVERFLOW_ERROR) {
        //tDLogE("convertFromUtf8() error[{}] {}", static_cast<int>(status), u_errorName(status));
        return false;
    }

    assert(result_size > 0);
    status = U_ZERO_ERROR;

    result.resize(result_size);
    result_size = ucnv_fromUChars(converter.get(), &result[0], result.size(), unicode.getBuffer(), unicode.length(), &status);

    if (U_FAILURE(status)) {
        //tDLogE("convertFromUtf8() error[{}] {}", static_cast<int>(status), u_errorName(status));
        return false;
    }

    return true;
}

bool convertToUtf8(std::string const & from_string, std::string const & from_charset, std::string & result)
{
    Converter converter(from_charset);

    if (U_FAILURE(converter.getStatus())) {
        //tDLogE("convertToUtf8() error[{}] {}", converter.getIntegerStatus(), u_errorName(converter.getStatus()));
        return false;
    }

    assert(converter.get() != nullptr);

    UErrorCode status = U_ZERO_ERROR;
    int32_t result_size = ucnv_toUChars(converter.get(), nullptr, 0, from_string.c_str(), from_string.size(), &status);

    if (status != U_BUFFER_OVERFLOW_ERROR) {
        //tDLogE("convertToUtf8() error[{}] {}", static_cast<int>(status), u_errorName(status));
        return false;
    }

    assert(result_size > 0);
    status = U_ZERO_ERROR;

    std::vector<UChar> unicode_buffer;
    unicode_buffer.resize(result_size + 1);
    unicode_buffer.at(result_size) = '\0';
    result_size = ucnv_toUChars_57(converter.get(), &unicode_buffer[0], unicode_buffer.size(), from_string.c_str(), from_string.size(), &status);

    if (U_FAILURE(status)) {
        //tDLogE("convertToUtf8() error[{}] {}", static_cast<int>(status), u_errorName(status));
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

