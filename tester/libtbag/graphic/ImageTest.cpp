/**
 * @file   ImageTest.cpp
 * @brief  Image class tester.
 * @author zer0
 * @date   2017-06-10
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/graphic/Image.hpp>

using namespace libtbag;
using namespace libtbag::graphic;

TEST(ImageTest, ReadImage)
{
    auto path = DemoAsset::get_tester_dir_image() / "lena.png";

    Image image;
    ASSERT_EQ(Err::E_SUCCESS, readImage(path.getString(), image));

    ASSERT_EQ(512, image.width());
    ASSERT_EQ(512, image.height());

    // First RGB pixel.
    ASSERT_EQ(226, image[0].r);
    ASSERT_EQ(137, image[0].g);
    ASSERT_EQ(125, image[0].b);

    // Last RGB pixel.
    ASSERT_EQ(185, image[image.size() - 1].r);
    ASSERT_EQ( 74, image[image.size() - 1].g);
    ASSERT_EQ( 81, image[image.size() - 1].b);
}

