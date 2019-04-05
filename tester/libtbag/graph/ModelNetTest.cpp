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

    ASSERT_EQ(E_SUCCESS, net.addFirst(log0));
    ASSERT_EQ(E_SUCCESS, net.addNode(log1));
    ASSERT_EQ(E_SUCCESS, net.addNode(log2));
    ASSERT_EQ(E_SUCCESS, net.addNode(log3));
    ASSERT_EQ(E_SUCCESS, net.addNode(log4));
    ASSERT_EQ(E_SUCCESS, net.addNode(log5));
    ASSERT_EQ(E_SUCCESS, net.addNode(log6));
    ASSERT_EQ(E_SUCCESS, net.addLast(log7));
    ASSERT_EQ(E_SUCCESS, net.addFirst(log8));

    auto const LOG0_ID = log0.getId();
    auto const LOG1_ID = log1.getId();
    auto const LOG2_ID = log2.getId();
    auto const LOG3_ID = log3.getId();
    auto const LOG4_ID = log4.getId();
    auto const LOG5_ID = log5.getId();
    auto const LOG6_ID = log6.getId();
    auto const LOG7_ID = log7.getId();
    auto const LOG8_ID = log8.getId();

    ASSERT_NE(int(ModelLayer::UNKNOWN_ID), LOG0_ID);
    ASSERT_NE(int(ModelLayer::UNKNOWN_ID), LOG1_ID);
    ASSERT_NE(int(ModelLayer::UNKNOWN_ID), LOG2_ID);
    ASSERT_NE(int(ModelLayer::UNKNOWN_ID), LOG3_ID);
    ASSERT_NE(int(ModelLayer::UNKNOWN_ID), LOG4_ID);
    ASSERT_NE(int(ModelLayer::UNKNOWN_ID), LOG5_ID);
    ASSERT_NE(int(ModelLayer::UNKNOWN_ID), LOG6_ID);
    ASSERT_NE(int(ModelLayer::UNKNOWN_ID), LOG7_ID);
    ASSERT_NE(int(ModelLayer::UNKNOWN_ID), LOG8_ID);

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
    ASSERT_EQ(E_SUCCESS, net.addArc(log0, log1));
    ASSERT_EQ(E_SUCCESS, net.addArc(log0, log2));
    ASSERT_EQ(E_SUCCESS, net.addArc(log0, log6));

    // Depth 1
    ASSERT_EQ(E_SUCCESS, net.addArc(log1, log3));
    ASSERT_EQ(E_SUCCESS, net.addArc(log1, log4));
    ASSERT_EQ(E_SUCCESS, net.addArc(log2, log5));
    ASSERT_EQ(E_SUCCESS, net.addArc(log2, log6));

    // Depth 2
    ASSERT_EQ(E_SUCCESS, net.addArc(log3, log7));
    ASSERT_EQ(E_SUCCESS, net.addArc(log4, log7));
    ASSERT_EQ(E_SUCCESS, net.addArc(log6, log7));

    // Depth Unknown
    ASSERT_EQ(E_SUCCESS, net.addArc(log8, log7));

    ASSERT_EQ(E_SUCCESS, net.forward());

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

    auto const LAYERS = net.getLayers();
    auto const LAYER_LOG0 = net.getLayer(LOG0_ID);
    auto const LAYER_LOG1 = net.getLayer(LOG1_ID);
    auto const LAYER_LOG2 = net.getLayer(LOG2_ID);
    auto const LAYER_LOG3 = net.getLayer(LOG3_ID);
    auto const LAYER_LOG4 = net.getLayer(LOG4_ID);
    auto const LAYER_LOG5 = net.getLayer(LOG5_ID);
    auto const LAYER_LOG6 = net.getLayer(LOG6_ID);
    auto const LAYER_LOG7 = net.getLayer(LOG7_ID);
    auto const LAYER_LOG8 = net.getLayer(LOG8_ID);

    ASSERT_NE(LAYERS.end(), LAYERS.find(LAYER_LOG0));
    ASSERT_NE(LAYERS.end(), LAYERS.find(LAYER_LOG1));
    ASSERT_NE(LAYERS.end(), LAYERS.find(LAYER_LOG2));
    ASSERT_NE(LAYERS.end(), LAYERS.find(LAYER_LOG3));
    ASSERT_NE(LAYERS.end(), LAYERS.find(LAYER_LOG4));
    ASSERT_NE(LAYERS.end(), LAYERS.find(LAYER_LOG5));
    ASSERT_NE(LAYERS.end(), LAYERS.find(LAYER_LOG6));
    ASSERT_NE(LAYERS.end(), LAYERS.find(LAYER_LOG7));
    ASSERT_NE(LAYERS.end(), LAYERS.find(LAYER_LOG8));
    ASSERT_EQ(9, LAYERS.size());
}

