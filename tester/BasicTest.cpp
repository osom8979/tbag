/**
 * @file   BasicTest.cpp
 * @brief  Basic tester.
 * @author zer0
 * @date   2016-01-18
 */

#include <gtest/gtest.h>
#include <string>

TEST(BasicTest, String)
{
    ASSERT_EQ(L'T', static_cast<wchar_t>( 'T'));
    ASSERT_EQ( 'T', static_cast<   char>(L'T'));

    std::wstring const WSTRING_TEST = {'T', 'E', 'S', 'T'};
    ASSERT_STREQ(L"TEST", WSTRING_TEST.c_str());
}

TEST(BasicTest, Integer)
{
    ASSERT_EQ(0, 0);
}

