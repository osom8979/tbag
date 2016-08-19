/**
 * @file   haarTest.cpp
 * @brief  haar structure tester.
 * @author zer0
 * @date   2016-05-24
 */

#include <gtest/gtest.h>
#include <libtbag/science/wavelet/haar.hpp>

using namespace libtbag;
using namespace libtbag::science;
using namespace libtbag::science::wavelet;

using TestType = double;

static TestType INPUT    [] = { 10, 20,  30, 40 };
static TestType RESULT1D [] = { 15, 35,  -5, -5 };
static TestType RESULT2D [] = { 25, -5, -10,  0 };

static std::size_t const    INPUT_LENGTH = sizeof(INPUT) / sizeof(INPUT[0]);
static std::size_t const RESULT1D_LENGTH = sizeof(RESULT1D) / sizeof(RESULT1D[0]);
static std::size_t const RESULT2D_LENGTH = sizeof(RESULT2D) / sizeof(RESULT2D[0]);

TEST(haarTest, haar1d)
{
    std::vector<TestType>  input(INPUT,    INPUT    +    INPUT_LENGTH);
    std::vector<TestType> result(RESULT1D, RESULT1D + RESULT1D_LENGTH);

    ASSERT_EQ(result.size(), input.size());

    // Forward.
    std::vector<TestType> forward;
    forward.resize(input.size());

    haar::forward(input.data(), input.size(), forward.data());
    {
        for (auto itr1 = forward.begin(), end1 = forward.end()
                , itr2 =  result.begin(), end2 =  result.end(); itr1 != end1 && itr2 != end2; ++itr1, ++itr2) {
            ASSERT_EQ(*itr2, *itr1);
        }
    }

    // Inverse.
    std::vector<TestType> inverse;
    inverse.resize(forward.size());

    haar::inverse(forward.data(), forward.size(), inverse.data());
    {
        for (auto itr1 = inverse.begin(), end1 = inverse.end()
                , itr2 =   input.begin(), end2 =   input.end(); itr1 != end1 && itr2 != end2; ++itr1, ++itr2) {
            ASSERT_EQ(*itr2, *itr1);
        }
    }
}

TEST(haarTest, haar2d)
{
    std::vector<TestType>  input(INPUT,    INPUT    +    INPUT_LENGTH);
    std::vector<TestType> result(RESULT2D, RESULT2D + RESULT2D_LENGTH);

    std::size_t const TEST_INPUT_WIDTH  = 2;
    std::size_t const TEST_INPUT_HEIGHT = 2;
    std::size_t const TEST_INPUT_SIZE   = TEST_INPUT_WIDTH * TEST_INPUT_HEIGHT;

    ASSERT_EQ(  result.size(), input.size());
    ASSERT_EQ(TEST_INPUT_SIZE, input.size());

    // Forward.
    std::vector<TestType> forward;
    forward.resize(input.size());

    haar::forward2d(input.data(), TEST_INPUT_WIDTH, TEST_INPUT_HEIGHT, forward.data());
    {
        for (auto itr1 = forward.begin(), end1 = forward.end()
                , itr2 =  result.begin(), end2 =  result.end(); itr1 != end1 && itr2 != end2; ++itr1, ++itr2) {
            ASSERT_EQ(*itr2, *itr1);
        }
    }

    // Inverse.
    std::vector<TestType> inverse;
    inverse.resize(forward.size());

    haar::inverse2d(forward.data(), TEST_INPUT_WIDTH, TEST_INPUT_HEIGHT, inverse.data());
    {
        for (auto itr1 = inverse.begin(), end1 = inverse.end()
                , itr2 =   input.begin(), end2 =   input.end(); itr1 != end1 && itr2 != end2; ++itr1, ++itr2) {
            ASSERT_EQ(*itr2, *itr1);
        }
    }
}

