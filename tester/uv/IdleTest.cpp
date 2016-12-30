/**
 * @file   IdleTest.cpp
 * @brief  Idle libuv class tester.
 * @author zer0
 * @date   2016-12-28
 */

#include <gtest/gtest.h>
#include <libtbag/uv/Loop.hpp>
#include <libtbag/uv/Idle.hpp>

#include <memory>

using namespace libtbag;
using namespace libtbag::uv;

static bool g_is_idle;
static bool g_is_close;

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
        this->stop();
        this->close();
    }

    virtual void onClose() override
    {
        g_is_close = true;
    }
};

TEST(IdleTest, Default)
{
    Loop loop;
    {
        IdleTest idle(loop);
        idle.start();
        loop.run();
    }

    ASSERT_TRUE(g_is_idle);
    ASSERT_TRUE(g_is_close);
}

