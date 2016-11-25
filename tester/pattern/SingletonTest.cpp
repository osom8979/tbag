/**
 * @file   SingletonTest.cpp
 * @brief  Singleton class tester.
 * @author zer0
 * @date   2016-04-22
 */

#include <gtest/gtest.h>
#include <libtbag/pattern/Singleton.hpp>

using namespace libtbag;
using namespace libtbag::pattern;

class DemoSingleton : SINGLETON_INHERITANCE(DemoSingleton)
{
public:
    SINGLETON_RESTRICT(DemoSingleton);

public:
    inline int test() const
    { return 1; }
};

TEST(SingletonTest, Default)
{
    ASSERT_EQ(1, DemoSingleton::getInstance()->test());
}

