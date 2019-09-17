/**
 * @file   SyncedWaitTest.cpp
 * @brief  SyncedWait class tester.
 * @author zer0
 * @date   2019-09-11
 */

#include <gtest/gtest.h>
#include <libtbag/time/SyncedWait.hpp>
#include <thread>

using namespace libtbag;
using namespace libtbag::time;

TEST(SyncedWaitTest, Default)
{
    SyncedWait obj;
    ASSERT_EQ(std::string(SyncedWait::STATE_READY), obj.toString());
}

TEST(SyncedWaitTest, Run_And_Delete)
{
    SyncedWait obj;
    auto const result = obj.run([](SyncedWait::RunningSignal & s) -> Err {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        return E_SUCCESS;
    }, 100);
    ASSERT_EQ(E_TIMEOUT, result);
}

TEST(SyncedWaitTest, Run_DirectSuccess)
{
    SyncedWait obj;
    auto const result = obj.run([](SyncedWait::RunningSignal & s) -> Err {
        return E_SUCCESS;
    }, 100);
    ASSERT_EQ(E_SUCCESS, result);
    ASSERT_EQ(E_SUCCESS, obj.done());
}

TEST(SyncedWaitTest, Run_Success)
{
    SyncedWait obj;
    auto const result = obj.run([](SyncedWait::RunningSignal & s) -> Err {
        s.nowRunning();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        return E_SUCCESS;
    }, 100);
    ASSERT_EQ(E_SUCCESS, result);
    ASSERT_EQ(E_SUCCESS, obj.done());
}

TEST(SyncedWaitTest, Run_Error)
{
    SyncedWait obj;
    auto const result = obj.run([](SyncedWait::RunningSignal & s) -> Err {
        return E_UNKNOWN;
    }, 100);
    ASSERT_EQ(E_SUCCESS, result);
    ASSERT_EQ(E_UNKNOWN, obj.done());
}

TEST(SyncedWaitTest, Run_Timeout)
{
    SyncedWait obj;
    auto const result = obj.run([](SyncedWait::RunningSignal & s) -> Err {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        return E_SUCCESS;
    }, 100);
    ASSERT_EQ(E_TIMEOUT, result);
    ASSERT_EQ(E_SUCCESS, obj.done());
}

