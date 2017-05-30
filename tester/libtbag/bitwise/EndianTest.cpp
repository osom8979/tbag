/**
 * @file   EndianTest.cpp
 * @brief  Endian class tester.
 * @author zer0
 * @date   2017-05-30
 */

#include <gtest/gtest.h>
#include <libtbag/bitwise/Endian.hpp>

using namespace libtbag;
using namespace libtbag::bitwise;

TEST(EndianTest, Default)
{
     int16_t const HOST_S16 = 10;
    uint16_t const HOST_U16 = 11;
     int32_t const HOST_S32 = 12;
    uint32_t const HOST_U32 = 13;
     int64_t const HOST_S64 = 14;
    uint64_t const HOST_U64 = 15;

    ASSERT_EQ(HOST_S16, toHost(toNetwork(HOST_S16)));
    ASSERT_EQ(HOST_U16, toHost(toNetwork(HOST_U16)));
    ASSERT_EQ(HOST_S32, toHost(toNetwork(HOST_S32)));
    ASSERT_EQ(HOST_U32, toHost(toNetwork(HOST_U32)));
    //ASSERT_EQ(HOST_S64, toHost(toNetwork(HOST_S64)));
    //ASSERT_EQ(HOST_U64, toHost(toNetwork(HOST_U64)));
}

