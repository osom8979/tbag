/**
 * @file   TextToImageTest.cpp
 * @brief  TextToImage class tester.
 * @author zer0
 * @date   2019-07-18
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/graphic/TextToImage.hpp>
#include <libtbag/graphic/ImageIO.hpp>

using namespace libtbag;
using namespace libtbag::graphic;

TEST(TextToImageTest, Default)
{
    std::string const CASE_NAME = test_info_->test_case_name();
    std::string const TEST_NAME = test_info_->name();

    libtbag::box::Box image;
    ASSERT_TRUE(renderCenteredText(CASE_NAME + "\n" + TEST_NAME, 600, 400, 20, WHITE_COLOR, GRAY_COLOR, image));
    ASSERT_EQ(400, image.dim(0));
    ASSERT_EQ(600, image.dim(1));
    ASSERT_EQ(4, image.dim(2));

    tttDir_Automatic();
    auto const IMAGE_PATH = tttDir_Get() / "text.png";

    ASSERT_EQ(E_SUCCESS, writeImage(IMAGE_PATH, image));
    ASSERT_TRUE(IMAGE_PATH.isRegularFile());

    libtbag::box::Box image2;
    ASSERT_EQ(E_SUCCESS, readImage(IMAGE_PATH, image2));
    ASSERT_EQ(400, image2.dim(0));
    ASSERT_EQ(600, image2.dim(1));
    ASSERT_EQ(4, image2.dim(2));
}

