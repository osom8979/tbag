/**
 * @file   FsProhibited.cpp
 * @brief  FsProhibited class implementation.
 * @author zer0
 * @date   2017-03-05
 */

#include <libtbag/filesystem/details/FsProhibited.hpp>
#include <libtbag/log/Log.hpp>

#include <unicode/unistr.h>
#include <unicode/uchar.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {
namespace details    {

// ---------------
namespace __impl {
// ---------------

template <typename Prohibited>
bool isProhibitedNameWithUtf8(std::string const & utf8_path, Prohibited checker)
{
    if (utf8_path.empty()) {
        return true;
    }

    icu::UnicodeString path = icu::UnicodeString::fromUTF8(icu::StringPiece(utf8_path.c_str()));
    int32_t const PATH_LENGTH = path.length();

    for (int32_t i = 0; i < PATH_LENGTH; ++i) {
        if (checker(path.charAt(i))) {
            return true;
        }
    }
    return false;
}

// ------------------
} // namespace __impl
// ------------------

// -----------
namespace uv {
// -----------

bool isProhibitedNameWithUtf8(std::string const & utf8_path)
{
    return __impl::isProhibitedNameWithUtf8(utf8_path, uv::isProhibitedChar<UChar>);
}

// --------------
} // namespace uv
// --------------

// ----------------
namespace windows {
// ----------------

bool isProhibitedNameWithUtf8(std::string const & utf8_path)
{
    return __impl::isProhibitedNameWithUtf8(utf8_path, windows::isProhibitedChar<UChar>);
}

// -------------------
} // namespace windows
// -------------------

} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

