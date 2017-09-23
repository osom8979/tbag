/**
 * @file   BasicTest.cpp
 * @brief  Basic tester.
 * @author zer0
 * @date   2016-01-18
 */

#include <gtest/gtest.h>

#include <vector>
#include <string>

TEST(BasicTest, String)
{
    ASSERT_EQ(L'T', static_cast<wchar_t>( 'T'));
    ASSERT_EQ( 'T', static_cast<   char>(L'T'));

    std::wstring const WSTRING_TEST = {'T', 'E', 'S', 'T'};
    ASSERT_STREQ(L"TEST", WSTRING_TEST.c_str());
}

TEST(BasicTest, Vector)
{
    std::vector<char> buffer;
    std::size_t cap1 = buffer.capacity();
    std::size_t size1 = buffer.size();
    char * pointer1 = buffer.data();
    ASSERT_GE(cap1, size1);

    buffer.resize(cap1 + 10);
    std::size_t cap2 = buffer.capacity();
    std::size_t size2 = buffer.size();
    char * pointer2 = buffer.data();
    ASSERT_GE(cap2, size2);

    buffer.resize(cap1);
    std::size_t cap3 = buffer.capacity();
    std::size_t size3 = buffer.size();
    char * pointer3 = buffer.data();
    ASSERT_GE(cap2, size2);

    ASSERT_NE(pointer1, pointer2);
    ASSERT_EQ(pointer2, pointer3);
    ASSERT_EQ(cap2, cap3);
}

TEST(BasicTest, Integer)
{
    ASSERT_EQ(0, 0);
}
