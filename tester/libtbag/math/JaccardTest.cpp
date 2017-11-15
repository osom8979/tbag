/**
 * @file   JaccardTest.cpp
 * @brief  Jaccard class tester.
 * @author zer0
 * @date   2017-11-03
 */

#include <gtest/gtest.h>
#include <libtbag/math/Jaccard.hpp>
#include <libtbag/algorithm/Equals.hpp>

using namespace libtbag;
using namespace libtbag::math;

TEST(JaccardTest, Default)
{
    using namespace libtbag::geometry;

    /*
     *  0 |  10  20  30  40  50
     * ---+--------------------
     *    |
     * 10 |   +-------+
     *    |   |       |
     * 20 |   |   +-------+
     *    |   |   |   |   |
     * 30 |   +---|---+   |
     *    |       |       |
     * 40 |       +-------+
     *    |
     * 50 |
     */
    Rect a(10, 10, 20, 20);
    Rect b(20, 20, 20, 20);
    ASSERT_TRUE(algorithm::equals(0.857143, jaccardDistance(a, b), 0.000001));
    ASSERT_TRUE(algorithm::equals(0.142857, jaccardIndex(a, b), 0.000001));
}

