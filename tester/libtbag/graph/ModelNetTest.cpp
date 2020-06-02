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
#include <libtbag/string/StringUtils.hpp>

#include <cstring>
#include <algorithm>
#include <sstream>
#include <set>

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

struct ModelNetTestLayer : public LayerBase
{
    int index;
    bool is_complete;

    ModelNetTestLayer(int i) : index(i), is_complete(false)
    { /* EMPTY. */ }
    virtual ~ModelNetTestLayer()
    { /* EMPTY. */ }

    Err runner(Direction direction, void * user) override
    {
        if (is_complete) {
            return E_SUCCESS;
        }
        is_complete = true;
        return E_SUCCESS;
    }
};

struct ModelNetFixture : public testing::Test
{
    ModelNet net;

    void SetUp() override
    {
        ModelLayer log0 = ModelLayer::create<ModelNetTestLayer>(0);
        ModelLayer log1 = ModelLayer::create<ModelNetTestLayer>(1);
        ModelLayer log2 = ModelLayer::create<ModelNetTestLayer>(2);
        ModelLayer log3 = ModelLayer::create<ModelNetTestLayer>(3);
        ModelLayer log4 = ModelLayer::create<ModelNetTestLayer>(4);
        ModelLayer log5 = ModelLayer::create<ModelNetTestLayer>(5);
        ModelLayer log6 = ModelLayer::create<ModelNetTestLayer>(6);
        ModelLayer log7 = ModelLayer::create<ModelNetTestLayer>(7);
        ModelLayer log8 = ModelLayer::create<ModelNetTestLayer>(8);

        ASSERT_EQ(int(LayerBase::NO_ASSIGN_ID), log0.id());
        ASSERT_EQ(int(LayerBase::NO_ASSIGN_ID), log1.id());
        ASSERT_EQ(int(LayerBase::NO_ASSIGN_ID), log2.id());
        ASSERT_EQ(int(LayerBase::NO_ASSIGN_ID), log3.id());
        ASSERT_EQ(int(LayerBase::NO_ASSIGN_ID), log4.id());
        ASSERT_EQ(int(LayerBase::NO_ASSIGN_ID), log5.id());
        ASSERT_EQ(int(LayerBase::NO_ASSIGN_ID), log6.id());
        ASSERT_EQ(int(LayerBase::NO_ASSIGN_ID), log7.id());
        ASSERT_EQ(int(LayerBase::NO_ASSIGN_ID), log8.id());

        net.addNode(log0);
        net.addNode(log1);
        net.addNode(log2);
        net.addNode(log3);
        net.addNode(log4);
        net.addNode(log5);
        net.addNode(log6);
        net.addNode(log7);
        net.addNode(log8);

        auto const LOG0_ID = log0.id();
        auto const LOG1_ID = log1.id();
        auto const LOG2_ID = log2.id();
        auto const LOG3_ID = log3.id();
        auto const LOG4_ID = log4.id();
        auto const LOG5_ID = log5.id();
        auto const LOG6_ID = log6.id();
        auto const LOG7_ID = log7.id();
        auto const LOG8_ID = log8.id();

        ASSERT_EQ(0, LOG0_ID);
        ASSERT_EQ(1, LOG1_ID);
        ASSERT_EQ(2, LOG2_ID);
        ASSERT_EQ(3, LOG3_ID);
        ASSERT_EQ(4, LOG4_ID);
        ASSERT_EQ(5, LOG5_ID);
        ASSERT_EQ(6, LOG6_ID);
        ASSERT_EQ(7, LOG7_ID);
        ASSERT_EQ(8, LOG8_ID);

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
        /*                               */
        /*  0 -+-> 1 -+-> 3 -+-> 7 <- 8  */
        /*  |  |      |      |           */
        /*  |  |      +-> 4 -+           */
        /*  |  |             |           */
        /*  |  +-> 2 -+-> 5  |           */
        /*  |         |      |           */
        /*  +---------+-> 6 -+           */
        /*                               */
        /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
        net.addArc(log0, log1);
        net.addArc(log0, log2);
        net.addArc(log0, log6);
        net.addArc(log1, log3);
        net.addArc(log1, log4);
        net.addArc(log2, log5);
        net.addArc(log2, log6);
        net.addArc(log3, log7);
        net.addArc(log4, log7);
        net.addArc(log6, log7);
        net.addArc(log8, log7);
    }

    void TearDown() override
    {
        net.clear();
    }

    bool isComplete(int id) const
    {
        return net.getLayer(id).cast<ModelNetTestLayer>()->is_complete;
    }

    Err dependencyForward_SkipFirst(std::set<int> const & start) const
    {
        return net.dependencyForward(start, ModelNet::MAX_RUN_DEPTH,
                                     nullptr, nullptr, false, true);
    }

    Err dependencyBackward_SkipFirst(std::set<int> const & start) const
    {
        return net.dependencyBackward(start, ModelNet::MAX_RUN_DEPTH,
                                      nullptr, nullptr, false, true);
    }
};

TEST_F(ModelNetFixture, Case1_Forward_Depth0)
{
    ASSERT_EQ(0, net.forward({0}, 0));
    ASSERT_FALSE(isComplete(0));
    ASSERT_FALSE(isComplete(1));
    ASSERT_FALSE(isComplete(2));
    ASSERT_FALSE(isComplete(3));
    ASSERT_FALSE(isComplete(4));
    ASSERT_FALSE(isComplete(5));
    ASSERT_FALSE(isComplete(6));
    ASSERT_FALSE(isComplete(7));
    ASSERT_FALSE(isComplete(8));
}

TEST_F(ModelNetFixture, Case1_Forward_Depth1)
{
    ASSERT_EQ(1, net.forward({0}, 1));
    ASSERT_TRUE(isComplete(0));
    ASSERT_FALSE(isComplete(1));
    ASSERT_FALSE(isComplete(2));
    ASSERT_FALSE(isComplete(3));
    ASSERT_FALSE(isComplete(4));
    ASSERT_FALSE(isComplete(5));
    ASSERT_FALSE(isComplete(6));
    ASSERT_FALSE(isComplete(7));
    ASSERT_FALSE(isComplete(8));
}

TEST_F(ModelNetFixture, Case1_Forward_Depth2)
{
    ASSERT_EQ(2, net.forward({0}, 2));
    ASSERT_TRUE(isComplete(0));
    ASSERT_TRUE(isComplete(1));
    ASSERT_TRUE(isComplete(2));
    ASSERT_FALSE(isComplete(3));
    ASSERT_FALSE(isComplete(4));
    ASSERT_FALSE(isComplete(5));
    ASSERT_TRUE(isComplete(6));
    ASSERT_FALSE(isComplete(7));
    ASSERT_FALSE(isComplete(8));
}

TEST_F(ModelNetFixture, Case1_Forward_Depth3)
{
    ASSERT_EQ(3, net.forward({0}, 3));
    ASSERT_TRUE(isComplete(0));
    ASSERT_TRUE(isComplete(1));
    ASSERT_TRUE(isComplete(2));
    ASSERT_TRUE(isComplete(3));
    ASSERT_TRUE(isComplete(4));
    ASSERT_TRUE(isComplete(5));
    ASSERT_TRUE(isComplete(6));
    ASSERT_TRUE(isComplete(7));
    ASSERT_FALSE(isComplete(8));
}

TEST_F(ModelNetFixture, Case1_Forward_Depth4)
{
    ASSERT_EQ(3, net.forward({0}, 4));
    ASSERT_TRUE(isComplete(0));
    ASSERT_TRUE(isComplete(1));
    ASSERT_TRUE(isComplete(2));
    ASSERT_TRUE(isComplete(3));
    ASSERT_TRUE(isComplete(4));
    ASSERT_TRUE(isComplete(5));
    ASSERT_TRUE(isComplete(6));
    ASSERT_TRUE(isComplete(7));
    ASSERT_FALSE(isComplete(8));
}

TEST_F(ModelNetFixture, Case2_Backward_Depth0)
{
    ASSERT_EQ(0, net.backward({7}, 0));
    ASSERT_FALSE(isComplete(0));
    ASSERT_FALSE(isComplete(1));
    ASSERT_FALSE(isComplete(2));
    ASSERT_FALSE(isComplete(3));
    ASSERT_FALSE(isComplete(4));
    ASSERT_FALSE(isComplete(5));
    ASSERT_FALSE(isComplete(6));
    ASSERT_FALSE(isComplete(7));
    ASSERT_FALSE(isComplete(8));
}

TEST_F(ModelNetFixture, Case2_Backward_Depth1)
{
    ASSERT_EQ(1, net.backward({7}, 1));
    ASSERT_FALSE(isComplete(0));
    ASSERT_FALSE(isComplete(1));
    ASSERT_FALSE(isComplete(2));
    ASSERT_FALSE(isComplete(3));
    ASSERT_FALSE(isComplete(4));
    ASSERT_FALSE(isComplete(5));
    ASSERT_FALSE(isComplete(6));
    ASSERT_TRUE(isComplete(7));
    ASSERT_FALSE(isComplete(8));
}

TEST_F(ModelNetFixture, Case2_Backward_Depth2)
{
    ASSERT_EQ(2, net.backward({7}, 2));
    ASSERT_FALSE(isComplete(0));
    ASSERT_FALSE(isComplete(1));
    ASSERT_FALSE(isComplete(2));
    ASSERT_TRUE(isComplete(3));
    ASSERT_TRUE(isComplete(4));
    ASSERT_FALSE(isComplete(5));
    ASSERT_TRUE(isComplete(6));
    ASSERT_TRUE(isComplete(7));
    ASSERT_TRUE(isComplete(8));
}

TEST_F(ModelNetFixture, Case2_Backward_Depth3)
{
    ASSERT_EQ(3, net.backward({7}, 3));
    ASSERT_TRUE(isComplete(0));
    ASSERT_TRUE(isComplete(1));
    ASSERT_TRUE(isComplete(2));
    ASSERT_TRUE(isComplete(3));
    ASSERT_TRUE(isComplete(4));
    ASSERT_FALSE(isComplete(5));
    ASSERT_TRUE(isComplete(6));
    ASSERT_TRUE(isComplete(7));
    ASSERT_TRUE(isComplete(8));
}

TEST_F(ModelNetFixture, Case2_Backward_Depth4)
{
    ASSERT_EQ(3, net.backward({7}, 4));
    ASSERT_TRUE(isComplete(0));
    ASSERT_TRUE(isComplete(1));
    ASSERT_TRUE(isComplete(2));
    ASSERT_TRUE(isComplete(3));
    ASSERT_TRUE(isComplete(4));
    ASSERT_FALSE(isComplete(5));
    ASSERT_TRUE(isComplete(6));
    ASSERT_TRUE(isComplete(7));
    ASSERT_TRUE(isComplete(8));
}

TEST_F(ModelNetFixture, Case3_Backward)
{
    ASSERT_EQ(2, net.backward({4}));
    ASSERT_TRUE(isComplete(0));
    ASSERT_TRUE(isComplete(1));
    ASSERT_FALSE(isComplete(2));
    ASSERT_FALSE(isComplete(3));
    ASSERT_TRUE(isComplete(4));
    ASSERT_FALSE(isComplete(5));
    ASSERT_FALSE(isComplete(6));
    ASSERT_FALSE(isComplete(7));
    ASSERT_FALSE(isComplete(8));
}

TEST_F(ModelNetFixture, Case4_Forward)
{
    ASSERT_EQ(1, net.forward({3}));
    ASSERT_FALSE(isComplete(0));
    ASSERT_FALSE(isComplete(1));
    ASSERT_FALSE(isComplete(2));
    ASSERT_TRUE(isComplete(3));
    ASSERT_FALSE(isComplete(4));
    ASSERT_FALSE(isComplete(5));
    ASSERT_FALSE(isComplete(6));
    ASSERT_TRUE(isComplete(7));
    ASSERT_FALSE(isComplete(8));
}

TEST_F(ModelNetFixture, Case5_DependencyForward_Depth0)
{
    ASSERT_EQ(E_SUCCESS, net.dependencyForward({0}, 0));
    ASSERT_FALSE(isComplete(0));
    ASSERT_FALSE(isComplete(1));
    ASSERT_FALSE(isComplete(2));
    ASSERT_FALSE(isComplete(3));
    ASSERT_FALSE(isComplete(4));
    ASSERT_FALSE(isComplete(5));
    ASSERT_FALSE(isComplete(6));
    ASSERT_FALSE(isComplete(7));
    ASSERT_FALSE(isComplete(8));
}

TEST_F(ModelNetFixture, Case5_DependencyForward_Depth1)
{
    ASSERT_EQ(E_SUCCESS, net.dependencyForward({0}, 1));
    ASSERT_TRUE(isComplete(0));
    ASSERT_FALSE(isComplete(1));
    ASSERT_FALSE(isComplete(2));
    ASSERT_FALSE(isComplete(3));
    ASSERT_FALSE(isComplete(4));
    ASSERT_FALSE(isComplete(5));
    ASSERT_FALSE(isComplete(6));
    ASSERT_FALSE(isComplete(7));
    ASSERT_FALSE(isComplete(8));
}

TEST_F(ModelNetFixture, Case5_DependencyForward_Depth2)
{
    ASSERT_EQ(E_SUCCESS, net.dependencyForward({0}, 2));
    ASSERT_TRUE(isComplete(0));
    ASSERT_TRUE(isComplete(1));
    ASSERT_TRUE(isComplete(2));
    ASSERT_FALSE(isComplete(3));
    ASSERT_FALSE(isComplete(4));
    ASSERT_FALSE(isComplete(5));
    ASSERT_FALSE(isComplete(6));
    ASSERT_FALSE(isComplete(7));
    ASSERT_FALSE(isComplete(8));
}

TEST_F(ModelNetFixture, Case5_DependencyForward_Depth3)
{
    ASSERT_EQ(E_SUCCESS, net.dependencyForward({0}, 3));
    ASSERT_TRUE(isComplete(0));
    ASSERT_TRUE(isComplete(1));
    ASSERT_TRUE(isComplete(2));
    ASSERT_TRUE(isComplete(3));
    ASSERT_TRUE(isComplete(4));
    ASSERT_TRUE(isComplete(5));
    ASSERT_TRUE(isComplete(6));
    ASSERT_FALSE(isComplete(7));
    ASSERT_FALSE(isComplete(8));
}

TEST_F(ModelNetFixture, Case5_DependencyForward_Depth4)
{
    ASSERT_EQ(E_SUCCESS, net.dependencyForward({0}, 4));
    ASSERT_TRUE(isComplete(0));
    ASSERT_TRUE(isComplete(1));
    ASSERT_TRUE(isComplete(2));
    ASSERT_TRUE(isComplete(3));
    ASSERT_TRUE(isComplete(4));
    ASSERT_TRUE(isComplete(5));
    ASSERT_TRUE(isComplete(6));
    ASSERT_FALSE(isComplete(7));
    ASSERT_FALSE(isComplete(8));
}

TEST_F(ModelNetFixture, Case6_DependencyBackward_Depth0)
{
    ASSERT_EQ(E_SUCCESS, net.dependencyBackward({7}, 0));
    ASSERT_FALSE(isComplete(0));
    ASSERT_FALSE(isComplete(1));
    ASSERT_FALSE(isComplete(2));
    ASSERT_FALSE(isComplete(3));
    ASSERT_FALSE(isComplete(4));
    ASSERT_FALSE(isComplete(5));
    ASSERT_FALSE(isComplete(6));
    ASSERT_FALSE(isComplete(7));
    ASSERT_FALSE(isComplete(8));
}

TEST_F(ModelNetFixture, Case6_DependencyBackward_Depth1)
{
    ASSERT_EQ(E_SUCCESS, net.dependencyBackward({7}, 1));
    ASSERT_FALSE(isComplete(0));
    ASSERT_FALSE(isComplete(1));
    ASSERT_FALSE(isComplete(2));
    ASSERT_FALSE(isComplete(3));
    ASSERT_FALSE(isComplete(4));
    ASSERT_FALSE(isComplete(5));
    ASSERT_FALSE(isComplete(6));
    ASSERT_TRUE(isComplete(7));
    ASSERT_FALSE(isComplete(8));
}

TEST_F(ModelNetFixture, Case6_DependencyBackward_Depth2)
{
    ASSERT_EQ(E_SUCCESS, net.dependencyBackward({7}, 2));
    ASSERT_FALSE(isComplete(0));
    ASSERT_FALSE(isComplete(1));
    ASSERT_FALSE(isComplete(2));
    ASSERT_TRUE(isComplete(3));
    ASSERT_TRUE(isComplete(4));
    ASSERT_FALSE(isComplete(5));
    ASSERT_TRUE(isComplete(6));
    ASSERT_TRUE(isComplete(7));
    ASSERT_TRUE(isComplete(8));
}

TEST_F(ModelNetFixture, Case6_DependencyBackward_Depth3)
{
    ASSERT_EQ(E_SUCCESS, net.dependencyBackward({7}, 3));
    ASSERT_FALSE(isComplete(0));
    ASSERT_TRUE(isComplete(1));
    ASSERT_FALSE(isComplete(2));
    ASSERT_TRUE(isComplete(3));
    ASSERT_TRUE(isComplete(4));
    ASSERT_FALSE(isComplete(5));
    ASSERT_TRUE(isComplete(6));
    ASSERT_TRUE(isComplete(7));
    ASSERT_TRUE(isComplete(8));
}

TEST_F(ModelNetFixture, Case6_DependencyBackward_Depth4)
{
    ASSERT_EQ(E_SUCCESS, net.dependencyBackward({7}, 4));
    ASSERT_FALSE(isComplete(0));
    ASSERT_TRUE(isComplete(1));
    ASSERT_FALSE(isComplete(2));
    ASSERT_TRUE(isComplete(3));
    ASSERT_TRUE(isComplete(4));
    ASSERT_FALSE(isComplete(5));
    ASSERT_TRUE(isComplete(6));
    ASSERT_TRUE(isComplete(7));
    ASSERT_TRUE(isComplete(8));
}

TEST_F(ModelNetFixture, Case7_DependencyBackward)
{
    ASSERT_EQ(E_SUCCESS, dependencyForward_SkipFirst({4}));
    ASSERT_FALSE(isComplete(0));
    ASSERT_FALSE(isComplete(1));
    ASSERT_FALSE(isComplete(2));
    ASSERT_FALSE(isComplete(3));
    ASSERT_TRUE(isComplete(4));
    ASSERT_FALSE(isComplete(5));
    ASSERT_FALSE(isComplete(6));
    ASSERT_FALSE(isComplete(7));
    ASSERT_FALSE(isComplete(8));
}

TEST_F(ModelNetFixture, Case8_DependencyBackward)
{
    ASSERT_EQ(E_SUCCESS, dependencyBackward_SkipFirst({3, 4}));
    ASSERT_FALSE(isComplete(0));
    ASSERT_TRUE(isComplete(1));
    ASSERT_FALSE(isComplete(2));
    ASSERT_TRUE(isComplete(3));
    ASSERT_TRUE(isComplete(4));
    ASSERT_FALSE(isComplete(5));
    ASSERT_FALSE(isComplete(6));
    ASSERT_FALSE(isComplete(7));
    ASSERT_FALSE(isComplete(8));
}

TEST_F(ModelNetFixture, Case9_DependencyForward)
{
    ASSERT_EQ(E_SUCCESS, dependencyForward_SkipFirst({3}));
    ASSERT_FALSE(isComplete(0));
    ASSERT_FALSE(isComplete(1));
    ASSERT_FALSE(isComplete(2));
    ASSERT_TRUE(isComplete(3));
    ASSERT_FALSE(isComplete(4));
    ASSERT_FALSE(isComplete(5));
    ASSERT_FALSE(isComplete(6));
    ASSERT_FALSE(isComplete(7));
    ASSERT_FALSE(isComplete(8));
}

