/**
 * @file   Basic.cpp
 * @brief  Basic tester.
 * @author zer0
 * @date   2016-01-18
 */

#include <gtest/gtest.h>
#include <string>

TEST(Basic, String)
{
    ASSERT_EQ(static_cast<wchar_t>('T'), L'T');
    ASSERT_EQ(static_cast<char>(L'T'), 'T');

    std::wstring const WSTRING_TEST = {'T', 'E', 'S', 'T'};
    ASSERT_STREQ(WSTRING_TEST.c_str(), L"TEST");
};

TEST(Basic, Integer)
{
    ASSERT_EQ(0, 0);
}

