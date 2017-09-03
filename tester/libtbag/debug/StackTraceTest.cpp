/**
 * @file   StackTraceTest.cpp
 * @brief  StackTrace class tester.
 * @author zer0
 * @date   2017-05-28
 */

#include <gtest/gtest.h>
#include <libtbag/debug/st/ExecInfoStackTrace.hpp>
#include <libtbag/debug/st/LibunwindStackTrace.hpp>
#include <libtbag/debug/st/UnwindStackTrace.hpp>
#include <libtbag/debug/st/WindbgStackTrace.hpp>
#include <libtbag/debug/StackTrace.hpp>
#include <iostream>

using namespace libtbag;
using namespace libtbag::debug;

TEST(StackTraceTest, ExecInfo)
{
    if (st::isExecInfoSupport()) {
        ASSERT_FALSE(st::getExecInfoStackTrace().empty());
    } else {
        std::cout << "Skip this test.\n";
    }
}

TEST(StackTraceTest, Libunwind)
{
    if (st::isLibunwindSupport()) {
        ASSERT_FALSE(st::getLibunwindStackTrace().empty());
    } else {
        std::cout << "Skip this test.\n";
    }
}

TEST(StackTraceTest, Unwind)
{
    if (st::isUnwindSupport()) {
        ASSERT_FALSE(st::getUnwindStackTrace().empty());
    } else {
        std::cout << "Skip this test.\n";
    }
}

TEST(StackTraceTest, Windbg)
{
    if (st::isWindbgSupport()) {
        ASSERT_FALSE(st::getWindbgStackTrace().empty());
    } else {
        std::cout << "Skip this test.\n";
    }
}

TEST(StackTraceTest, Default)
{
    std::cout << getStackTrace() << std::endl;
}

