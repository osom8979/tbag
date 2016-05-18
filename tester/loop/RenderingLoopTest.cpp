/**
 * @file   RenderingLoopTest.cpp
 * @brief  RenderingLoop class tester.
 * @author zer0
 * @date   2016-04-28
 */

#include <gtest/gtest.h>
#include <libtbag/loop/RenderingLoop.hpp>

using namespace libtbag;
using namespace libtbag::loop;

class RenderingCallback : public RenderingLoop<>::Callback
{
public:
    using Looper = typename RenderingLoop<>::Callback::Looper;

public:
    static constexpr int getTestTime() noexcept {
        return 100;
    }

public:
    std::chrono::system_clock::time_point start_time;
    int sleep_time = 0;

    int  start_count = 0;
    int    end_count = 0;
    int update_count = 0;
    int render_count = 0;

public:
    RenderingCallback(int time) {
        this->sleep_time = time;
    }

    ~RenderingCallback() {
    }

public:
    virtual void onStart(Looper &) override {
        ++start_count;
        start_time = std::chrono::system_clock::now();
    }

    virtual void onEnd(Looper &) override {
        ++end_count;
    }

    virtual bool update(Looper &) override {
        ++update_count;
        if (std::chrono::system_clock::now() - start_time >= std::chrono::milliseconds(getTestTime())) {
            return false;
        }
        return true;
    }

    virtual void render(Looper &) override {
        ++render_count;
        std::this_thread::sleep_for(std::chrono::milliseconds(this->sleep_time));
    }
};

TEST(RenderingLoopTest, CoverageOnly)
{
    RenderingCallback callback(1);
    RenderingLoop<> loop(callback, std::chrono::milliseconds(1));

    loop.getTimeStep();
    loop.setExit();
    loop.exit();
    loop.setResultCode(0);
    loop.getPreStart();
    loop.getNowStart();
    loop.getDuration();
    loop.getTimeLag();
}

TEST(RenderingLoopTest, SlowMachine)
{
    int const FPS = 10;
    int const EPSILON = 1;

    int const MIN_FPS = FPS - EPSILON;
    int const MAX_FPS = FPS + EPSILON;

    int const TIME_STEP = RenderingCallback::getTestTime() / FPS;
    int const SLEEP_TIME = 20;

    RenderingCallback callback(SLEEP_TIME);
    RenderingLoop<> loop(callback, std::chrono::milliseconds(TIME_STEP));
    loop.run();

    ASSERT_EQ(callback.start_count, 1);
    ASSERT_EQ(callback.end_count, 1);

    ASSERT_GE(callback.update_count, MIN_FPS);
    ASSERT_LE(callback.update_count, MAX_FPS);

    ASSERT_LT(callback.render_count, FPS);
}

TEST(RenderingLoopTest, FastMachine)
{
    // TODO: Some time, test fails.

    int const FPS = 10;
    int const EPSILON = 1;

    int const MIN_FPS = FPS - EPSILON;
    int const MAX_FPS = FPS + EPSILON;

    int const TIME_STEP = RenderingCallback::getTestTime() / FPS;
    int const SLEEP_TIME = 1;

    RenderingCallback callback(SLEEP_TIME);
    RenderingLoop<> loop(callback, std::chrono::milliseconds(TIME_STEP));
    loop.run();

    ASSERT_EQ(callback.start_count, 1);
    ASSERT_EQ(callback.end_count, 1);

    ASSERT_GE(callback.update_count, MIN_FPS);
    ASSERT_LE(callback.update_count, MAX_FPS);

    ASSERT_GE(callback.render_count, FPS);
}

