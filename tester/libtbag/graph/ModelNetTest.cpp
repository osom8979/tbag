/**
 * @file   ModelNetTest.cpp
 * @brief  ModelNet class tester.
 * @author zer0
 * @date   2018-10-22
 */

#include <gtest/gtest.h>
#include <libtbag/graph/ModelNet.hpp>
#include <libtbag/graph/ModelLayer.hpp>
#include <libtbag/log/Log.hpp>

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
    ModelLayer log0;

    // Depth 1
    ModelLayer log1;
    ModelLayer log2;

    // Depth 2
    ModelLayer log3;
    ModelLayer log4;
    ModelLayer log5;
    ModelLayer log6;

    // Depth 3
    ModelLayer log7;

    // Depth Unknown
    ModelLayer log8;

    ASSERT_EQ(int(LayerBase::NO_ASSIGN_ID), log0.id());
    ASSERT_EQ(int(LayerBase::NO_ASSIGN_ID), log1.id());
    ASSERT_EQ(int(LayerBase::NO_ASSIGN_ID), log2.id());
    ASSERT_EQ(int(LayerBase::NO_ASSIGN_ID), log3.id());
    ASSERT_EQ(int(LayerBase::NO_ASSIGN_ID), log4.id());
    ASSERT_EQ(int(LayerBase::NO_ASSIGN_ID), log5.id());
    ASSERT_EQ(int(LayerBase::NO_ASSIGN_ID), log6.id());
    ASSERT_EQ(int(LayerBase::NO_ASSIGN_ID), log7.id());
    ASSERT_EQ(int(LayerBase::NO_ASSIGN_ID), log8.id());

    ASSERT_FALSE(log0.isComplete());
    ASSERT_FALSE(log1.isComplete());
    ASSERT_FALSE(log2.isComplete());
    ASSERT_FALSE(log3.isComplete());
    ASSERT_FALSE(log4.isComplete());
    ASSERT_FALSE(log5.isComplete());
    ASSERT_FALSE(log6.isComplete());
    ASSERT_FALSE(log7.isComplete());
    ASSERT_FALSE(log8.isComplete());

    net.addFirst(log0);
    net.addNode(log1);
    net.addNode(log2);
    net.addNode(log3);
    net.addNode(log4);
    net.addNode(log5);
    net.addNode(log6);
    net.addLast(log7);
    net.addFirst(log8);

    auto const LOG0_ID = log0.id();
    auto const LOG1_ID = log1.id();
    auto const LOG2_ID = log2.id();
    auto const LOG3_ID = log3.id();
    auto const LOG4_ID = log4.id();
    auto const LOG5_ID = log5.id();
    auto const LOG6_ID = log6.id();
    auto const LOG7_ID = log7.id();
    auto const LOG8_ID = log8.id();

    ASSERT_NE(int(LayerBase::NO_ASSIGN_ID), log0.id());
    ASSERT_NE(int(LayerBase::NO_ASSIGN_ID), log1.id());
    ASSERT_NE(int(LayerBase::NO_ASSIGN_ID), log2.id());
    ASSERT_NE(int(LayerBase::NO_ASSIGN_ID), log3.id());
    ASSERT_NE(int(LayerBase::NO_ASSIGN_ID), log4.id());
    ASSERT_NE(int(LayerBase::NO_ASSIGN_ID), log5.id());
    ASSERT_NE(int(LayerBase::NO_ASSIGN_ID), log6.id());
    ASSERT_NE(int(LayerBase::NO_ASSIGN_ID), log7.id());
    ASSERT_NE(int(LayerBase::NO_ASSIGN_ID), log8.id());

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
    net.addArc(log0, log1);
    net.addArc(log0, log2);
    net.addArc(log0, log6);

    // Depth 1
    net.addArc(log1, log3);
    net.addArc(log1, log4);
    net.addArc(log2, log5);
    net.addArc(log2, log6);

    // Depth 2
    net.addArc(log3, log7);
    net.addArc(log4, log7);
    net.addArc(log6, log7);

    // Depth Unknown
    net.addArc(log8, log7);

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

    Err code;
    auto const forward = net.getForwardSequence(1024, nullptr, &code);
    ASSERT_EQ(E_SUCCESS, code);
    ASSERT_EQ(9, forward.size());
    ASSERT_EQ(0, forward[0]);
    ASSERT_EQ(8, forward[1]);
    ASSERT_EQ(1, forward[2]);
    ASSERT_EQ(2, forward[3]);
    ASSERT_EQ(3, forward[4]);
    ASSERT_EQ(4, forward[5]);
    ASSERT_EQ(5, forward[6]);
    ASSERT_EQ(6, forward[7]);
    ASSERT_EQ(7, forward[8]);

    auto const backward = net.getBackwardSequence(1024, nullptr, &code);
    ASSERT_EQ(E_SUCCESS, code);
    ASSERT_EQ(6, backward.size());
    ASSERT_EQ(7, backward[0]);
    ASSERT_EQ(3, backward[1]);
    ASSERT_EQ(4, backward[2]);
    ASSERT_EQ(6, backward[3]);
    ASSERT_EQ(8, backward[4]);
    ASSERT_EQ(1, backward[5]);
}

