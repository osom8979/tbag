/**
 * @file   OverlayTest.cpp
 * @brief  Overlay class tester.
 * @author zer0
 * @date   2018-06-28
 */

#include <gtest/gtest.h>
#include <libtbag/graphic/Overlay.hpp>

using namespace libtbag;
using namespace libtbag::graphic;

TEST(OverlayTest, Default)
{
    /*
     * 0 | 10 20 30 40 50
     * --+--------------------
     *   |              *   <-- 5TH (50,  5) TEST 12, 13
     * 10|  +--------+
     *   |  |  *     |      <-- 4TH (20, 15) TEST 8, 9, 10, 11
     * 20|  |        |  *   <-- 3RD (50, 20) TEST 7
     *   |  |        |
     * 30|  |     *  |      <-- 2ND (30, 30) TEST 5, 6
     *   |  |        |
     * 40|  +--------+
     *   |
     * 50|              *   <-- 1ST (50, 50) TEST 1, 2, 3, 4
     */
    Overlay<float> overlay(10, 10, 30, 30);

    // TEST1
    ASSERT_FALSE(overlay.update(50, 50, false));
    ASSERT_EQ(OverlayState::OS_NORMAL, overlay.state);
    ASSERT_EQ(MouseEvent::ME_NONE, overlay.event);
    ASSERT_FALSE(overlay.pressed);
    ASSERT_FALSE(overlay.clicked);
    ASSERT_FALSE(overlay.on_event);

    // TEST2
    ASSERT_FALSE(overlay.update(50, 50, true));
    ASSERT_EQ(OverlayState::OS_NORMAL, overlay.state);
    ASSERT_EQ(MouseEvent::ME_DOWN, overlay.event);
    ASSERT_TRUE(overlay.pressed);
    ASSERT_FALSE(overlay.clicked);
    ASSERT_FALSE(overlay.on_event);

    // TEST3
    ASSERT_FALSE(overlay.update(50, 50, false));
    ASSERT_EQ(OverlayState::OS_NORMAL, overlay.state);
    ASSERT_EQ(MouseEvent::ME_UP, overlay.event);
    ASSERT_FALSE(overlay.pressed);
    ASSERT_FALSE(overlay.clicked);
    ASSERT_FALSE(overlay.on_event);

    // TEST4
    ASSERT_FALSE(overlay.update(50, 50, false));
    ASSERT_EQ(OverlayState::OS_NORMAL, overlay.state);
    ASSERT_EQ(MouseEvent::ME_NONE, overlay.event);
    ASSERT_FALSE(overlay.pressed);
    ASSERT_FALSE(overlay.clicked);
    ASSERT_FALSE(overlay.on_event);

    // TEST5
    ASSERT_TRUE(overlay.update(30, 30, false));
    ASSERT_EQ(OverlayState::OS_HOVER, overlay.state);
    ASSERT_EQ(MouseEvent::ME_NONE, overlay.event);
    ASSERT_FALSE(overlay.pressed);
    ASSERT_FALSE(overlay.clicked);
    ASSERT_FALSE(overlay.on_event);

    // TEST6
    ASSERT_TRUE(overlay.update(30, 30, true));
    ASSERT_EQ(OverlayState::OS_ACTIVE, overlay.state);
    ASSERT_EQ(MouseEvent::ME_DOWN, overlay.event);
    ASSERT_TRUE(overlay.pressed);
    ASSERT_TRUE(overlay.clicked);
    ASSERT_FALSE(overlay.on_event);

    // TEST7
    ASSERT_TRUE(overlay.update(50, 20, true));
    ASSERT_EQ(OverlayState::OS_NORMAL, overlay.state);
    ASSERT_EQ(MouseEvent::ME_DOWN, overlay.event);
    ASSERT_TRUE(overlay.pressed);
    ASSERT_TRUE(overlay.clicked);
    ASSERT_FALSE(overlay.on_event);

    // TEST8
    ASSERT_TRUE(overlay.update(20, 15, true));
    ASSERT_EQ(OverlayState::OS_ACTIVE, overlay.state);
    ASSERT_EQ(MouseEvent::ME_DOWN, overlay.event);
    ASSERT_TRUE(overlay.pressed);
    ASSERT_TRUE(overlay.clicked);
    ASSERT_FALSE(overlay.on_event);

    // TEST9
    ASSERT_FALSE(overlay.update(20, 15, true));
    ASSERT_EQ(OverlayState::OS_ACTIVE, overlay.state);
    ASSERT_EQ(MouseEvent::ME_DOWN, overlay.event);
    ASSERT_TRUE(overlay.pressed);
    ASSERT_TRUE(overlay.clicked);
    ASSERT_FALSE(overlay.on_event);

    // TEST10
    ASSERT_TRUE(overlay.update(20, 15, false));
    ASSERT_EQ(OverlayState::OS_HOVER, overlay.state);
    ASSERT_EQ(MouseEvent::ME_UP, overlay.event);
    ASSERT_FALSE(overlay.pressed);
    ASSERT_FALSE(overlay.clicked);
    ASSERT_TRUE(overlay.on_event);

    // TEST11
    ASSERT_FALSE(overlay.update(20, 15, false));
    ASSERT_EQ(OverlayState::OS_HOVER, overlay.state);
    ASSERT_EQ(MouseEvent::ME_NONE, overlay.event);
    ASSERT_FALSE(overlay.pressed);
    ASSERT_FALSE(overlay.clicked);
    ASSERT_FALSE(overlay.on_event);

    // TEST11
    ASSERT_TRUE(overlay.update(50, 5, false));
    ASSERT_EQ(OverlayState::OS_NORMAL, overlay.state);
    ASSERT_EQ(MouseEvent::ME_NONE, overlay.event);
    ASSERT_FALSE(overlay.pressed);
    ASSERT_FALSE(overlay.clicked);
    ASSERT_FALSE(overlay.on_event);

    // TEST12
    ASSERT_FALSE(overlay.update(50, 5, false));
    ASSERT_EQ(OverlayState::OS_NORMAL, overlay.state);
    ASSERT_EQ(MouseEvent::ME_NONE, overlay.event);
    ASSERT_FALSE(overlay.pressed);
    ASSERT_FALSE(overlay.clicked);
    ASSERT_FALSE(overlay.on_event);
}

