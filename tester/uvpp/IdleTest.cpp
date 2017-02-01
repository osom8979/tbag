/**
 * @file   IdleTest.cpp
 * @brief  Idle libuv class tester.
 * @author zer0
 * @date   2016-12-28
 */

#include <gtest/gtest.h>
#include <libtbag/uv/Loop.hpp>
#include <libtbag/uv/Idle.hpp>

#include <cstdio>
#include <memory>

using namespace libtbag;
using namespace libtbag::uv;

static bool g_is_idle;
static bool g_is_close;

TBAG_CONSTEXPR static bool const USE_DEBUGGING = false;

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
        auto weak = loop.insertChildHandle(new IdleTest(loop));
        static_cast<IdleTest*>(weak.lock().get())->start();
        loop.run();

        if (USE_DEBUGGING) { // ONLY DEBUGGING.
            weak.lock()->close();
            loop.run();
            loop.printAllHandles(stdout);
        }
    }

    ASSERT_TRUE(g_is_idle);
    ASSERT_TRUE(g_is_close);
}

