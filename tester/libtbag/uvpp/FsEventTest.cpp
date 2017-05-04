/**
 * @file   FsEventTest.cpp
 * @brief  FsEvent class tester.
 * @author zer0
 * @date   2017-05-02
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/uvpp/FsEvent.hpp>
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/filesystem/File.hpp>

#include <thread>
#include <string>

using namespace libtbag;
using namespace libtbag::uvpp;

struct FsEventTest : public FsEvent
{
    using str = std::string;

    int counter;

    str   last_filename;
    Event last_events;
    uerr  last_status;

    FsEventTest(Loop & loop) : FsEvent(loop), counter(0)
    { /* EMPTY. */ }

    virtual void onFsEvent(const char * filename, Event events, uerr status) override
    {
        ++counter;
        last_filename = filename;
        last_events = events;
        last_status = status;

        stop();
        close();
    }
};

TEST(FsEventTest, Default)
{
    char const * const TEST_FILENAME = "test.file";
    tttDir(true, true);
    auto path = tttDirGet() / TEST_FILENAME;

    Loop loop;
    auto fs = loop.newHandle<FsEventTest>(loop);

    filesystem::File f;
    ASSERT_TRUE(f.open(path));
    ASSERT_TRUE(f.isOpen());

    ASSERT_EQ(uerr::UVPP_SUCCESS, fs->start(path));

    std::thread thread = std::thread([&loop](){
        loop.run();
    });
    int const WRITE_SIZE = 4;
    ASSERT_EQ(WRITE_SIZE, f.write2("TEMP", WRITE_SIZE, 0));
    ASSERT_TRUE(f.close());

    thread.join();
    ASSERT_EQ(1, fs->counter);
    ASSERT_STREQ(TEST_FILENAME, fs->last_filename.c_str());
    ASSERT_LT(0, fs->last_events);
    ASSERT_EQ(uerr::UVPP_SUCCESS, fs->last_status);
}

