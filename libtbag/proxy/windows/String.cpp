/**
 * @file   String.cpp
 * @brief  String class implementation.
 * @author zer0
 * @date   2017-03-05
 */

#include <libtbag/proxy/windows/String.hpp>
#include <libtbag/log/Log.hpp>

#if defined(__PLATFORM_WINDOWS__)
# include <Windows.h>
#else
# include <libtbag/proxy/windows/Dummy.hpp>
using namespace ::libtbag::proxy::windows;
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace proxy   {
namespace windows {

std::wstring mbsToWcsWithAcp(std::string const & path)
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(std::wstring());

    if (path.empty()) {
        __tbag_error("Illegal argument: path is 0 length.");
        return std::wstring();
    }

    int const RESERVE_SIZE = MultiByteToWideChar(CP_ACP, 0, &path[0], (int)path.size(), nullptr, 0);

    std::wstring result;
    if (RESERVE_SIZE == 0) {
        result.resize(path.size());
    } else {
        result.resize(static_cast<std::size_t>(RESERVE_SIZE + 1));
    }

    int const WRITTEN_LENGTH = MultiByteToWideChar(CP_ACP, 0, &path[0], (int)path.size(), &result[0], (int)result.size());
    if (WRITTEN_LENGTH == 0) {
        // ERROR_INSUFFICIENT_BUFFER:    // A supplied buffer size was not large enough, or it was incorrectly set to NULL.
        // ERROR_INVALID_FLAGS:          // The values supplied for flags were not valid.
        // ERROR_INVALID_PARAMETER:      // Any of the parameter values was invalid.
        // ERROR_NO_UNICODE_TRANSLATION: // Invalid Unicode was found in a string.
        __tbag_error("MultiByteToWideChar() ERROR: {}", GetLastError());
        return std::wstring();
    }

    result.resize(static_cast<std::size_t>(WRITTEN_LENGTH));
    return result;
}

std::string wcsToMbsWithAcp(std::wstring const & path)
{
    TBAG_ASSERT_WINDOWS_NOT_IMPLEMENT(std::string());

    if (path.empty()) {
        __tbag_error("Illegal argument: path is 0 length.");
        return std::string();
    }

    int const RESERVE_SIZE = WideCharToMultiByte(CP_ACP, 0, &path[0], (int)path.size(), nullptr, 0, nullptr, nullptr);
    std::string result;

    if (RESERVE_SIZE == 0) {
        result.resize(path.size());
    } else {
        result.resize(static_cast<std::size_t>(RESERVE_SIZE + 1));
    }

    int const WRITTEN_LENGTH = WideCharToMultiByte(CP_ACP, 0, &path[0], (int)path.size(), &result[0], (int)result.size(), nullptr, nullptr);
    if (WRITTEN_LENGTH == 0) {
        // ERROR_INSUFFICIENT_BUFFER:    // A supplied buffer size was not large enough, or it was incorrectly set to NULL.
        // ERROR_INVALID_FLAGS:          // The values supplied for flags were not valid.
        // ERROR_INVALID_PARAMETER:      // Any of the parameter values was invalid.
        // ERROR_NO_UNICODE_TRANSLATION: // Invalid Unicode was found in a string.
        __tbag_error("WideCharToMultiByte() ERROR: {}", GetLastError());
        return std::string();
    }

    result.resize(static_cast<std::size_t>(WRITTEN_LENGTH));
    return result;
}

} // namespace windows
} // namespace proxy

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

