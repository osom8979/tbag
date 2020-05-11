/**
 * @file   MimeTest.cpp
 * @brief  Mime class tester.
 * @author zer0
 * @date   2020-05-11
 */

#include <gtest/gtest.h>
#include <libtbag/net/Mime.hpp>

using namespace libtbag;
using namespace libtbag::net;

TEST(MimeTest, Default)
{
    Mime image_png;
    image_png.type = "image";
    image_png.subtype = "png";
    ASSERT_STREQ("image/png", image_png.toString().c_str());
}

