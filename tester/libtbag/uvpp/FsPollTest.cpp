/**
 * @file   FsPollTest.cpp
 * @brief  FsPoll class tester.
 * @author zer0
 * @date   2017-05-02
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/uvpp/FsPoll.hpp>
#include <libtbag/uvpp/func/FunctionalIdle.hpp>

#include <libtbag/lock/UvLock.hpp>
#include <libtbag/lock/UvCondition.hpp>
#include <libtbag/filesystem/File.hpp>

#include <thread>
#include <iostream>

using namespace libtbag;
using namespace libtbag::uvpp;

struct FsPollTest : public FsPoll
{
    int counter;
    int size;

    FsPollTest(Loop & loop) : FsPoll(loop), counter(0)
    { /* EMPTY. */ }

    virtual void onFsPoll(Err status, FileState const & prev, FileState const & curr) override
    {
        ++counter;
        size = static_cast<int>(curr.size - prev.size);

        stop();
        close();
    }
};

TEST(FsPollTest, Default)
{
#if defined(SKIP_FSPOLL_TESTER)
    std::cout << "Skip this test. (The SKIP_FSPOLL_TESTER macro has been defined)\n";
    return;
#endif

    char const * const TEST_FILENAME = "test.file";
    tttDir_Automatic();
    auto path = tttDir_Get() / TEST_FILENAME;

    Loop loop;
    auto fs = loop.newHandle<FsPollTest>(loop);

    filesystem::File f;
    ASSERT_TRUE(f.open(path));
    ASSERT_TRUE(f.isOpen());

    ASSERT_EQ(Err::E_SUCCESS, fs->start(path.c_str(), 10));

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
    ASSERT_EQ(Err::E_SUCCESS, idle->start());

    Err loop_result = Err::E_UNKNOWN;
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
    ASSERT_EQ(Err::E_SUCCESS, loop_result);
    ASSERT_EQ(1, fs->counter);
    ASSERT_EQ(WRITE_SIZE, fs->size);
}

