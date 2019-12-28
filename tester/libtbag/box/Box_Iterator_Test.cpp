/**
 * @file   Box_Iterator_Test.cpp
 * @brief  Box class tester.
 * @author zer0
 * @date   2019-12-27
 */

#include <gtest/gtest.h>
#include <libtbag/box/Box.hpp>
#include <vector>

using namespace libtbag;
using namespace libtbag::box;

TEST(Box_Iterator_Test, Rank1)
{
    Box box = {1, 2, 3};

    auto cursor_code = box.cursor();
    ASSERT_EQ(E_SUCCESS, cursor_code.code);

    std::vector<int> result;
    for (auto & data : cursor_code.value.itr<int>()) {
        result.emplace_back(data);
    }

    ASSERT_EQ(3, result.size());
    ASSERT_EQ(1, result[0]);
    ASSERT_EQ(2, result[1]);
    ASSERT_EQ(3, result[2]);
}

TEST(Box_Iterator_Test, Rank2)
{
    Box box = {{1, 2}, {3, 4}};

    auto cursor_code0 = box.cursor();
    ASSERT_EQ(E_SUCCESS, cursor_code0.code);

    std::vector<int> result0;
    for (auto const & i : cursor_code0.value.itr<int>()) {
        result0.emplace_back(i);
    }
    ASSERT_EQ(2, result0.size());
    ASSERT_EQ(1, result0[0]);
    ASSERT_EQ(3, result0[1]);

    std::vector<int> result1;
    do {
        auto cursor_code1 = cursor_code0.value.sub();
        ASSERT_EQ(E_SUCCESS, cursor_code1.code);
        for (auto const & j : cursor_code1.value.itr<int>()) {
            result1.emplace_back(j);
        }
    } while (cursor_code0.value.next());

    ASSERT_EQ(4, result1.size());
    ASSERT_EQ(1, result1[0]);
    ASSERT_EQ(2, result1[1]);
    ASSERT_EQ(3, result1[2]);
    ASSERT_EQ(4, result1[3]);
}

