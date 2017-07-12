/**
 * @file   RiseServiceTest.cpp
 * @brief  RiseService class tester.
 * @author zer0
 * @date   2017-07-12
 */

#include <gtest/gtest.h>
#include <libtbag/loop/RiseService.hpp>
#include <libtbag/log/Log.hpp>

using namespace libtbag;
using namespace libtbag::loop;

struct TestCom : public RiseComponent
{
    bool result;
    int run_count;

    TestCom(bool r = true, int c = 0) : result(r), run_count(c)
    { /* EMPTY. */ }
    virtual ~TestCom()
    { /* EMPTY. */ }

    virtual bool run() override
    {
        ++run_count;
        return result;
    }
};

TEST(RiseServiceTest, RiseComponentDefault)
{
    TestCom com(true);
    ASSERT_TRUE(com.result);

    ASSERT_TRUE(TestCom::ComState::CS_READY == com.getState());
    ASSERT_FALSE(com.isEnd());
    ASSERT_EQ(0, com.run_count);

    com.autoRun();
    com.joinIfJoinable();

    ASSERT_TRUE(TestCom::ComState::CS_END_SUCCESS == com.getState());
    ASSERT_TRUE(com.isEnd());
    ASSERT_EQ(1, com.run_count);
    ASSERT_EQ(0, com.getTryCount());

    com.autoRun();
    com.joinIfJoinable();

    ASSERT_TRUE(TestCom::ComState::CS_END_SUCCESS == com.getState());
    ASSERT_TRUE(com.isEnd());
    ASSERT_EQ(1, com.run_count);
    ASSERT_EQ(0, com.getTryCount());
}

TEST(RiseServiceTest, RiseComponentRiseRun)
{
    TestCom com(false);
    ASSERT_FALSE(com.result);

    int const TEST_TRY_COUNT = 1000;
    com.setRiseIfFailure(TEST_TRY_COUNT);

    ASSERT_TRUE(TestCom::ComState::CS_READY == com.getState());
    ASSERT_FALSE(com.isEnd());
    ASSERT_EQ(0, com.run_count);
    ASSERT_EQ(0, com.getTryCount());

    com.autoRun();
    com.joinIfJoinable();

    ASSERT_TRUE(TestCom::ComState::CS_THREAD_END_FAILURE == com.getState());
    ASSERT_FALSE(com.isEnd());
    ASSERT_EQ(1, com.run_count);
    ASSERT_EQ(0, com.getTryCount());

    int i = 1;
    for (; i < TEST_TRY_COUNT; ++i) {
        com.autoRun();
        com.joinIfJoinable();

        ASSERT_TRUE(TestCom::ComState::CS_THREAD_END_FAILURE == com.getState());
        ASSERT_FALSE(com.isEnd());
        ASSERT_EQ(i + 1, com.run_count);
        ASSERT_EQ(i + 0, com.getTryCount());
    }

    ASSERT_EQ(TEST_TRY_COUNT, i);

    // Last try ...
    com.autoRun();
    com.joinIfJoinable();

    ASSERT_TRUE(TestCom::ComState::CS_END_FAILURE == com.getState());
    ASSERT_TRUE(com.isEnd());
    ASSERT_EQ(TEST_TRY_COUNT + 1, com.run_count);
    ASSERT_EQ(TEST_TRY_COUNT, com.getTryCount());

    // Over running...
    com.autoRun();
    com.joinIfJoinable();

    ASSERT_TRUE(TestCom::ComState::CS_END_FAILURE == com.getState());
    ASSERT_TRUE(com.isEnd());
    ASSERT_EQ(TEST_TRY_COUNT + 1, com.run_count);
    ASSERT_EQ(TEST_TRY_COUNT, com.getTryCount());
}

TEST(RiseServiceTest, RiseComponentNeverDie)
{
    TestCom com(false);
    ASSERT_FALSE(com.result);
    com.setNeverDie();

    int const TEST_TRY_COUNT = 2000;

    ASSERT_TRUE(TestCom::ComState::CS_READY == com.getState());
    ASSERT_FALSE(com.isEnd());
    ASSERT_EQ(0, com.run_count);
    ASSERT_EQ(0, com.getTryCount());

    com.autoRun();
    com.joinIfJoinable();

    ASSERT_TRUE(TestCom::ComState::CS_THREAD_END_FAILURE == com.getState());
    ASSERT_FALSE(com.isEnd());
    ASSERT_EQ(1, com.run_count);
    ASSERT_EQ(0, com.getTryCount());

    int i = 1;
    for (; i < TEST_TRY_COUNT; ++i) {
        com.autoRun();
        com.joinIfJoinable();

        ASSERT_TRUE(TestCom::ComState::CS_THREAD_END_FAILURE == com.getState());
        ASSERT_FALSE(com.isEnd());
        ASSERT_EQ(i + 1, com.run_count);
        ASSERT_EQ(i + 0, com.getTryCount());
    }
}

TEST(RiseServiceTest, EmptyRiseService)
{
    log::SeverityGuard guard;
    RiseService service;
    ASSERT_EQ(Err::E_SUCCESS, service.run());
}

TEST(RiseServiceTest, RiseServiceCallback)
{
    int  on_create_counter = 0;
    int   on_timer_counter = 0;
    int on_destroy_counter = 0;

    FuncRiseService service(1, true);
    service.setOnCreate([&](FuncRiseService::SharedComSet & coms){
        on_create_counter++;
        ASSERT_TRUE(coms.insert(FuncRiseService::SharedCom(new TestCom)).second);
    });
    service.setOnTimer([&](FuncRiseService::SharedComSet & coms){
        on_timer_counter++;
        ASSERT_EQ(1, coms.size());
    });
    service.setOnDestroy([&](FuncRiseService::SharedComSet & coms){
        on_destroy_counter++;
        ASSERT_EQ(1, coms.size());
    });

    ASSERT_EQ(Err::E_SUCCESS, service.run());
}

TEST(RiseServiceTest, RiseServiceDefault)
{
    RiseService::SharedComSet coms;
    TestCom * test_com1 = new TestCom();
    TestCom * test_com2 = new TestCom();
    auto com1 = RiseService::SharedCom(test_com1);
    auto com2 = RiseService::SharedCom(test_com2);
    ASSERT_TRUE(static_cast<bool>(com1));
    ASSERT_TRUE(static_cast<bool>(com2));
    ASSERT_TRUE(coms.insert(com1).second);
    ASSERT_TRUE(coms.insert(com2).second);
    ASSERT_EQ(2, coms.size());

    RiseService service(1, true);
    ASSERT_EQ(Err::E_SUCCESS, service.run(coms));

    ASSERT_EQ(1, test_com1->run_count);
    ASSERT_EQ(1, test_com2->run_count);
}

