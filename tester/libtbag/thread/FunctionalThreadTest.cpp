/**
 * @file   FunctionalThreadTest.cpp
 * @brief  FunctionalThread class tester.
 * @author zer0
 * @date   2019-07-17
 */

#include <gtest/gtest.h>
#include <libtbag/thread/FunctionalThread.hpp>

using namespace libtbag;
using namespace libtbag::thread;

TEST(FunctionalThreadTest, Default)
{
    int test_number = 0;
    FunctionalThread t([&](){
        test_number = 100;
    });
    ASSERT_EQ(E_SUCCESS, t.run());
    ASSERT_EQ(E_SUCCESS, t.join());
    ASSERT_EQ(100, test_number);
}

TEST(FunctionalThreadTest, Exception)
{
    FunctionalThread t([&](){
        throw std::runtime_error("ERROR");
    });
    ASSERT_EQ(E_SUCCESS, t.run());
    ASSERT_THROW(t.join(), std::runtime_error);
}

TEST(FunctionalThreadTest, NoException)
{
    FunctionalThread t([&](){
        throw std::runtime_error("ERROR");
    });
    ASSERT_EQ(E_SUCCESS, t.run());
    ASSERT_NO_THROW(t.join(false));
}

