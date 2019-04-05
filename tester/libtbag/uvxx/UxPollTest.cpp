/**
 * @file   UxPollTest.cpp
 * @brief  UxPoll class tester.
 * @author zer0
 * @date   2018-12-06
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/uvxx/UxLoop.hpp>
#include <libtbag/uvxx/UxPoll.hpp>
#include <libtbag/uvxx/UxIdle.hpp>

#include <libtbag/lock/UvLock.hpp>
#include <libtbag/lock/UvCondition.hpp>
#include <libtbag/filesystem/File.hpp>

#include <thread>
#include <string>

using namespace libtbag;
using namespace libtbag::uvxx;

TEST(UxPollTest, Default)
{
#if defined(TBAG_PLATFORM_WINDOWS) || defined(TBAG_PLATFORM_LINUX)
    std::cout << "Skip this test in Windows/Linux Platform." << std::endl;
    return;
#endif

    char const * const TEST_FILENAME = "test.file";
    tttDir_Automatic();
    auto const PATH = tttDir_Get() / TEST_FILENAME;

    UxPoll poll;
    ASSERT_FALSE(poll.isInit());

    UxLoop loop;
    ASSERT_TRUE(loop.empty());

    libtbag::filesystem::File f;
    ASSERT_TRUE(f.open(PATH));
    ASSERT_TRUE(f.isOpen());

    ASSERT_EQ(E_SUCCESS, poll.initFile(loop, static_cast<int>(f.getFd())));
    ASSERT_TRUE(poll.isInit());
    ASSERT_FALSE(loop.empty());
    ASSERT_EQ(1, loop.size());

    UxIdle idle;
    ASSERT_EQ(E_SUCCESS, idle.init(loop));
    ASSERT_TRUE(idle.isInit());
    ASSERT_FALSE(loop.empty());
    ASSERT_EQ(2, loop.size());

    libtbag::lock::UvLock lock;
    libtbag::lock::UvCondition cond;
    bool on_idle = false;

    idle.setOnIdle([&](){
        lock.lock();
        on_idle = true;
        cond.broadcast();
        lock.unlock();
        idle.close();
    });
    ASSERT_EQ(E_SUCCESS, idle.start());

    using EventType = UxPoll::EventType;

    int event_counter = 0;
    int close_counter = 0;
    Err poll_status = E_UNKNOWN;

    poll.setOnPoll([&](Err status, EventType events){
        ++event_counter;
        poll_status = status;
        poll.close();
    });
    poll.setOnClose([&](){
        ++close_counter;
    });

    ASSERT_EQ(E_SUCCESS, poll.start());

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
    ASSERT_TRUE(loop.empty());
    ASSERT_EQ(E_SUCCESS, loop_result);
    ASSERT_EQ(1, event_counter);
    ASSERT_EQ(1, close_counter);
    ASSERT_EQ(E_SUCCESS, poll_status);
}

