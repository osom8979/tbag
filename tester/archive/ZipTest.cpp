/**
 * @file   ZipTest.cpp
 * @brief  Zip class tester.
 * @author zer0
 * @date   2016-11-17
 */

#include <gtest/gtest.h>
#include <libtbag/archive/Zip.hpp>

#include <cstdint>
#include <string>
#include <vector>

using namespace libtbag;
using namespace libtbag::archive;

TEST(ZipTest, Default)
{
    Zip zip;

    char const * const CONTENT = "__tester_archive_ziptest_default__";
    std::string const TEST_BODY = std::string(CONTENT) + std::string(CONTENT);
    std::string result;

    std::vector<uint8_t> input;
    input.assign(TEST_BODY.begin(), TEST_BODY.end());

    std::vector<uint8_t> deflate;
    ASSERT_EQ(Zip::ResultCode::SUCCESS, Zip::encode(deflate, &input[0], input.size()));

    result.assign(deflate.begin(), deflate.end());
    ASSERT_NE(TEST_BODY, result);
    ASSERT_GT(TEST_BODY.size(), result.size());

    std::vector<uint8_t> inflate;
    ASSERT_EQ(Zip::ResultCode::SUCCESS, Zip::decode(inflate, &deflate[0], deflate.size()));

    result.assign(inflate.begin(), inflate.end());
    ASSERT_EQ(TEST_BODY, result);
}

TEST(ZipTest, unzip)
{
    Zip::unzip("tiny.zip", "");
}

