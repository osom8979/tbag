/**
 * @file   OverlayStateTest.cpp
 * @brief  OverlayState class tester.
 * @author zer0
 * @date   2018-06-18
 */

#include <gtest/gtest.h>
#include <libtbag/graphic/OverlayState.hpp>

using namespace libtbag;
using namespace libtbag::graphic;

TBAG_CONSTEXPR static int const RECT_X =  0;
TBAG_CONSTEXPR static int const RECT_Y =  0;
TBAG_CONSTEXPR static int const RECT_W = 10;
TBAG_CONSTEXPR static int const RECT_H = 10;

TBAG_CONSTEXPR static int const IN_X = 5;
TBAG_CONSTEXPR static int const IN_Y = 5;

TBAG_CONSTEXPR static int const OUT_X = 15;
TBAG_CONSTEXPR static int const OUT_Y = 15;

TEST(OverlayStateTest, Out_In)
{
    bool save_clicked = false;
    auto result = calcOverlayState(RECT_X, RECT_Y, RECT_W, RECT_H, OUT_X, OUT_Y, MouseEvent::ME_NONE, save_clicked);
    ASSERT_EQ(OverlayState::OS_NORMAL, result.first);
    ASSERT_FALSE(result.second);
    ASSERT_FALSE(save_clicked);

    result = calcOverlayState(RECT_X, RECT_Y, RECT_W, RECT_H, IN_X, IN_Y, MouseEvent::ME_NONE, save_clicked);
    ASSERT_EQ(OverlayState::OS_HOVER, result.first);
    ASSERT_FALSE(result.second);
    ASSERT_FALSE(save_clicked);

    result = calcOverlayState(RECT_X, RECT_Y, RECT_W, RECT_H, IN_X, IN_Y, MouseEvent::ME_DOWN, save_clicked);
    ASSERT_EQ(OverlayState::OS_ACTIVE, result.first);
    ASSERT_FALSE(result.second);
    ASSERT_TRUE(save_clicked);

    result = calcOverlayState(RECT_X, RECT_Y, RECT_W, RECT_H, IN_X, IN_Y, MouseEvent::ME_UP, save_clicked);
    ASSERT_EQ(OverlayState::OS_HOVER, result.first);
    ASSERT_TRUE(result.second);
    ASSERT_FALSE(save_clicked);
}

