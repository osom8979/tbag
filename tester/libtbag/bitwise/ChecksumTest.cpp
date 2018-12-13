/**
 * @file   ChecksumTest.cpp
 * @brief  Checksum class tester.
 * @author zer0
 * @date   2018-12-13
 */

#include <gtest/gtest.h>
#include <libtbag/bitwise/Checksum.hpp>

#include <vector>

using namespace libtbag;
using namespace libtbag::bitwise;

TEST(ChecksumTest, CalcXorChecksum1)
{
    std::vector<uint8_t> data;
    data.push_back(0xFF);
    data.push_back(0x0F);
    ASSERT_EQ(0xF0, calcXorChecksum(data.data(), data.size()));
}

TEST(ChecksumTest, CalcXorChecksum2)
{
    std::vector<uint8_t> data;
    data.push_back(0x01);
    data.push_back(0x02);
    data.push_back(0x04);
    data.push_back(0x08);
    data.push_back(0x10);
    data.push_back(0x20);
    data.push_back(0x40);
    data.push_back(0x80);
    ASSERT_EQ(0xFF, calcXorChecksum(data.data(), data.size()));
}

