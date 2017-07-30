/**
 * @file   FsPollTest.cpp
 * @brief  FsPoll class tester.
 * @author zer0
 * @date   2017-05-02
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/uvpp/FsPoll.hpp>
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/filesystem/File.hpp>

#include <thread>
#include <string>

using namespace libtbag;
using namespace libtbag::uvpp;

struct FsPollTest : public FsPoll
{
    using str = std::string;

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
    char const * const TEST_FILENAME = "test.file";
    tttDir(true, true);
    auto path = tttDirGet() / TEST_FILENAME;

    Loop loop;
    auto fs = loop.newHandle<FsPollTest>(loop);

    filesystem::File f;
    ASSERT_TRUE(f.open(path));
    ASSERT_TRUE(f.isOpen());

    ASSERT_EQ(Err::E_SUCCESS, fs->start(path, 10));

    std::thread thread = std::thread([&loop](){
        loop.run();
    });

    busyWaitForAlive(loop);

    while (fs->isActive() == false) {
        // BUSY WAIT.
    }

    int const WRITE_SIZE = 4;
    ASSERT_EQ(WRITE_SIZE, f.write("TEMP", WRITE_SIZE, 0));
    ASSERT_TRUE(f.close());

    thread.join();
    ASSERT_EQ(1, fs->counter);
    ASSERT_EQ(WRITE_SIZE, fs->size);
}

