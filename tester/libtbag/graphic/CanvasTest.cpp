/**
 * @file   CanvasTest.cpp
 * @brief  Canvas class tester.
 * @author zer0
 * @date   2020-01-11
 */

#include <gtest/gtest.h>
#include <libtbag/graphic/Canvas.hpp>

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

int const CanvasFixtureTest::TEST_WIDTH = 10;
int const CanvasFixtureTest::TEST_HEIGHT = 20;
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

