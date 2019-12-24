/**
 * @file   Box_Encode_Test.cpp
 * @brief  Box class tester.
 * @author zer0
 * @date   2019-12-24
 */

#include <gtest/gtest.h>
#include <libtbag/box/Box.hpp>

using namespace libtbag;
using namespace libtbag::box;

TEST(Box_Encode_Test, Default)
{
    std::string const INFO = "{TEMP STRING}";
    Box box = {{11, 22}};
    box.setInfo(INFO);
    ASSERT_TRUE(box.is_device_cpu());
    ASSERT_TRUE(box.is_si32());
    ASSERT_EQ(2, box.rank());
    ASSERT_EQ(2, box.size());
    ASSERT_EQ(1, box.dim(0));
    ASSERT_EQ(2, box.dim(1));
    ASSERT_EQ(11, box.getElementByOffset<si32>(0));
    ASSERT_EQ(22, box.getElementByOffset<si32>(1));
    ASSERT_EQ(INFO.size(), box.info_capacity());
    ASSERT_EQ(INFO.size(), box.info_size());
    ASSERT_EQ(INFO, box.getInfoString());

    Box::Buffer buffer;
    ASSERT_EQ(E_SUCCESS, box.encode(buffer));
    ASSERT_FALSE(buffer.empty());
    auto const ENCODE_BUFFER_SIZE = buffer.size();

    Box box2;
    std::size_t computed_size = 0;
    ASSERT_EQ(E_SUCCESS, box2.decode(buffer, &computed_size));
    ASSERT_EQ(ENCODE_BUFFER_SIZE, computed_size);
    ASSERT_TRUE(box2.is_device_cpu());
    ASSERT_TRUE(box2.is_si32());
    ASSERT_EQ(2, box2.rank());
    ASSERT_EQ(2, box2.size());
    ASSERT_EQ(1, box2.dim(0));
    ASSERT_EQ(2, box2.dim(1));
    ASSERT_EQ(11, box2.getElementByOffset<si32>(0));
    ASSERT_EQ(22, box2.getElementByOffset<si32>(1));
    ASSERT_EQ(INFO.size(), box2.info_capacity());
    ASSERT_EQ(INFO.size(), box2.info_size());
    ASSERT_EQ(INFO, box2.getInfoString());
}

TEST(Box_Encode_Test, InfoOnly)
{
    std::string const INFO = "{TEMP STRING}";
    Box box;
    box.setInfo(INFO);
    ASSERT_EQ(0, box.rank());
    ASSERT_EQ(0, box.size());
    ASSERT_EQ(INFO.size(), box.info_capacity());
    ASSERT_EQ(INFO.size(), box.info_size());
    ASSERT_EQ(INFO, box.getInfoString());

    Box::Buffer buffer;
    ASSERT_EQ(E_SUCCESS, box.encode(buffer));
    ASSERT_FALSE(buffer.empty());
    auto const ENCODE_BUFFER_SIZE = buffer.size();

    Box box2;
    std::size_t computed_size = 0;
    ASSERT_EQ(E_SUCCESS, box2.decode(buffer, &computed_size));
    ASSERT_EQ(ENCODE_BUFFER_SIZE, computed_size);
    ASSERT_EQ(0, box2.rank());
    ASSERT_EQ(0, box2.size());
    ASSERT_EQ(INFO.size(), box2.info_capacity());
    ASSERT_EQ(INFO.size(), box2.info_size());
    ASSERT_EQ(INFO, box2.getInfoString());
}

TEST(Box_Encode_Test, Json)
{
    Box b0 = {10, 20};
    ASSERT_TRUE(b0.is_device_cpu());
    ASSERT_TRUE(b0.is_si32());
    ASSERT_EQ(1, b0.rank());
    ASSERT_EQ(2, b0.size());
    ASSERT_EQ(2, b0.dim(0));
    ASSERT_EQ(10, b0.at<si32>(0));
    ASSERT_EQ(20, b0.at<si32>(1));

    Err code0;
    auto const JSON = b0.toJsonText(&code0);
    ASSERT_EQ(E_SUCCESS, code0);
    ASSERT_FALSE(JSON.empty());
    std::cout << JSON << std::endl;

    Err code1;
    Box b1;
    ASSERT_TRUE(b1.fromJsonText(JSON, &code1));
    ASSERT_EQ(E_SUCCESS, code1);
    ASSERT_TRUE(b1.is_device_cpu());
    ASSERT_TRUE(b1.is_si32());
    ASSERT_EQ(1, b1.rank());
    ASSERT_EQ(2, b1.size());
    ASSERT_EQ(2, b1.dim(0));
    ASSERT_EQ(10, b1.at<si32>(0));
    ASSERT_EQ(20, b1.at<si32>(1));
}


