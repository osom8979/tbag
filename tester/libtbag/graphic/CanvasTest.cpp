/**
 * @file   CanvasTest.cpp
 * @brief  Canvas class tester.
 * @author zer0
 * @date   2020-01-11
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/graphic/Canvas.hpp>
#include <libtbag/graphic/ImageIO.hpp>

using namespace libtbag;
using namespace libtbag::graphic;
using namespace libtbag::box;

TEST(CanvasTest, Default)
{
    Canvas canvas;
    ASSERT_FALSE(canvas.exists());
}

TEST(CanvasTest, FirstReset)
{
    Canvas canvas;
    ASSERT_FALSE(canvas.exists());
    canvas.reset();
    ASSERT_FALSE(canvas.exists());
}

TEST(CanvasTest, OnlyInit)
{
    Canvas canvas;
    ASSERT_EQ(E_SUCCESS, canvas.init(100, 100, 3));
    ASSERT_TRUE(canvas.exists());
}

struct CanvasFixtureTest : public testing::Test
{
    static int const TEST_WIDTH;
    static int const TEST_HEIGHT;
    static int const TEST_CHANNELS;

    Canvas canvas;

    void SetUp() override
    {
        canvas.init(TEST_WIDTH, TEST_HEIGHT, TEST_CHANNELS);
    }

    void TearDown() override
    {
        canvas.reset();
    }
};

int const CanvasFixtureTest::TEST_WIDTH = 800;
int const CanvasFixtureTest::TEST_HEIGHT = 600;
int const CanvasFixtureTest::TEST_CHANNELS = 3;

TEST_F(CanvasFixtureTest, Default)
{
    ASSERT_TRUE(canvas.exists());
    Box result;
    ASSERT_EQ(E_SUCCESS, canvas.toBox(result));
    ASSERT_TRUE(result.is_ui8());
    ASSERT_EQ(TEST_HEIGHT, result.dim(0));
    ASSERT_EQ(TEST_WIDTH, result.dim(1));
    ASSERT_EQ(TEST_CHANNELS, result.dim(2));
}

TEST_F(CanvasFixtureTest, Fill)
{
    ASSERT_TRUE(canvas.exists());
    ASSERT_EQ(E_SUCCESS, canvas.setFillStyle(libtbag::graphic::RED_COLOR));
    ASSERT_EQ(E_SUCCESS, canvas.fillAll());
    ASSERT_EQ(E_SUCCESS, canvas.end());

    Box result;
    ASSERT_EQ(E_SUCCESS, canvas.toBox(result));
    ASSERT_TRUE(result.is_ui8());
    ASSERT_EQ(TEST_HEIGHT, result.dim(0));
    ASSERT_EQ(TEST_WIDTH, result.dim(1));
    ASSERT_EQ(TEST_CHANNELS, result.dim(2));
    ASSERT_TRUE((result.slice({ {}, {}, {0, 1} }) == 0x00).all());
    ASSERT_TRUE((result.slice({ {}, {}, {1, 2} }) == 0x00).all());
    ASSERT_TRUE((result.slice({ {}, {}, {2, 3} }) == 0xFF).all());
}

TEST_F(CanvasFixtureTest, BlitImage)
{
    auto path = DemoAsset::get_tester_dir_image() / "lena.png";
    Box image;
    ASSERT_EQ(E_SUCCESS, readRgbaImage(path.getString(), image));

    ASSERT_TRUE(canvas.exists());
    ASSERT_EQ(E_SUCCESS, canvas.setFillStyle(libtbag::graphic::BLACK_COLOR));
    ASSERT_EQ(E_SUCCESS, canvas.fillAll());
    ASSERT_EQ(E_SUCCESS, canvas.blitImage(Canvas::Point2i(0, 0), image));
    ASSERT_EQ(E_SUCCESS, canvas.end());

    Box result;
    ASSERT_EQ(E_SUCCESS, canvas.toBox(result));
    ASSERT_TRUE(result.is_ui8());
    ASSERT_EQ(TEST_HEIGHT, result.dim(0));
    ASSERT_EQ(TEST_WIDTH, result.dim(1));
    ASSERT_EQ(TEST_CHANNELS, result.dim(2));

    // Save & Load.
    tttDir_Automatic();
    auto const SAVE_PATH = tttDir_Get() / "save.png";
    ASSERT_EQ(E_SUCCESS, writeImage(SAVE_PATH.getString(), result));

    Box reload;
    ASSERT_EQ(E_SUCCESS, readImage(SAVE_PATH.getString(), reload));

    ASSERT_EQ(TEST_HEIGHT, reload.dim(0));
    ASSERT_EQ(TEST_WIDTH, reload.dim(1));
    ASSERT_EQ(TEST_CHANNELS, reload.dim(2));

    // First RGB pixel.
    ASSERT_EQ(226, reload.offset<uint8_t>(0)); // r
    ASSERT_EQ(137, reload.offset<uint8_t>(1)); // g
    ASSERT_EQ(125, reload.offset<uint8_t>(2)); // b

    // Last RGB pixel.
    ASSERT_EQ(0, reload.offset<uint8_t>(reload.size() - 4));
    ASSERT_EQ(0, reload.offset<uint8_t>(reload.size() - 3));
    ASSERT_EQ(0, reload.offset<uint8_t>(reload.size() - 2));
}

