/**
 * @file   CommanderTest.cpp
 * @brief  Commander class tester.
 * @author zer0
 * @date   2019-11-09
 */

#include <gtest/gtest.h>
#include <libtbag/functional/Commander.hpp>

using namespace libtbag;
using namespace libtbag::functional;

TEST(CommanderTest, Default)
{
    Commander<std::string, int(int, int)> commander;
    commander->emplace("test", [](int a, int b) -> auto {
        return a + b;
    });

    ASSERT_EQ(1u, commander.size());
    ASSERT_FALSE(commander.empty());

    ASSERT_TRUE(commander.exists("test"));
    auto result1 = commander("test", 100, 20);
    ASSERT_EQ(E_SUCCESS, result1);
    ASSERT_EQ(120, result1.val);

    ASSERT_FALSE(commander.exists("rand"));
    auto result2 = commander("rand", 200, 30);
    ASSERT_EQ(E_NFOUND, result2);
}

