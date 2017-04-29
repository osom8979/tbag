/**
 * @file   HashCombineTest.cpp
 * @brief  HashCombine class tester.
 * @author zer0
 * @date   2017-04-29
 */

#include <gtest/gtest.h>
#include <libtbag/functional/HashCombine.hpp>

#include <string>
#include <vector>

using namespace libtbag;
using namespace libtbag::functional;

TEST(HashCombineTest, Default)
{
    std::string temp = "TEST";
    std::vector<uint32_t> buffer = {0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF};

    ASSERT_LT(0, hashRange(temp.begin(), temp.end()));
    ASSERT_LT(0, hashRange(buffer.begin(), buffer.end()));
}

