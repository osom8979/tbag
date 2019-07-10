/**
 * @file   BoxTest.cpp
 * @brief  Box class tester.
 * @author zer0
 * @date   2019-05-16
 */

#include <gtest/gtest.h>
#include <libtbag/box/Box.hpp>
#include <libtbag/util/TestUtils.hpp>

#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

using namespace libtbag;
using namespace libtbag::box;

TEST(BoxTest, Default)
{
    Box b0;
    ASSERT_TRUE(b0);

    Box b1_move;
    ASSERT_TRUE(b1_move);
    auto const * b1_data = b1_move.get();
    Box b2 = std::move(b1_move);
    ASSERT_TRUE(b2);
    auto const * b2_data = b2.get();
    ASSERT_EQ(b1_data, b2_data);

    Box b3;
    Box b4 = b3;
    ASSERT_TRUE(b3);
    ASSERT_TRUE(b4);
    auto const * b3_data = b3.get();
    auto const * b4_data = b4.get();
    ASSERT_EQ(b3_data, b4_data);

    Box b5;
    ASSERT_TRUE(b5);
    Box b6(nullptr);
    ASSERT_FALSE(b6);
    b6 = b5;
    auto const * b5_data = b5.get();
    auto const * b6_data = b6.get();
    ASSERT_EQ(b5_data, b6_data);
}

TEST(BoxTest, StlContainerCompatibility)
{
    Box b1;
    Box b2;
    b1.setInfo("1");
    b2.setInfo("2");

    TBAG_SCENARIO("std::vector") {
        std::vector<Box> container;
        container.push_back(b1);
        container.push_back(b2);

        ASSERT_EQ(2, container.size());
        ASSERT_STREQ("1", container[0].getInfoString().c_str());
        ASSERT_STREQ("2", container[1].getInfoString().c_str());

        auto container2 = container;
        ASSERT_STREQ("1", container2[0].getInfoString().c_str());
        ASSERT_STREQ("2", container2[1].getInfoString().c_str());

        ASSERT_EQ(container[0].get(), container2[0].get());
        ASSERT_EQ(container[1].get(), container2[1].get());
    }

    TBAG_SCENARIO("std::set") {
        std::set<Box> container;
        ASSERT_TRUE(container.insert(b1).second);
        ASSERT_TRUE(container.insert(b2).second);
        ASSERT_FALSE(container.insert(b1).second);
        ASSERT_FALSE(container.insert(b2).second);

        ASSERT_EQ(2, container.size());
        ASSERT_STREQ("1", container.find(b1)->getInfoString().c_str());
        ASSERT_STREQ("2", container.find(b2)->getInfoString().c_str());

        auto container2 = container;
        ASSERT_STREQ("1", container2.find(b1)->getInfoString().c_str());
        ASSERT_STREQ("2", container2.find(b2)->getInfoString().c_str());
    }

    TBAG_SCENARIO("std::map") {
        std::map<int, Box> container;
        ASSERT_TRUE(container.insert(std::make_pair(1, b1)).second);
        ASSERT_TRUE(container.insert(std::make_pair(2, b2)).second);
        ASSERT_TRUE(container.insert(std::make_pair(3, b1)).second);
        ASSERT_TRUE(container.insert(std::make_pair(4, b2)).second);

        ASSERT_EQ(4, container.size());
        ASSERT_STREQ("1", container.find(1)->second.getInfoString().c_str());
        ASSERT_STREQ("2", container.find(2)->second.getInfoString().c_str());
        ASSERT_STREQ("1", container.find(3)->second.getInfoString().c_str());
        ASSERT_STREQ("2", container.find(4)->second.getInfoString().c_str());

        auto container2 = container;
        ASSERT_STREQ("1", container2.find(1)->second.getInfoString().c_str());
        ASSERT_STREQ("2", container2.find(2)->second.getInfoString().c_str());
        ASSERT_STREQ("1", container2.find(3)->second.getInfoString().c_str());
        ASSERT_STREQ("2", container2.find(4)->second.getInfoString().c_str());

        ASSERT_EQ(container.find(1)->second.get(), container2.find(1)->second.get());
        ASSERT_EQ(container.find(2)->second.get(), container2.find(2)->second.get());
        ASSERT_EQ(container.find(3)->second.get(), container2.find(3)->second.get());
        ASSERT_EQ(container.find(4)->second.get(), container2.find(4)->second.get());
    }

    TBAG_SCENARIO("std::unordered_set") {
        std::unordered_set<Box> container;
        ASSERT_TRUE(container.insert(b1).second);
        ASSERT_TRUE(container.insert(b2).second);
        ASSERT_FALSE(container.insert(b1).second);
        ASSERT_FALSE(container.insert(b2).second);

        ASSERT_EQ(2, container.size());
        ASSERT_STREQ("1", container.find(b1)->getInfoString().c_str());
        ASSERT_STREQ("2", container.find(b2)->getInfoString().c_str());

        auto container2 = container;
        ASSERT_STREQ("1", container2.find(b1)->getInfoString().c_str());
        ASSERT_STREQ("2", container2.find(b2)->getInfoString().c_str());
    }

    TBAG_SCENARIO("std::unordered_map") {
        std::unordered_map<int, Box> container;
        ASSERT_TRUE(container.insert(std::make_pair(1, b1)).second);
        ASSERT_TRUE(container.insert(std::make_pair(2, b2)).second);
        ASSERT_TRUE(container.insert(std::make_pair(3, b1)).second);
        ASSERT_TRUE(container.insert(std::make_pair(4, b2)).second);

        ASSERT_EQ(4, container.size());
        ASSERT_STREQ("1", container.find(1)->second.getInfoString().c_str());
        ASSERT_STREQ("2", container.find(2)->second.getInfoString().c_str());
        ASSERT_STREQ("1", container.find(3)->second.getInfoString().c_str());
        ASSERT_STREQ("2", container.find(4)->second.getInfoString().c_str());

        auto container2 = container;
        ASSERT_STREQ("1", container2.find(1)->second.getInfoString().c_str());
        ASSERT_STREQ("2", container2.find(2)->second.getInfoString().c_str());
        ASSERT_STREQ("1", container2.find(3)->second.getInfoString().c_str());
        ASSERT_STREQ("2", container2.find(4)->second.getInfoString().c_str());

        ASSERT_EQ(container.find(1)->second.get(), container2.find(1)->second.get());
        ASSERT_EQ(container.find(2)->second.get(), container2.find(2)->second.get());
        ASSERT_EQ(container.find(3)->second.get(), container2.find(3)->second.get());
        ASSERT_EQ(container.find(4)->second.get(), container2.find(4)->second.get());
    }
}

TEST(BoxTest, Reshape)
{
    auto b0 = Box::shape<int>(2, 3);
    ASSERT_TRUE(b0.is_device_cpu());
    ASSERT_TRUE(b0.is_si32());
    ASSERT_EQ(2, b0.rank());
    ASSERT_EQ(2*3, b0.size());
    ASSERT_EQ(2, b0.dim(0));
    ASSERT_EQ(3, b0.dim(1));

    b0.reshape<double>(2);
    ASSERT_TRUE(b0.is_device_cpu());
    ASSERT_TRUE(b0.is_fp64());
    ASSERT_EQ(1, b0.rank());
    ASSERT_EQ(2, b0.size());
    ASSERT_EQ(2, b0.dim(0));
}

TEST(BoxTest, Assign)
{
    Box b0 = {10, 20};
    ASSERT_TRUE(b0.is_device_cpu());
    ASSERT_TRUE(b0.is_si32());
    ASSERT_EQ(1, b0.rank());
    ASSERT_EQ(2, b0.size());
    ASSERT_EQ(2, b0.dim(0));
    ASSERT_EQ(10, b0.at<si32>(0));
    ASSERT_EQ(20, b0.at<si32>(1));

    Box b1 = {{10.0f, 20.0f}, {30.0f, 40.0f}, {50.0f, 60.0f}};
    ASSERT_TRUE(b1.is_device_cpu());
    ASSERT_TRUE(b1.is_fp32());
    ASSERT_EQ(2, b1.rank());
    ASSERT_EQ(6, b1.size());
    ASSERT_EQ(3, b1.dim(0));
    ASSERT_EQ(2, b1.dim(1));
    ASSERT_EQ(10.0f, b1.at<fp32>(0, 0));
    ASSERT_EQ(20.0f, b1.at<fp32>(0, 1));
    ASSERT_EQ(30.0f, b1.at<fp32>(1, 0));
    ASSERT_EQ(40.0f, b1.at<fp32>(1, 1));
    ASSERT_EQ(50.0f, b1.at<fp32>(2, 0));
    ASSERT_EQ(60.0f, b1.at<fp32>(2, 1));

    Box b2 = {{{10.0},{20.0}},{{30.0},{40.0}},{{50.0},{60.0}}};
    ASSERT_TRUE(b2.is_device_cpu());
    ASSERT_TRUE(b2.is_fp64());
    ASSERT_EQ(3, b2.rank());
    ASSERT_EQ(6, b2.size());
    ASSERT_EQ(3, b2.dim(0));
    ASSERT_EQ(2, b2.dim(1));
    ASSERT_EQ(1, b2.dim(2));
    ASSERT_EQ(10.0f, b2.at<fp64>(0, 0, 0));
    ASSERT_EQ(20.0f, b2.at<fp64>(0, 1, 0));
    ASSERT_EQ(30.0f, b2.at<fp64>(1, 0, 0));
    ASSERT_EQ(40.0f, b2.at<fp64>(1, 1, 0));
    ASSERT_EQ(50.0f, b2.at<fp64>(2, 0, 0));
    ASSERT_EQ(60.0f, b2.at<fp64>(2, 1, 0));

    Box b3 = {{{{10},{20}},{{30},{40}},{{50},{60}}},
              {{{11},{21}},{{31},{41}},{{51},{61}}}};
    ASSERT_TRUE(b3.is_device_cpu());
    ASSERT_TRUE(b3.is_si32());
    ASSERT_EQ(4, b3.rank());
    ASSERT_EQ(12, b3.size());
    ASSERT_EQ(2, b3.dim(0));
    ASSERT_EQ(3, b3.dim(1));
    ASSERT_EQ(2, b3.dim(2));
    ASSERT_EQ(1, b3.dim(3));
    ASSERT_EQ(10, b3.at<si32>(0, 0, 0, 0));
    ASSERT_EQ(20, b3.at<si32>(0, 0, 1, 0));
    ASSERT_EQ(30, b3.at<si32>(0, 1, 0, 0));
    ASSERT_EQ(40, b3.at<si32>(0, 1, 1, 0));
    ASSERT_EQ(50, b3.at<si32>(0, 2, 0, 0));
    ASSERT_EQ(60, b3.at<si32>(0, 2, 1, 0));
    ASSERT_EQ(11, b3.at<si32>(1, 0, 0, 0));
    ASSERT_EQ(21, b3.at<si32>(1, 0, 1, 0));
    ASSERT_EQ(31, b3.at<si32>(1, 1, 0, 0));
    ASSERT_EQ(41, b3.at<si32>(1, 1, 1, 0));
    ASSERT_EQ(51, b3.at<si32>(1, 2, 0, 0));
    ASSERT_EQ(61, b3.at<si32>(1, 2, 1, 0));

    Box b4 = {{10}};
    ASSERT_TRUE(b4.is_device_cpu());
    ASSERT_TRUE(b4.is_si32());
    ASSERT_EQ(2, b4.rank());
    ASSERT_EQ(1, b4.size());
    ASSERT_EQ(1, b4.dim(0));
    ASSERT_EQ(1, b4.dim(1));
}

TEST(BoxTest, AsType)
{
    Box b0 = {10, 20};
    Box b1 = b0.asType<double>();

    ASSERT_TRUE(b0.is_device_cpu());
    ASSERT_TRUE(b0.is_si32());
    ASSERT_EQ(1, b0.rank());
    ASSERT_EQ(2, b0.size());
    ASSERT_EQ(2, b0.dim(0));
    ASSERT_EQ(10, b0.at<si32>(0));
    ASSERT_EQ(20, b0.at<si32>(1));

    ASSERT_TRUE(b1.is_device_cpu());
    ASSERT_TRUE(b1.is_fp64());
    ASSERT_EQ(1, b1.rank());
    ASSERT_EQ(2, b1.size());
    ASSERT_EQ(2, b1.dim(0));
    ASSERT_EQ(10.0, b1.at<fp64>(0));
    ASSERT_EQ(20.0, b1.at<fp64>(1));
}

TEST(BoxTest, Info)
{
    std::string const INFO = "TEST";
    Box box = {{0}};
    ASSERT_EQ(E_SUCCESS, box.setInfo(INFO));
    ASSERT_EQ(INFO.size(), box.info_capacity());
    ASSERT_EQ(INFO.size(), box.info_size());
    ASSERT_EQ(E_SUCCESS, box.setInfo(INFO));
    ASSERT_EQ(INFO, box.getInfoString());
}

TEST(BoxTest, EncodeDecode)
{
    std::string const INFO = "{TEMP STRING}";
    Box box = {{11, 22}};
    ASSERT_EQ(E_SUCCESS, box.setInfo(INFO));
    ASSERT_TRUE(box.is_device_cpu());
    ASSERT_TRUE(box.is_si32());
    ASSERT_EQ(2, box.rank());
    ASSERT_EQ(2, box.size());
    ASSERT_EQ(1, box.dim(0));
    ASSERT_EQ(2, box.dim(1));
    ASSERT_EQ(11, box.offset<si32>(0));
    ASSERT_EQ(22, box.offset<si32>(1));
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
    ASSERT_EQ(11, box2.offset<si32>(0));
    ASSERT_EQ(22, box2.offset<si32>(1));
    ASSERT_EQ(INFO.size(), box2.info_capacity());
    ASSERT_EQ(INFO.size(), box2.info_size());
    ASSERT_EQ(INFO, box2.getInfoString());
}

TEST(BoxTest, EncodeDecode_ONLYINFO)
{
    std::string const INFO = "{TEMP STRING}";
    Box box;
    ASSERT_EQ(E_SUCCESS, box.setInfo(INFO));
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

TEST(BoxTest, EncodeDecode_Json)
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

