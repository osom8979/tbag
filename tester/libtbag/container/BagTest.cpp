/**
 * @file   BagTest.cpp
 * @brief  Bag class tester.
 * @author zer0
 * @date   2017-12-08
 */

#include <gtest/gtest.h>
#include <libtbag/container/Bag.hpp>

using namespace libtbag;
using namespace libtbag::container;

TEST(BagTest, Coverage)
{
    Bagi bagi;
    Bagf bagf;

    bagi.type_name();
    bagi.type_size();
    bagi.type_index();
    bagi.type_table();
    bagi.type_maximum();
    bagi.type_minimum();

    bagf.type_name();
    bagf.type_size();
    bagf.type_index();
    bagf.type_table();
    bagf.type_maximum();
    bagf.type_minimum();
}

