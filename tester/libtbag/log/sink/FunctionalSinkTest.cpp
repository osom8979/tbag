/**
 * @file   FunctionalSinkTest.cpp
 * @brief  FunctionalSink class tester.
 * @author zer0
 * @date   2019-07-03
 */

#include <gtest/gtest.h>
#include <libtbag/log/sink/FunctionalSink.hpp>

using namespace libtbag;
using namespace libtbag::log;
using namespace libtbag::log::sink;

TEST(FunctionalSinkTest, Default)
{
    auto write_cb = [](char const * msg, int size, void * user) -> bool {
        return true;
    };
    auto flush_cb = [](void * user){
    };

    int test = 100;
    FunctionalSink object(write_cb, flush_cb, &test);
}

