/**
 * @file   TestUtilsTest.cpp
 * @brief  TestUtils class tester.
 * @author zer0
 * @date   2018-03-24
 */

#include <gtest/gtest.h>
#include <libtbag/util/TestUtils.hpp>

using namespace libtbag;
using namespace libtbag::util;

TEST(TestUtilsTest, Default)
{
    TBAG_SCENARIO("Scenario1", "tester") {
        TBAG_GIVEN("Given1") {
            TBAG_WHEN("When1") {
                TBAG_THEN("Then1") {
                    ASSERT_TRUE(true);
                }
                TBAG_THEN("Then2") {
                    ASSERT_TRUE(true);
                }
            }
            TBAG_WHEN("When2") {
                ASSERT_TRUE(true);
            }
        }
        TBAG_GIVEN("Given2") {
            ASSERT_TRUE(true);
        }
    }
    TBAG_SCENARIO("Scenario2") {
        ASSERT_TRUE(true);
    }
    TBAG_SCENARIO("Scenario3") {
        TBAG_GIVEN("Given3") {
            TBAG_WHEN("When3") {
                TBAG_THEN("Then3") {
                    ASSERT_TRUE(true);
                }
            }
        }
    }
}

