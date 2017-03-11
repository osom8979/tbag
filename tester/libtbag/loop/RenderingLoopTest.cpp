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

static int const _FPS           = 10;
static int const _TEST_MILLISEC = 100;
static int const _TIME_STEP     = _TEST_MILLISEC / _FPS;

static int const _SLOW_SLEEP_TIME_MILLISEC = 20;
static int const _FAST_SLEEP_TIME_MILLISEC =  1;

static int const _EPSILON = (_SLOW_SLEEP_TIME_MILLISEC / _TIME_STEP) + 1;
static int const _MIN_FPS = _FPS - _EPSILON;
static int const _MAX_FPS = _FPS + _EPSILON;


class RenderingCallback : public RenderingLoop<>::Callback
{
public:
    using Looper = RenderingLoop<>::Callback::Looper;

public:
    std::chrono::system_clock::time_point start_time;
    std::chrono::nanoseconds test_time;

    int sleep_time = 0;

    int  start_count = 0;
    int    end_count = 0;
    int update_count = 0;
    int render_count = 0;

public:
    RenderingCallback(int time)
    {
        using namespace std::chrono;
        this->sleep_time = time;
        this->test_time  = duration_cast<nanoseconds>(milliseconds(_TEST_MILLISEC));
    }

    ~RenderingCallback()
    {
        // EMPTY.
    }

public:
    virtual void onStart(Looper &) override
    {
        start_time = std::chrono::system_clock::now();
        ++start_count;
    }

    virtual void onEnd(Looper &) override
    {
        ++end_count;
    }

    virtual void update(Looper &) override
    {
        ++update_count;
    }

    virtual void render(Looper &) override
    {
        ++render_count;
        std::this_thread::sleep_for(std::chrono::milliseconds(this->sleep_time));
    }

    virtual bool isExit(Looper &) override
    {
        using namespace std::chrono;
        if (system_clock::now() - start_time >= test_time) {
            return true;
        }
        return false;
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
    RenderingCallback callback(_SLOW_SLEEP_TIME_MILLISEC);
    RenderingLoop<> loop(callback, std::chrono::milliseconds(_TIME_STEP));
    loop.run();

    ASSERT_EQ(       1, callback.start_count);
    ASSERT_EQ(       1, callback.end_count);
    ASSERT_LE(_MIN_FPS, callback.update_count);
    ASSERT_GE(_MAX_FPS, callback.update_count);
    ASSERT_GE(    _FPS, callback.render_count);
}

TEST(RenderingLoopTest, FastMachine)
{
    RenderingCallback callback(_FAST_SLEEP_TIME_MILLISEC);
    RenderingLoop<> loop(callback, std::chrono::milliseconds(_TIME_STEP));
    loop.run();

    ASSERT_EQ(       1, callback.start_count);
    ASSERT_EQ(       1, callback.end_count);
    ASSERT_LE(_MIN_FPS, callback.update_count);
    ASSERT_GE(_MAX_FPS, callback.update_count);
    ASSERT_GE(    _FPS, callback.render_count);
}

