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
#include <libtbag/filesystem/File.hpp>

using namespace libtbag;
using namespace libtbag::graphic;

TEST(ImageIOTest, ReadImage)
{
    auto path = DemoAsset::get_tester_dir_image() / "lena.png";

    Box image;
    ASSERT_EQ(E_SUCCESS, readImage(path.getString(), image));

    ASSERT_EQ(3, image.dim(2));
    ASSERT_EQ(512, image.dim(1));
    ASSERT_EQ(512, image.dim(0));

    // First RGB pixel.
    ASSERT_EQ(226, image.offset<uint8_t>(0)); // r
    ASSERT_EQ(137, image.offset<uint8_t>(1)); // g
    ASSERT_EQ(125, image.offset<uint8_t>(2)); // b

    // Last RGB pixel.
    ASSERT_EQ(185, image.offset<uint8_t>(image.size() - 3)); // r
    ASSERT_EQ( 74, image.offset<uint8_t>(image.size() - 2)); // g
    ASSERT_EQ( 81, image.offset<uint8_t>(image.size() - 1)); // b

    libtbag::util::Buffer buffer;
    ASSERT_EQ(E_SUCCESS, writeImage(buffer, image, ImageFileFormat::IFF_PNG));
    ASSERT_FALSE(buffer.empty());

    // Save & Load.
    tttDir_Automatic();
    auto const SAVE_PATH = tttDir_Get() / "save.png";
    ASSERT_EQ(E_SUCCESS, writeImage(SAVE_PATH.getString(), image));
    ASSERT_EQ(buffer.size(), SAVE_PATH.getState().size);

    Box reload;
    ASSERT_EQ(E_SUCCESS, readImage(SAVE_PATH.getString(), reload));

    ASSERT_EQ(3, image.dim(2));
    ASSERT_EQ(512, reload.dim(1));
    ASSERT_EQ(512, reload.dim(0));

    // First RGB pixel.
    ASSERT_EQ(226, reload.offset<uint8_t>(0)); // r
    ASSERT_EQ(137, reload.offset<uint8_t>(1)); // g
    ASSERT_EQ(125, reload.offset<uint8_t>(2)); // b

    // Last RGB pixel.
    ASSERT_EQ(185, reload.offset<uint8_t>(reload.size() - 3)); // r
    ASSERT_EQ( 74, reload.offset<uint8_t>(reload.size() - 2)); // g
    ASSERT_EQ( 81, reload.offset<uint8_t>(reload.size() - 1)); // b
}

TEST(ImageIOTest, UseJpeg)
{
    auto path = DemoAsset::get_tester_dir_image() / "lena.png";

    Box image;
    ASSERT_EQ(E_SUCCESS, readImage(path.getString(), image));

    // Save & Load.
    tttDir_Automatic();
    auto const JPEG_PATH = tttDir_Get() / "save.jpg";
    ASSERT_EQ(E_SUCCESS, writeImage(JPEG_PATH.getString(), image));
    ASSERT_TRUE(JPEG_PATH.isRegularFile());

    Box reload;
    ASSERT_EQ(E_SUCCESS, readImage(JPEG_PATH.getString(), reload));

    auto const channels = reload.dim(2);
    auto const width = reload.dim(1);
    auto const height = reload.dim(0);

    ASSERT_EQ(3, channels);
    ASSERT_EQ(512, width);
    ASSERT_EQ(512, height);

    // Write To memory:
    libtbag::util::Buffer buffer;
    ASSERT_NE(0, writeJpg(width, height, channels, reload.data<char>(), DEFAULT_JPG_QUALITY, buffer));
    ASSERT_FALSE(buffer.empty());
    auto const JPEG_PATH_02 = tttDir_Get() / "save_02.jpg";
    ASSERT_EQ(E_SUCCESS, libtbag::filesystem::writeFile(JPEG_PATH_02, buffer));
}

