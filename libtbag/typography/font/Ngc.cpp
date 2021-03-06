/**
 * @file   Ngc.cpp
 * @brief  Ngc class implementation.
 * @author zer0
 * @date   2019-02-22
 */

#include <libtbag/typography/font/Ngc.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/archive/Archive.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <libtbag/typography/font/ngc/ngcn.h.inl>
#include <libtbag/typography/font/ngc/ngcb.h.inl>

#include <cassert>
#include <cstring>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace typography {
namespace font       {

static libtbag::util::Buffer __get_decompressed_7z_font(char const * hex_text)
{
    assert(hex_text != nullptr);

    using namespace libtbag::util;
    using namespace libtbag::string;
    using namespace libtbag::archive;

    Buffer buffer;
    auto const CODE = convertHexStringToBuffer(hex_text, strlen(hex_text), buffer);
    if (isFailure(CODE)) {
        tDLogE("__get_decompressed_7z_font() Convert error: {}", CODE);
        return Buffer();
    }

    BaseArchive::MemoryEntries entries;
    auto const FILE_COUNT = decompressMemoryArchive(buffer.data(), buffer.size(), entries);
    if (FILE_COUNT != 1) {
        tDLogE("__get_decompressed_7z_font() File count error: {}", FILE_COUNT);
        return Buffer();
    }

    assert(entries.size() == 1);
    return entries[0].data;
}

libtbag::util::Buffer getNgcNormal()
{
    auto const FONT_7Z_HEX_TEXT = libtbag::string::mergeArgv((char**)__data_to_string__ngcn__);
    return __get_decompressed_7z_font(FONT_7Z_HEX_TEXT.c_str());
}

libtbag::util::Buffer getNgcBold()
{
    auto const FONT_7Z_HEX_TEXT = libtbag::string::mergeArgv((char**)__data_to_string__ngcb__);
    return __get_decompressed_7z_font(FONT_7Z_HEX_TEXT.c_str());
}

} // namespace font
} // namespace typography

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

