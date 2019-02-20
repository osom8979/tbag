/**
 * @file   ImageIOTest.cpp
 * @brief  ImageIO class tester.
 * @author zer0
 * @date   2017-06-10
 * @date   2019-02-20 (Rename: Image -> ImageIO)
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/graphic/ImageIO.hpp>
#include <libtbag/filesystem/Path.hpp>

using namespace libtbag;
using namespace libtbag::graphic;

TEST(ImageIOTest, ReadImage)
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

    util::Buffer buffer;
    ASSERT_EQ(Err::E_SUCCESS, writeImage(buffer, image, ImageFileFormat::IFF_PNG));
    ASSERT_FALSE(buffer.empty());

    // Save & Load.
    tttDir_Automatic();
    auto const SAVE_PATH = tttDir_Get() / "save.png";
    ASSERT_EQ(Err::E_SUCCESS, writeImage(SAVE_PATH.getString(), image));
    ASSERT_EQ(buffer.size(), SAVE_PATH.getState().size);

    Image reload;
    ASSERT_EQ(Err::E_SUCCESS, readImage(SAVE_PATH.getString(), reload));

    ASSERT_EQ(512, reload.width());
    ASSERT_EQ(512, reload.height());

    // First RGB pixel.
    ASSERT_EQ(226, reload[0].r);
    ASSERT_EQ(137, reload[0].g);
    ASSERT_EQ(125, reload[0].b);

    // Last RGB pixel.
    ASSERT_EQ(185, reload[reload.size() - 1].r);
    ASSERT_EQ( 74, reload[reload.size() - 1].g);
    ASSERT_EQ( 81, reload[reload.size() - 1].b);
}

TEST(ImageIOTest, UseJpeg)
{
    auto path = DemoAsset::get_tester_dir_image() / "lena.png";

    Image image;
    ASSERT_EQ(Err::E_SUCCESS, readImage(path.getString(), image));

    // Save & Load.
    tttDir_Automatic();
    auto const JPEG_PATH = tttDir_Get() / "save.jpg";
    ASSERT_EQ(Err::E_SUCCESS, writeImage(JPEG_PATH.getString(), image));
    ASSERT_TRUE(JPEG_PATH.isRegularFile());

    Image reload;
    ASSERT_EQ(Err::E_SUCCESS, readImage(JPEG_PATH.getString(), reload));

    ASSERT_EQ(512, reload.width());
    ASSERT_EQ(512, reload.height());
}

