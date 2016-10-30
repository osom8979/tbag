/**
 * @file   LzmaTest.cpp
 * @brief  Lzma class tester.
 * @author zer0
 * @date   2016-10-30
 */

#include <gtest/gtest.h>
#include <libtbag/archive/Lzma.hpp>

using namespace libtbag;
using namespace libtbag::archive;

TEST(LzmaTest, Default)
{
    Lzma lzma;

    //std::string const TEST_BODY = "__TESTER_ARCHIVE_LZMATEST__";
    //ASSERT_TRUE(lzma.write(""));
    //ASSERT_TRUE(lzma.read(""));
}

