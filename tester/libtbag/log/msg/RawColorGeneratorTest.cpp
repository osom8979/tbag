/**
 * @file   RawColorGeneratorTest.cpp
 * @brief  RawColorGenerator class tester.
 * @author zer0
 * @date   2020-05-09
 */

#include <gtest/gtest.h>
#include <libtbag/log/msg/RawColorGenerator.hpp>

using namespace libtbag;
using namespace libtbag::log;
using namespace libtbag::log::msg;

TEST(RawColorGeneratorTest, Default)
{
    RawColorGenerator gen(RawColorGenerator::LineFeedStyle::LFS_NONE);
    std::string msg;
    gen.make("A", 8, "D", "abcdefg", 4, msg);
    std::cout << "Generated message: " << msg << std::endl;
}

