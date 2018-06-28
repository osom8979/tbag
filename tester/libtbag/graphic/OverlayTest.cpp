/**
 * @file   OverlayTest.cpp
 * @brief  Overlay class tester.
 * @author zer0
 * @date   2018-06-28
 */

#include <gtest/gtest.h>
#include <libtbag/graphic/Overlay.hpp>
#include <libtbag/util/TestUtils.hpp>

using namespace libtbag;
using namespace libtbag::graphic;

TEST(OverlayTest, Default)
{
    TBAG_SCENARIO("Overlay update scenario") {
        Overlay<float> overlay;

        TBAG_GIVEN("Update overlay area") {
            /*
             * 0 | 10 20 30 40 50
             * --+--------------------
             *   |              *   <-- 5TH (50,  5) OUT TEST 12, 13
             * 10|  +--------+
             *   |  |  *     |      <-- 4TH (20, 15) IN TEST 8, 9, 10, 11
             * 20|  |        |  *   <-- 3RD (50, 20) OUT TEST 7
             *   |  |        |
             * 30|  |     *  |      <-- 2ND (30, 30) IN TEST 5, 6
             *   |  |        |
             * 40|  +--------+
             *   |
             * 50|              *   <-- 1ST (50, 50) OUT TEST 1, 2, 3, 4
             */
            overlay.area = {10, 10, 30, 30};

            TBAG_WHEN("1ST") {
                float const TEST_X = 50;
                float const TEST_Y = 50;

                TBAG_THEN("TEST 1 (OUT)") {
                    ASSERT_FALSE(overlay.update(TEST_X, TEST_Y, false));
                    ASSERT_EQ(OverlayState::OS_NORMAL, overlay.state);
                    ASSERT_EQ(MouseEvent::ME_NONE, overlay.event);
                    ASSERT_FALSE(overlay.pressed);
                    ASSERT_FALSE(overlay.clicked);
                    ASSERT_FALSE(overlay.on_event);
                }

                TBAG_THEN("TEST 2 (OUT & CLICK)") {
                    ASSERT_FALSE(overlay.update(TEST_X, TEST_Y, true));
                    ASSERT_EQ(OverlayState::OS_NORMAL, overlay.state);
                    ASSERT_EQ(MouseEvent::ME_DOWN, overlay.event);
                    ASSERT_TRUE(overlay.pressed);
                    ASSERT_FALSE(overlay.clicked);
                    ASSERT_FALSE(overlay.on_event);
                }

                TBAG_THEN("TEST 3 (OUT)") {
                    ASSERT_FALSE(overlay.update(TEST_X, TEST_Y, false));
                    ASSERT_EQ(OverlayState::OS_NORMAL, overlay.state);
                    ASSERT_EQ(MouseEvent::ME_UP, overlay.event);
                    ASSERT_FALSE(overlay.pressed);
                    ASSERT_FALSE(overlay.clicked);
                    ASSERT_FALSE(overlay.on_event);
                }

                TBAG_THEN("TEST 4 (OUT // STAY)") {
                    ASSERT_FALSE(overlay.update(TEST_X, TEST_Y, false));
                    ASSERT_EQ(OverlayState::OS_NORMAL, overlay.state);
                    ASSERT_EQ(MouseEvent::ME_NONE, overlay.event);
                    ASSERT_FALSE(overlay.pressed);
                    ASSERT_FALSE(overlay.clicked);
                    ASSERT_FALSE(overlay.on_event);
                }
            }

            TBAG_WHEN("2ND") {
                float const TEST_X = 30;
                float const TEST_Y = 30;

                TBAG_THEN("TEST 5 (OUT -> IN)") {
                    ASSERT_TRUE(overlay.update(TEST_X, TEST_Y, false));
                    ASSERT_EQ(OverlayState::OS_HOVER, overlay.state);
                    ASSERT_EQ(MouseEvent::ME_NONE, overlay.event);
                    ASSERT_FALSE(overlay.pressed);
                    ASSERT_FALSE(overlay.clicked);
                    ASSERT_FALSE(overlay.on_event);
                }

                TBAG_THEN("TEST 6 (IN & CLICK)") {
                    ASSERT_TRUE(overlay.update(TEST_X, TEST_Y, true));
                    ASSERT_EQ(OverlayState::OS_ACTIVE, overlay.state);
                    ASSERT_EQ(MouseEvent::ME_DOWN, overlay.event);
                    ASSERT_TRUE(overlay.pressed);
                    ASSERT_TRUE(overlay.clicked);
                    ASSERT_FALSE(overlay.on_event);
                }
            }

            TBAG_WHEN("3RD") {
                float const TEST_X = 50;
                float const TEST_Y = 20;

                TBAG_THEN("TEST 7 (IN -> OUT & CLICK)") {
                    ASSERT_TRUE(overlay.update(TEST_X, TEST_Y, true));
                    ASSERT_EQ(OverlayState::OS_NORMAL, overlay.state);
                    ASSERT_EQ(MouseEvent::ME_DOWN, overlay.event);
                    ASSERT_TRUE(overlay.pressed);
                    ASSERT_TRUE(overlay.clicked);
                    ASSERT_FALSE(overlay.on_event);
                }
            }

            TBAG_WHEN("4TH") {
                float const TEST_X = 20;
                float const TEST_Y = 15;

                TBAG_THEN("TEST 8 (OUT -> IN & CLICK)") {
                    ASSERT_TRUE(overlay.update(TEST_X, TEST_Y, true));
                    ASSERT_EQ(OverlayState::OS_ACTIVE, overlay.state);
                    ASSERT_EQ(MouseEvent::ME_DOWN, overlay.event);
                    ASSERT_TRUE(overlay.pressed);
                    ASSERT_TRUE(overlay.clicked);
                    ASSERT_FALSE(overlay.on_event);
                }

                TBAG_THEN("TEST 9 (IN & CLICK)") {
                    ASSERT_FALSE(overlay.update(TEST_X, TEST_Y, true));
                    ASSERT_EQ(OverlayState::OS_ACTIVE, overlay.state);
                    ASSERT_EQ(MouseEvent::ME_DOWN, overlay.event);
                    ASSERT_TRUE(overlay.pressed);
                    ASSERT_TRUE(overlay.clicked);
                    ASSERT_FALSE(overlay.on_event);
                }

                TBAG_THEN("TEST 10 (IN)") {
                    ASSERT_TRUE(overlay.update(TEST_X, TEST_Y, false));
                    ASSERT_EQ(OverlayState::OS_HOVER, overlay.state);
                    ASSERT_EQ(MouseEvent::ME_UP, overlay.event);
                    ASSERT_FALSE(overlay.pressed);
                    ASSERT_FALSE(overlay.clicked);
                    ASSERT_TRUE(overlay.on_event);
                }

                TBAG_THEN("TEST 11 (IN // STAY)") {
                    ASSERT_FALSE(overlay.update(TEST_X, TEST_Y, false));
                    ASSERT_EQ(OverlayState::OS_HOVER, overlay.state);
                    ASSERT_EQ(MouseEvent::ME_NONE, overlay.event);
                    ASSERT_FALSE(overlay.pressed);
                    ASSERT_FALSE(overlay.clicked);
                    ASSERT_FALSE(overlay.on_event);
                }
            }

            TBAG_WHEN("5TH") {
                float const TEST_X = 50;
                float const TEST_Y =  5;

                TBAG_THEN("TEST 12 (IN -> OUT)") {
                    ASSERT_TRUE(overlay.update(TEST_X, TEST_Y, false));
                    ASSERT_EQ(OverlayState::OS_NORMAL, overlay.state);
                    ASSERT_EQ(MouseEvent::ME_NONE, overlay.event);
                    ASSERT_FALSE(overlay.pressed);
                    ASSERT_FALSE(overlay.clicked);
                    ASSERT_FALSE(overlay.on_event);
                }

                TBAG_THEN("TEST 13 (OUT // STAY)") {
                    ASSERT_FALSE(overlay.update(TEST_X, TEST_Y, false));
                    ASSERT_EQ(OverlayState::OS_NORMAL, overlay.state);
                    ASSERT_EQ(MouseEvent::ME_NONE, overlay.event);
                    ASSERT_FALSE(overlay.pressed);
                    ASSERT_FALSE(overlay.clicked);
                    ASSERT_FALSE(overlay.on_event);
                }
            }
        }
    }
}

