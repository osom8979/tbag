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
#include <vector>
#include <random>

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

TEST(ZipTest, ImageTest)
{
    int width    = 300;
    int height   = 300;
    int channels = 3;
    int total    = width * height * channels;

    std::random_device device;
    std::mt19937 engine(device());
    std::uniform_int_distribution<uint8_t> distribution(0x00, 0xFF);

    std::vector<uint8_t> input;
    input.resize(total);
    for (auto & pixel : input) {
        pixel = distribution(engine);
    }

    std::vector<uint8_t> deflate;
    Zip::ResultCode result;

    auto encode_duration = time::getDurationWithPredicated<std::chrono::milliseconds>([&](){
        result = Zip::encode(deflate, &input[0], input.size());
    });
    ASSERT_EQ(Zip::ResultCode::SUCCESS, result);
    std::cout << "Image(" << width << "x" << height << ") Encode size: " << deflate.size() << " byte, time: " << encode_duration.count() << " milliseconds.\n";

    std::vector<uint8_t> inflate;
    auto decode_duration = time::getDurationWithPredicated<std::chrono::milliseconds>([&](){
        result = Zip::decode(inflate, &deflate[0], deflate.size());
    });
    ASSERT_EQ(Zip::ResultCode::SUCCESS, result);
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

