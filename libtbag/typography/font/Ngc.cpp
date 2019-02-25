/**
 * @file   Ngc.cpp
 * @brief  Ngc class implementation.
 * @author zer0
 * @date   2019-02-22
 */

#include <libtbag/typography/font/Ngc.hpp>
//#include <libtbag/typography/font/ngc/ngcn.h.inl>
//#include <libtbag/typography/font/ngc/ngcb.h.inl>

#include <cassert>
#include <cstring>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace typography {
namespace font       {

libtbag::util::Buffer getNgcNormal() TBAG_NOEXCEPT
{
//    auto const * const HEX_STRING = (char const * const)__get_binary_to_cpp_array__ngcn__();
//    return libtbag::util::Buffer(HEX_STRING, HEX_STRING + __get_binary_to_cpp_array_size__ngcn__());
    return libtbag::util::Buffer();
}

libtbag::util::Buffer getNgcBold() TBAG_NOEXCEPT
{
//    auto const * const HEX_STRING = (char const * const)__get_binary_to_cpp_array__ngcb__();
//    return libtbag::util::Buffer(HEX_STRING, HEX_STRING + __get_binary_to_cpp_array_size__ngcb__());
    return libtbag::util::Buffer();
}

} // namespace font
} // namespace typography

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

