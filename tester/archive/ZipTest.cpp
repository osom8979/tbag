/**
 * @file   ZipTest.cpp
 * @brief  Zip class tester.
 * @author zer0
 * @date   2016-11-17
 */

#include <gtest/gtest.h>
#include <libtbag/archive/Zip.hpp>

using namespace libtbag;
using namespace libtbag::archive;

TEST(ZipTest, Default)
{
    Zip zip;

    //std::string const TEST_BODY = "__tester_archive_ziptest_default__";
    //ASSERT_TRUE(zip.write(""));
    //ASSERT_TRUE(zip.read(""));
}

