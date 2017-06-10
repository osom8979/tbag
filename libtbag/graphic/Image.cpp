/**
 * @file   Image.cpp
 * @brief  Image class implementation.
 * @author zer0
 * @date   2017-06-10
 */

#include <libtbag/graphic/Image.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/filesystem/Path.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace graphic {

Err readImage(std::string const & path, Image & image)
{
    if (filesystem::Path(path).exists() == false) {
        return Err::E_EEXIST;
    }
    //if (filesystem::Path(path).isReadable() == false) {
    //    return Err::E_RDERR;
    //}

    int   width = 0;
    int  height = 0;
    int channel = 0;

    // process data if not NULL ...
    // x = width, y = height, n = # 8-bit components per pixel ...
    // replace '0' with '1'..'4' to force that many components per pixel
    // but 'n' will always be the number that it would have been if you said 0
    unsigned char * data = ::stbi_load(path.c_str(), &width, &height, &channel, 0);
    if (data == nullptr) {
        return Err::E_RDERR;
    }

    if (channel == 3) {
        image.resize(width, height);
        image.resize(width, height);
        image.assign(data, width * height * channel);
    }

    stbi_image_free(data);
    return Err::E_SUCCESS;
}

} // namespace graphic

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

