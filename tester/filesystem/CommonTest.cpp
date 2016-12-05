/**
 * @file   CommonTest.cpp
 * @brief  Common class tester.
 * @author zer0
 * @date   2016-04-08
 */

#include <gtest/gtest.h>
#include <libtbag/filesystem/Common.hpp>

#include <iostream>
#include <libtbag/debug/UvError.hpp>

using namespace libtbag;
using namespace libtbag::filesystem;

TEST(CommonTest, GetPathSeparator)
{
#if defined(WIN32) || defined(_WIN32)
    ASSERT_EQ('\\', PATH_SEPARATOR);
    ASSERT_EQ(';', PATH_SPLITTER);
#else
    ASSERT_EQ('/', PATH_SEPARATOR);
    ASSERT_EQ(':', PATH_SPLITTER);
#endif
}

TEST(CommonTest, printInfos)
{
    printInfos();
}

