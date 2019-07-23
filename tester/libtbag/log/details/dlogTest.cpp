/**
 * @file   dlogTest.cpp
 * @brief  dlog class tester.
 * @author zer0
 * @date   2019-07-23
 */

#include <gtest/gtest.h>
#include <libtbag/log/details/dlog.hpp>

using namespace libtbag;
using namespace libtbag::log;
using namespace libtbag::log::details;

TEST(dlogTest, OneArg)
{
    _tp("profile");
    _td("debug");
    _ti("info");
    _tw("warning");
    _te("error");
}

TEST(dlogTest, TwoArg)
{
    _tp("profile {}", 1);
    _td("debug {}", 1);
    _ti("info {}", 1);
    _tw("warning {}", 1);
    _te("error {}", 1);
}

TEST(dlogTest, ThreeArg)
{
    _tp("profile {} {}", 1, 2);
    _td("debug {} {}", 1, 2);
    _ti("info {} {}", 1, 2);
    _tw("warning {} {}", 1, 2);
    _te("error {} {}", 1, 2);
}

