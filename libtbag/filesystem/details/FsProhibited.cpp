/**
 * @file   FsProhibited.cpp
 * @brief  FsProhibited class implementation.
 * @author zer0
 * @date   2017-03-05
 */

#include <libtbag/filesystem/details/FsProhibited.hpp>

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
inline static bool isProhibitedName(std::string const & utf8_path, Prohibited checker)
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

// -------------
namespace unix {
// -------------

bool isProhibitedName(std::string const & utf8_path)
{
    return __impl::isProhibitedName(utf8_path, unix::isProhibitedChar<UChar>);
}

// ----------------
} // namespace unix
// ----------------

// ----------------
namespace windows {
// ----------------

bool isProhibitedName(std::string const & utf8_path)
{
    return __impl::isProhibitedName(utf8_path, windows::isProhibitedChar<UChar>);
}

// -------------------
} // namespace windows
// -------------------

} // namespace details
} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

