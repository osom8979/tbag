/**
 * @file   UxFsEventTest.cpp
 * @brief  UxFsEvent class tester.
 * @author zer0
 * @date   2018-12-06
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/uvxx/UxLoop.hpp>
#include <libtbag/uvxx/UxFsEvent.hpp>
#include <libtbag/uvxx/UxIdle.hpp>
#include <libtbag/uvxx/UxCheck.hpp>

#include <libtbag/lock/UvLock.hpp>
#include <libtbag/lock/UvCondition.hpp>
#include <libtbag/filesystem/File.hpp>

#include <thread>
#include <string>

using namespace libtbag;
using namespace libtbag::uvxx;

TEST(UxFsEventTest, Default)
{
    if (isMacPlatform()) {
        std::cout << "Skip this test in macOS platform." << std::endl;
        return;
    }

    char const * const TEST_FILENAME = "test.file";
    tttDir_Automatic();
    auto const PATH = tttDir_Get() / TEST_FILENAME;

    UxFsEvent fs;
    ASSERT_FALSE(fs.isInit());

    UxLoop loop;
    ASSERT_TRUE(loop.empty());

    ASSERT_EQ(E_SUCCESS, fs.init(loop));
    ASSERT_TRUE(fs.isInit());
    ASSERT_FALSE(loop.empty());
    ASSERT_EQ(1, loop.size());

    ASSERT_EQ(E_SUCCESS, fs.init(loop));
    ASSERT_TRUE(fs.isInit());
    ASSERT_FALSE(loop.empty());
    ASSERT_EQ(2, loop.size());

    UxIdle idle;
    ASSERT_EQ(E_SUCCESS, idle.init(loop));
    ASSERT_TRUE(idle.isInit());
    ASSERT_FALSE(loop.empty());
    ASSERT_EQ(3, loop.size());

    UxCheck check;
    ASSERT_EQ(E_SUCCESS, check.init(loop));
    ASSERT_TRUE(check.isInit());
    ASSERT_FALSE(loop.empty());
    ASSERT_EQ(4, loop.size());

    libtbag::lock::UvLock lock;
    libtbag::lock::UvCondition cond;
    bool on_idle = false;
    bool on_check = false;

    idle.setOnIdle([&](){
        lock.lock();
        on_idle = true;
        cond.broadcast();
        lock.unlock();
        idle.close();
    });
    ASSERT_EQ(E_SUCCESS, idle.start());

    check.setOnCheck([&](){
        lock.lock();
        on_check = true;
        cond.broadcast();
        lock.unlock();
        check.close();
    });
    ASSERT_EQ(E_SUCCESS, check.start());

    libtbag::filesystem::File f;
    ASSERT_TRUE(f.open(PATH));
    ASSERT_TRUE(f.isOpen());

    int event_counter = 0;
    int close_counter = 0;

    std::string fs_filename;
    UxFsEvent::Event fs_events = 0;
    Err fs_status = E_UNKNOWN;

    fs.setOnFsEvent([&](const char * filename, UxFsEvent::Event events, Err status){
        ++event_counter;
        fs_filename = filename;
        fs_events = events;
        fs_status = status;
        fs.close();
    });
    fs.setOnClose([&](){
        ++close_counter;
    });

    ASSERT_EQ(E_SUCCESS, fs.start(PATH.c_str()));

    Err loop_result = E_UNKNOWN;
    auto thread = std::thread([&](){
        loop_result = loop.run();
    });

    lock.lock();
    while (true) {
        if (on_idle && on_check) {
            break;
        }
        cond.wait(lock);
    }
    lock.unlock();

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    int const WRITE_SIZE = 4;
    ASSERT_EQ(WRITE_SIZE, f.write("TEMP", WRITE_SIZE, 0));
    ASSERT_TRUE(f.close());

    thread.join();
    ASSERT_TRUE(loop.empty());
    ASSERT_EQ(E_SUCCESS, loop_result);
    ASSERT_EQ(1, event_counter);
    ASSERT_EQ(1, close_counter);
    ASSERT_STREQ(TEST_FILENAME, fs_filename.c_str());
    ASSERT_LT(0, fs_events);
    ASSERT_EQ(E_SUCCESS, fs_status);
}

