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
#include <libtbag/filesystem/File.hpp>

#include <thread>
#include <string>
#include <iostream>

using namespace libtbag;
using namespace libtbag::uvpp;

struct PollTest : public Poll
{
    using str = std::string;

    int counter;

    PollTest(Loop & loop, int fd) : Poll(loop, init_fd(fd)), counter(0)
    { /* EMPTY. */ }

    virtual void onPoll(uerr status, EventType events) override
    {
        ++counter;
        stop();
        close();
    }
};

TEST(PollTest, Default)
{
#if defined(TBAG_PLATFORM_WINDOWS)
    std::cout << "Skip this test in Windows Platform.\n";
#else
    char const * const TEST_FILENAME = "test.file";
    tttDir(true, true);
    auto path = tttDirGet() / TEST_FILENAME;

    filesystem::File f;
    ASSERT_TRUE(f.open(path));
    ASSERT_TRUE(f.isOpen());

    Loop loop;
    auto fs = loop.newHandle<PollTest>(loop, static_cast<int>(f.getFd()));

    ASSERT_EQ(uerr::UVPP_SUCCESS, fs->start());

    std::thread thread = std::thread([&loop](){
        loop.run();
    });
    int const WRITE_SIZE = 4;
    ASSERT_EQ(WRITE_SIZE, f.write2("TEMP", WRITE_SIZE, 0));
    ASSERT_TRUE(f.close());

    thread.join();
    ASSERT_EQ(1, fs->counter);
#endif
}

