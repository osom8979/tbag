/**
 * @file   listTest.cpp
 * @brief  list container tester.
 * @author zer0
 * @date   2016-05-23
 */

#include <gtest/gtest.h>
#include <libtbag/container/simple/list.hpp>

#include <vector>
#include <algorithm>

using namespace libtbag;
using namespace libtbag::container;
using namespace libtbag::container::simple;

TEST(listTest, Default)
{
    list::Node<int> * first = nullptr;
    ASSERT_EQ(list::size(first), 0);

    first = list::createNode<int>(0, nullptr);
    ASSERT_EQ(list::size(first), 1);

    list::releaseNode(&first);
    ASSERT_EQ(first, nullptr);
}

