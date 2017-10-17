/**
 * @file   ZipTest.cpp
 * @brief  Zip class tester.
 * @author zer0
 * @date   2016-11-17
 */

#include <gtest/gtest.h>
#include <libtbag/archive/Zip.hpp>
#include <libtbag/time/ElapsedTime.hpp>

#include <cstdint>

#include <string>
#include <iostream>
#include <chrono>
#include <random>

using namespace libtbag;
using namespace libtbag::archive;

TEST(ZipTest, Default)
{
    char const * const CONTENT = "__tester_archive_ziptest_default__";
    std::string const TEST_BODY = std::string(CONTENT) + std::string(CONTENT);
    std::string result;

    util::Buffer input;
    input.assign(TEST_BODY.begin(), TEST_BODY.end());

    util::Buffer deflate;
    ASSERT_EQ(Err::E_SUCCESS, encode(&input[0], input.size(), deflate));

    result.assign(deflate.begin(), deflate.end());
    ASSERT_NE(TEST_BODY, result);
    ASSERT_GT(TEST_BODY.size(), result.size());

    util::Buffer inflate;
    ASSERT_EQ(Err::E_SUCCESS, decode(&deflate[0], deflate.size(), inflate));

    result.assign(inflate.begin(), inflate.end());
    ASSERT_EQ(TEST_BODY, result);
}

TEST(ZipTest, ImageTest)
{
    int width    = 300;
    int height   = 300;
    int channels = 3;
    int total    = width * height * channels;

    std::random_device device;
    std::mt19937 engine(device());
    std::uniform_int_distribution<int> distribution(0x00, 0xFF);

    util::Buffer input;
    input.resize(total);
    for (auto & pixel : input) {
        pixel = distribution(engine);
    }

    util::Buffer deflate;
    Err code;

    auto encode_duration = time::getDurationWithPredicated<std::chrono::milliseconds>([&](){
        code = encode(&input[0], input.size(), deflate);
    });
    ASSERT_EQ(Err::E_SUCCESS, code);
    std::cout << "Image(" << width << "x" << height << ") Encode size: " << deflate.size() << " byte, time: " << encode_duration.count() << " milliseconds.\n";

    util::Buffer inflate;
    auto decode_duration = time::getDurationWithPredicated<std::chrono::milliseconds>([&](){
        code = decode(&deflate[0], deflate.size(), inflate);
    });
    ASSERT_EQ(Err::E_SUCCESS, code);
    std::cout << "Image(" << width << "x" << height << ") Decode size: " << inflate.size() << " byte, time: " << decode_duration.count() << " milliseconds.\n";

    ASSERT_EQ(input.size(), inflate.size());
    for (auto itr = input.begin(), itr2 = inflate.begin(); itr != input.end(); ++itr, ++itr2) {
        ASSERT_EQ(*itr, *itr2);
    }
}

TEST(ZipTest, unzip)
{
    //Zip::unzip("tiny.zip", ".");
}

