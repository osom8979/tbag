/**
 * @file   IdleTest.cpp
 * @brief  Idle libuv class tester.
 * @author zer0
 * @date   2016-12-28
 */

#include <gtest/gtest.h>
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/uvpp/Idle.hpp>

#include <cstdio>
#include <memory>

using namespace libtbag;
using namespace libtbag::uvpp;

static bool g_is_idle;
static bool g_is_close;

TBAG_CONSTEXPR static bool const USE_DEBUGGING = true;

struct IdleTest : public Idle
{
    IdleTest(Loop & loop) : Idle(loop)
    {
        g_is_idle = false;
        g_is_close = false;
    }

    virtual void onIdle() override
    {
        g_is_idle = true;
        stop();
        //close(); // Don't use this method in tester.
    }

    virtual void onClose() override
    {
        g_is_close = true;
    }
};

TEST(IdleTest, Default)
{
    {
        Loop loop;
        auto shared = loop.newHandle<IdleTest>(loop);
        static_cast<IdleTest*>(shared.get())->start();
        loop.run();

        if (USE_DEBUGGING) { // ONLY DEBUGGING.
            shared->close();
            loop.run();
            loop.printAllHandles(stdout);
        }
    }

    ASSERT_TRUE(g_is_idle);
    ASSERT_TRUE(g_is_close);
}

