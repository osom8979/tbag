/**
 * @file   BoxConverter.cpp
 * @brief  BoxConverter class implementation.
 * @author zer0
 * @date   2020-05-12
 */

#include <libtbag/box/BoxConverter.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/graphic/ImageIO.hpp>

#include <cassert>
#include <cstring>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace box {

using ImageFileFormat = libtbag::graphic::ImageFileFormat;

static Err __convert_image(Box const & src, Box & dest, ImageFileFormat format)
{
    using namespace libtbag::graphic;
    using namespace libtbag::util;
    Buffer buffer;
    auto step = writeImage(buffer, src, format);
    if (isFailure(step)) {
        return step;
    }
    assert(!buffer.empty());
    step = dest.resize<Box::ui8>(buffer.size());
    if (isFailure(step)) {
        return step;
    }
    memcpy(dest.data(), buffer.data(), buffer.size());
    return E_SUCCESS;
}

static Err __convert_text(Box const & src, Box & dest)
{
    auto const text = src.getDataString();
    auto const code = dest.resize<Box::ui8>(text.size());
    if (isFailure(code)) {
        return code;
    }
    memcpy(dest.data(), text.data(), text.size());
    return E_SUCCESS;
}

Err convert(libtbag::net::Mime const & mime, Box const & src, Box & dest)
{
    using namespace libtbag::string;
    auto const subtype = lower(trim(mime.subtype));

    if (mime.isImageType()) {
        if (subtype == "jpeg") {
            return __convert_image(src, dest, ImageFileFormat::IFF_JPG);
        } else if (subtype == "png") {
            return __convert_image(src, dest, ImageFileFormat::IFF_PNG);
        }
    } else if (mime.isTextType()) {
        return __convert_text(src, dest);
    }
    return E_ENOSYS;
}

Err convert(std::string const & mime, Box const & src, Box & dest)
{
    return convert(libtbag::net::Mime(mime), src, dest);
}

} // namespace box

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

