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

struct IdleTest : Idle
{
    bool is_idle;
    bool is_close;

    IdleTest(Loop & loop) : Idle(loop)
    {
        is_idle = false;
        is_close = false;
    }

    virtual void onIdle() override
    {
        is_idle = true;
        this->stop();
    }

    virtual void onClose() override
    {
        is_close = true;
    }
};

TEST(IdleTest, Default)
{
    std::shared_ptr<IdleTest> idle;
    {
        Loop loop;
        idle.reset(new IdleTest(loop));
        idle->start();
        loop.run();
    }

    ASSERT_TRUE(idle->is_idle);
    ASSERT_TRUE(idle->is_close);
}

