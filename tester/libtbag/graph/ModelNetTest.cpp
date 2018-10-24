/**
 * @file   ModelNetTest.cpp
 * @brief  ModelNet class tester.
 * @author zer0
 * @date   2018-10-22
 */

#include <gtest/gtest.h>
#include <libtbag/graph/ModelNet.hpp>

using namespace libtbag;
using namespace libtbag::graph;

TEST(ModelNetTest, Constructor)
{
    ModelNet obj1;
    ModelNet obj2(nullptr);

    ASSERT_TRUE(static_cast<bool>(obj1));
    ASSERT_FALSE(static_cast<bool>(obj2));

    ModelNet obj3 = obj2;
    ASSERT_FALSE(static_cast<bool>(obj3));
    ASSERT_FALSE(static_cast<bool>(obj2));

    ModelNet obj4;
    ASSERT_TRUE(static_cast<bool>(obj4));
    ModelNet obj5 = std::move(obj4);
    ASSERT_FALSE(static_cast<bool>(obj4));
    ASSERT_TRUE(static_cast<bool>(obj5));
}

TEST(ModelNetTest, Default)
{
    ModelNet net;

    // Depth 0
    auto log0 = ModelLayer::createLog();

    // Depth 1
    auto log1 = ModelLayer::createLog();
    auto log2 = ModelLayer::createLog();

    // Depth 2
    auto log3 = ModelLayer::createLog();
    auto log4 = ModelLayer::createLog();
    auto log5 = ModelLayer::createLog();
    auto log6 = ModelLayer::createLog();

    // Depth 3
    auto log7 = ModelLayer::createLog();

    // Depth Unknown
    auto log8 = ModelLayer::createLog();

    ASSERT_EQ(int(ModelLayer::UNKNOWN_ID), log0.getId());
    ASSERT_EQ(int(ModelLayer::UNKNOWN_ID), log1.getId());
    ASSERT_EQ(int(ModelLayer::UNKNOWN_ID), log2.getId());
    ASSERT_EQ(int(ModelLayer::UNKNOWN_ID), log3.getId());
    ASSERT_EQ(int(ModelLayer::UNKNOWN_ID), log4.getId());
    ASSERT_EQ(int(ModelLayer::UNKNOWN_ID), log5.getId());
    ASSERT_EQ(int(ModelLayer::UNKNOWN_ID), log6.getId());
    ASSERT_EQ(int(ModelLayer::UNKNOWN_ID), log7.getId());
    ASSERT_EQ(int(ModelLayer::UNKNOWN_ID), log8.getId());

    ASSERT_FALSE(log0.isComplete());
    ASSERT_FALSE(log1.isComplete());
    ASSERT_FALSE(log2.isComplete());
    ASSERT_FALSE(log3.isComplete());
    ASSERT_FALSE(log4.isComplete());
    ASSERT_FALSE(log5.isComplete());
    ASSERT_FALSE(log6.isComplete());
    ASSERT_FALSE(log7.isComplete());
    ASSERT_FALSE(log8.isComplete());

    ASSERT_EQ(Err::E_SUCCESS, net.addFirst(log0));
    ASSERT_EQ(Err::E_SUCCESS, net.addNode(log1));
    ASSERT_EQ(Err::E_SUCCESS, net.addNode(log2));
    ASSERT_EQ(Err::E_SUCCESS, net.addNode(log3));
    ASSERT_EQ(Err::E_SUCCESS, net.addNode(log4));
    ASSERT_EQ(Err::E_SUCCESS, net.addNode(log5));
    ASSERT_EQ(Err::E_SUCCESS, net.addNode(log6));
    ASSERT_EQ(Err::E_SUCCESS, net.addLast(log7));
    ASSERT_EQ(Err::E_SUCCESS, net.addFirst(log8));

    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
    /* (F) = First                   */
    /* (L) = Last                    */
    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
    /*                               */
    /* (F)                  (L)  (F) */
    /*  0 -+-> 1 -+-> 3 -+-> 7 <- 8  */
    /*  |  |      |      |           */
    /*  |  |      +-> 4 -+           */
    /*  |  |             |           */
    /*  |  +-> 2 -+-> 5  |           */
    /*  |         |      |           */
    /*  +---------+-> 6 -+           */
    /*                               */
    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

    // Depth 0
    ASSERT_EQ(Err::E_SUCCESS, net.addArc(log0, log1));
    ASSERT_EQ(Err::E_SUCCESS, net.addArc(log0, log2));
    ASSERT_EQ(Err::E_SUCCESS, net.addArc(log0, log6));

    // Depth 1
    ASSERT_EQ(Err::E_SUCCESS, net.addArc(log1, log3));
    ASSERT_EQ(Err::E_SUCCESS, net.addArc(log1, log4));
    ASSERT_EQ(Err::E_SUCCESS, net.addArc(log2, log5));
    ASSERT_EQ(Err::E_SUCCESS, net.addArc(log2, log6));

    // Depth 2
    ASSERT_EQ(Err::E_SUCCESS, net.addArc(log3, log7));
    ASSERT_EQ(Err::E_SUCCESS, net.addArc(log4, log7));
    ASSERT_EQ(Err::E_SUCCESS, net.addArc(log6, log7));

    // Depth Unknown
    ASSERT_EQ(Err::E_SUCCESS, net.addArc(log8, log7));

    ASSERT_EQ(Err::E_SUCCESS, net.forward());

    ASSERT_TRUE(log0.isComplete());
    ASSERT_TRUE(log1.isComplete());
    ASSERT_TRUE(log2.isComplete());
    ASSERT_TRUE(log3.isComplete());
    ASSERT_TRUE(log4.isComplete());
    ASSERT_TRUE(log5.isComplete());
    ASSERT_TRUE(log6.isComplete());
    ASSERT_TRUE(log7.isComplete());
    ASSERT_TRUE(log8.isComplete());

    std::cout << net.toString() << std::endl;
}

