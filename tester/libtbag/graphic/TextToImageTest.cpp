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
#include <libtbag/string/Format.hpp>

using namespace libtbag;
using namespace libtbag::graphic;

TEST(TextToImageTest, Default)
{
    tttDir_Automatic();
    auto const TEMP_DIR = tttDir_Get();

    std::string const CASE_NAME = test_info_->test_case_name();
    std::string const TEST_NAME = test_info_->name();

    auto internal_task = [&](int width, int height, int channels){
        using namespace libtbag::string;
        std::cout << fformat("Internal task: {}x{}x{}", width, height, channels) << std::endl;
        auto const RENDER_TEXT = fformat("{}\n{}\n{}x{}x{}", CASE_NAME, TEST_NAME, width, height, channels);
        auto const FILE_NAME = fformat("{}{}-{}x{}x{}.png", CASE_NAME, TEST_NAME, width, height, channels);
        auto const IMAGE_PATH = TEMP_DIR / FILE_NAME;

        libtbag::box::Box image;
        auto const render_result = renderCenteredText(RENDER_TEXT, width, height, channels, 20,
                                                      WHITE_COLOR, GRAY_COLOR, image);
        ASSERT_TRUE(isSuccess(render_result));
        ASSERT_EQ(height, image.dim(0));
        ASSERT_EQ(width, image.dim(1));
        ASSERT_EQ(channels, image.dim(2));

        ASSERT_EQ(E_SUCCESS, writeImage(IMAGE_PATH, image));
        ASSERT_TRUE(IMAGE_PATH.isRegularFile());

        libtbag::box::Box image2;
        ASSERT_EQ(E_SUCCESS, readImage(IMAGE_PATH, image2));
        ASSERT_EQ(height, image2.dim(0));
        ASSERT_EQ(width, image2.dim(1));
        ASSERT_EQ(channels, image2.dim(2));
    };

    internal_task(600, 400, 4);
    internal_task(600, 400, 3);
    internal_task(600, 400, 1);
}

TEST(TextToImageTest, EmptyImage)
{
    libtbag::box::Box image;
    auto const render_result = renderCenteredText({}, 100, 100, 1, 20,
                                                  WHITE_COLOR, BLACK_COLOR, image);
    ASSERT_TRUE(isSuccess(render_result));
    ASSERT_EQ(100, image.dim(0));
    ASSERT_EQ(100, image.dim(1));
    ASSERT_EQ(1, image.dim(2));

    auto const * pixels = image.cast<std::uint8_t>();
    for (auto i = 0u; i < image.size(); ++i) {
        ASSERT_EQ(0, *pixels);
    }
}

