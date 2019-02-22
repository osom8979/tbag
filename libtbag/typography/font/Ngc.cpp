/**
 * @file   Ngc.cpp
 * @brief  Ngc class implementation.
 * @author zer0
 * @date   2019-02-22
 */

#include <libtbag/typography/font/Ngc.hpp>
#include <libtbag/typography/font/ngc/ngcn.h.inl>
#include <libtbag/typography/font/ngc/ngcb.h.inl>
#include <libtbag/string/StringUtils.hpp>

#include <cassert>
#include <cstring>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace typography {
namespace font       {

libtbag::util::Buffer getNgcNormal() TBAG_NOEXCEPT
{
    auto const * const HEX_STRING = __get_binary_to_cpp11_string__ngcn__();
    auto const HEX_STRING_LENGTH = strlen(HEX_STRING);
    libtbag::util::Buffer result;
    auto const CODE = libtbag::string::convertHexStringToBuffer(HEX_STRING, HEX_STRING_LENGTH, result);
    assert(isSuccess(CODE));
    return result;
}

libtbag::util::Buffer getNgcBold() TBAG_NOEXCEPT
{
    auto const * const HEX_STRING = __get_binary_to_cpp11_string__ngcb__();
    auto const HEX_STRING_LENGTH = strlen(HEX_STRING);
    libtbag::util::Buffer result;
    auto const CODE = libtbag::string::convertHexStringToBuffer(HEX_STRING, HEX_STRING_LENGTH, result);
    assert(isSuccess(CODE));
    return result;
}

} // namespace font
} // namespace typography

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

