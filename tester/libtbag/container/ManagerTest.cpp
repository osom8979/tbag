/**
 * @file   ManagerTest.cpp
 * @brief  Manager class tester.
 * @author zer0
 * @date   2017-09-13
 */

#include <gtest/gtest.h>
#include <libtbag/container/Manager.hpp>
#include <libtbag/common/Object.hpp>

using namespace libtbag;
using namespace libtbag::container;

TEST(ManagerTest, Default)
{
    using Mgr = Manager<int, common::Object>;
    Mgr mgr;

    ASSERT_TRUE(mgr.empty());
    ASSERT_EQ(0, mgr.size());

    int const KEY1 = 1;
    int const KEY2 = 2;
    int const KEY3 = 3;

    auto weak1 = mgr.newAdd(KEY1);
    auto weak2 = mgr.newAdd(KEY2, *(weak1.lock()));
    ASSERT_FALSE(mgr.insert(KEY3, Mgr::SharedBase(new common::Object())).expired());

    ASSERT_FALSE(mgr.empty());
    ASSERT_EQ(3, mgr.size());

    ASSERT_TRUE (mgr.remove(KEY1));
    ASSERT_FALSE(mgr.exists(KEY1));
    ASSERT_TRUE (mgr.exists(KEY2));
    ASSERT_TRUE (mgr.exists(KEY3));

    ASSERT_FALSE(mgr.empty());
    ASSERT_EQ(2, mgr.size());

    std::size_t size = 0;
    mgr.safeRun([&](Mgr::BaseMap const & map){
        size = map.size();
    });
    ASSERT_EQ(2, size);

    ASSERT_TRUE (mgr.get(KEY1).expired());
    ASSERT_FALSE(mgr.get(KEY2).expired());
    ASSERT_FALSE(mgr.get(KEY3).expired());

    mgr.clear();
    ASSERT_TRUE(mgr.empty());
    ASSERT_EQ(0, mgr.size());
}

