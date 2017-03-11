/**
 * @file   ProfileTest.cpp
 * @brief  Profile class tester.
 * @author zer0
 * @date   2016-09-21
 */

#include <gtest/gtest.h>
#include <libtbag/debug/Profile.hpp>

using namespace libtbag;
using namespace libtbag::debug;

TEST(ProfileTest, Default)
{
    std::size_t const CYCLE =  2;
    std::size_t const TOTAL = 11;

    int update_callback = 0;

    Profile profile(CYCLE, [&](Profile::Rep average){
        ++update_callback;
    });

    for (std::size_t i = 0; i < TOTAL; ++i) {
        profile.tick();
    }

    ASSERT_EQ(static_cast<std::size_t>(TOTAL / CYCLE), update_callback);
}

TEST(ProfileTest, GetUnitString)
{
    ASSERT_FALSE(std::string(Profile::getTimeUnitString()).empty());
}

