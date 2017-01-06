/**
 * @file   AsyncTest.cpp
 * @brief  Async libuv class tester.
 * @author zer0
 * @date   2017-01-06
 */

#include <gtest/gtest.h>
#include <libtbag/uv/Loop.hpp>
#include <libtbag/uv/Async.hpp>

using namespace libtbag;
using namespace libtbag::uv;

static bool g_is_async = false;
static bool g_is_close = false;

struct AsyncTest : public Async 
{
    AsyncTest(Loop & loop) : Async(loop)
    { /* EMPTY. */ }

    virtual void onAsync() override
    {
        g_is_async = true;
        close();
    }

    virtual void onClose() override
    {
        g_is_close = true;
    }
};

TEST(AsyncTest, Default)
{
    {
        Loop loop;
        auto async = loop.newHandle<AsyncTest>(loop);
        async->send();
        loop.run();
    }

    ASSERT_TRUE(g_is_async);
    ASSERT_TRUE(g_is_close);
}

