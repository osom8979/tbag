/**
 * @file   Box_Info_Test.cpp
 * @brief  Box class tester.
 * @author zer0
 * @date   2019-12-24
 */

#include <gtest/gtest.h>
#include <libtbag/box/Box.hpp>

using namespace libtbag;
using namespace libtbag::box;

TEST(Box_Info_Test, String)
{
    std::string const info = "TEST";

    Box box = {{0}};
    box.setInfo(info);
    ASSERT_EQ(info.size(), box.info_capacity());
    ASSERT_EQ(info.size(), box.info_size());
    ASSERT_EQ(info, box.getInfoString());
}

TEST(Box_Info_Test, Buffer)
{
    Box::Buffer const buffer = {1, 2};

    Box box;
    box.setInfo(buffer);
    ASSERT_EQ(buffer.size(), box.info_capacity());
    ASSERT_EQ(buffer.size(), box.info_size());

    auto const info_buffer = box.getInfoBuffer();
    ASSERT_EQ(buffer.size(), info_buffer.size());
    ASSERT_EQ(buffer[0], info_buffer[0]);
    ASSERT_EQ(buffer[1], info_buffer[1]);
}

TEST(Box_Info_Test, NullAssign)
{
    Box box(nullptr);
    ASSERT_FALSE(static_cast<bool>(box));

    std::string const INFO = "TEST";
    box.setInfo(INFO);

    ASSERT_TRUE(static_cast<bool>(box));
    ASSERT_EQ(INFO, box.getInfoString());
}

TEST(Box_Info_Test, EmptyString)
{
    Box box;
    box.setInfo("");
    ASSERT_EQ(0, box.info_size());
    ASSERT_GE(box.info_capacity(), box.info_size());
}

TEST(Box_Info_Test, EmptyBuffer)
{
    Box box;
    box.setInfo(Box::Buffer());
    ASSERT_EQ(0, box.info_size());
    ASSERT_GE(box.info_capacity(), box.info_size());
}

