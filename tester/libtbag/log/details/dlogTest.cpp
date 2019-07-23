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

TEST(dlogTest, Default)
{
    _dp("profile");
    _dd("debug");
    _di("info");
    _dw("warning");
    _de("error");
}

