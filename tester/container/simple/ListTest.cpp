/**
 * @file   ListTest.cpp
 * @brief  List container tester.
 * @author zer0
 * @date   2016-05-23
 */

#include <gtest/gtest.h>
#include <libtbag/container/simple/List.hpp>

#include <vector>
#include <algorithm>

using namespace libtbag;
using namespace libtbag::container;
using namespace libtbag::container::simple;

TEST(ListTest, Default)
{
    list::Node<int> * first = nullptr;
    ASSERT_EQ(0U, list::size(first));

    first = list::createNode<int>(0, nullptr);
    ASSERT_EQ(1U, list::size(first));

    list::releaseNode(&first);
    ASSERT_EQ(nullptr, first);
}

