/**
 * @file   SingleManagerTest.cpp
 * @brief  SingleManager class tester.
 * @author zer0
 * @date   2017-06-06
 */

#include <gtest/gtest.h>
#include <libtbag/container/SingleManager.hpp>
#include <libtbag/common/Object.hpp>

using namespace libtbag;
using namespace libtbag::container;

TEST(SingleManagerTest, Default)
{
    using Mgr = SingleManager<int, common::Object>;
    Mgr::createInstance();

    ASSERT_TRUE(Mgr::singleEmpty());
    ASSERT_EQ(0, Mgr::singleSize());

    int const KEY1 = 1;
    int const KEY2 = 2;
    int const KEY3 = 3;

    auto weak1 = Mgr::singleNewAdd(KEY1);
    auto weak2 = Mgr::singleNewAdd(KEY2, *(weak1.lock()));
    ASSERT_FALSE(Mgr::singleInsert(KEY3, Mgr::SharedBase(new common::Object())).expired());

    ASSERT_FALSE(Mgr::singleEmpty());
    ASSERT_EQ(3, Mgr::singleSize());

    ASSERT_TRUE (Mgr::singleRemove(KEY1));
    ASSERT_FALSE(Mgr::singleExists(KEY1));
    ASSERT_TRUE (Mgr::singleExists(KEY2));
    ASSERT_TRUE (Mgr::singleExists(KEY3));

    ASSERT_FALSE(Mgr::singleEmpty());
    ASSERT_EQ(2, Mgr::singleSize());

    ASSERT_TRUE (Mgr::singleGet(KEY1).expired());
    ASSERT_FALSE(Mgr::singleGet(KEY2).expired());
    ASSERT_FALSE(Mgr::singleGet(KEY3).expired());

    Mgr::singleClear();
    ASSERT_TRUE(Mgr::singleEmpty());
    ASSERT_EQ(0, Mgr::singleSize());

    Mgr::releaseInstance();
}

