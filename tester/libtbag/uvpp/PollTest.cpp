/**
 * @file   PollTest.cpp
 * @brief  Poll class tester.
 * @author zer0
 * @date   2017-05-02
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/uvpp/Poll.hpp>
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/uvpp/func/FunctionalIdle.hpp>

#include <libtbag/lock/UvLock.hpp>
#include <libtbag/lock/UvCondition.hpp>
#include <libtbag/filesystem/File.hpp>

#include <thread>
#include <iostream>

using namespace libtbag;
using namespace libtbag::uvpp;

struct PollTest : public Poll
{
    int counter;

    PollTest(Loop & loop, int fd) : Poll(loop, init_file(fd)), counter(0)
    { /* EMPTY. */ }

    virtual void onPoll(Err status, EventType events) override
    {
        ++counter;
        stop();
        close();
    }
};

TEST(PollTest, Default)
{
#if defined(TBAG_PLATFORM_WINDOWS) || defined(TBAG_PLATFORM_LINUX)
    std::cout << "Skip this test in Windows/Linux Platform." << std::endl;
    return;
#endif

    char const * const TEST_FILENAME = "test.file";
    tttDir_Automatic();
    auto path = tttDir_Get() / TEST_FILENAME;

    filesystem::File f;
    ASSERT_EQ(E_SUCCESS, f.open(path));
    ASSERT_TRUE(f.isOpen());

    Loop loop;
    auto fs = loop.newHandle<PollTest>(loop, static_cast<int>(f.getFd()));

    libtbag::lock::UvLock lock;
    libtbag::lock::UvCondition cond;
    bool on_idle = false;

    auto idle = loop.newHandle<libtbag::uvpp::func::FuncIdle>(loop);
    idle->idle_cb = [&](){
        lock.lock();
        on_idle = true;
        cond.broadcast();
        lock.unlock();
        idle->close();
    };
    ASSERT_EQ(E_SUCCESS, idle->start());
    ASSERT_EQ(E_SUCCESS, fs->start());

    Err loop_result = E_UNKNOWN;
    auto thread = std::thread([&](){
        loop_result = loop.run();
    });

    lock.lock();
    while (!on_idle) {
        cond.wait(lock);
    }
    lock.unlock();

    int const WRITE_SIZE = 4;
    ASSERT_EQ(WRITE_SIZE, f.write("TEMP", WRITE_SIZE, 0));
    ASSERT_TRUE(f.close());

    thread.join();
    ASSERT_EQ(E_SUCCESS, loop_result);
    ASSERT_EQ(1, fs->counter);
}

