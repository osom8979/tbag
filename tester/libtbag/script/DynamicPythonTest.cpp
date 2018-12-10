/**
 * @file   DynamicPythonTest.cpp
 * @brief  DynamicPython class tester.
 * @author zer0
 * @date   2018-10-31
 */

#include <gtest/gtest.h>
#include <libtbag/script/DynamicPython.hpp>

using namespace libtbag;
using namespace libtbag::script;

TEST(DynamicPythonTest, FindLibrary)
{
}

TEST(DynamicPythonTest, Default)
{
    DynamicPython object;
    ASSERT_TRUE(true);
}

