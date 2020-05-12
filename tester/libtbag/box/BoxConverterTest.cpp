/**
 * @file   BoxConverterTest.cpp
 * @brief  BoxConverter class tester.
 * @author zer0
 * @date   2020-05-12
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/box/BoxConverter.hpp>
#include <libtbag/graphic/ImageIO.hpp>
#include <libtbag/filesystem/File.hpp>

using namespace libtbag;
using namespace libtbag::box;
using namespace libtbag::graphic;
using namespace libtbag::filesystem;

TEST(BoxConverterTest, ImageJpeg)
{
    auto const path = DemoAsset::get_tester_dir_image() / "lena.png";

    Box image;
    ASSERT_EQ(E_SUCCESS, readImage(path, image));
    ASSERT_EQ(3, image.rank());
    ASSERT_EQ(3, image.dim(2));
    ASSERT_EQ(512, image.dim(1));
    ASSERT_EQ(512, image.dim(0));

    Box jpeg_image;
    ASSERT_EQ(E_SUCCESS, convert("image/jpeg", image, jpeg_image));
    ASSERT_FALSE(jpeg_image.empty());

    tttDir_Automatic();
    auto const save_path = tttDir_Get() / "save.jpg";
    ASSERT_EQ(E_SUCCESS, writeFile(save_path, jpeg_image.data<char>(),
                                   jpeg_image.total_data_byte()));

    Box read_image;
    ASSERT_EQ(E_SUCCESS, readImage(save_path, read_image));

    ASSERT_EQ(3, read_image.rank());
    ASSERT_EQ(3, read_image.dim(2));
    ASSERT_EQ(512, read_image.dim(1));
    ASSERT_EQ(512, read_image.dim(0));
}

