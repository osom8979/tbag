/**
 * @file   FlowChartTest.cpp
 * @brief  FlowChart class tester.
 * @author zer0
 * @date   2017-12-21
 */

#include <gtest/gtest.h>
#include <libtbag/flow/FlowChart.hpp>
#include <vector>

using namespace libtbag;
using namespace libtbag::flow;

struct TestFlowLayer : public FlowLayer<int, int>
{
    using Base = FlowLayer<int, int>;
    using Key  = Base::Key;

    TestFlowLayer(Key const & key) : Base(key) { /* EMPTY. */ }
    virtual ~TestFlowLayer() { /* EMPTY. */ }

    DataMap  forward_data;
    DataMap backward_data;

    virtual void init() override
    {
        // EMPTY.
    }

    virtual bool forward(DataMap const & prev) override
    {
        forward_data = prev;
        return true;
    }

    virtual bool backward(DataMap const & next) override
    {
        backward_data = next;
        return true;
    }
};

TEST(FlowChartTest, Default)
{
    FlowChart<int, int> flow;
    ASSERT_EQ(0, flow.size());
    ASSERT_TRUE(flow.empty());

    ASSERT_TRUE(flow.createAndInsert<TestFlowLayer>(0));
    ASSERT_EQ(1, flow.size());
    ASSERT_FALSE(flow.empty());

    ASSERT_TRUE( static_cast<bool>(flow.get(0)));
    ASSERT_FALSE(static_cast<bool>(flow.get(1)));

    ASSERT_TRUE( static_cast<bool>(flow.cast<TestFlowLayer>(0)));
    ASSERT_FALSE(static_cast<bool>(flow.cast<TestFlowLayer>(1)));

    flow.init();
    ASSERT_TRUE(flow.forward());
    ASSERT_TRUE(flow.backward());

    ASSERT_TRUE(flow.erase(0));
    ASSERT_EQ(0, flow.size());
    ASSERT_TRUE(flow.empty());
}

TEST(FlowChartTest, Run)
{
    FlowChart<int, int> flow;
    ASSERT_TRUE(flow.createAndInsert<TestFlowLayer>(0));
    ASSERT_TRUE(flow.createAndInsert<TestFlowLayer>(1));
    ASSERT_TRUE(flow.createAndInsert<TestFlowLayer>(2));
    ASSERT_TRUE(flow.createAndInsert<TestFlowLayer>(3));

    flow.get(0)->next.push_back(0);
    flow.get(0)->next.push_back(1);

    flow.get(1)->prev.push_back(0);
    flow.get(1)->next.push_back(2);

    flow.get(2)->prev.push_back(0);
    flow.get(2)->prev.push_back(1);
    flow.get(2)->next.push_back(3);

    flow.get(3)->prev.push_back(2);

    ASSERT_TRUE(flow.forward());
    ASSERT_TRUE(flow.backward());

    auto layer0 = flow.cast<TestFlowLayer>(0);
    auto layer1 = flow.cast<TestFlowLayer>(1);
    auto layer2 = flow.cast<TestFlowLayer>(2);
    auto layer3 = flow.cast<TestFlowLayer>(3);

    ASSERT_TRUE(static_cast<bool>(layer0));
    ASSERT_TRUE(static_cast<bool>(layer1));
    ASSERT_TRUE(static_cast<bool>(layer2));
    ASSERT_TRUE(static_cast<bool>(layer3));
    ASSERT_FALSE(static_cast<bool>(flow.cast<TestFlowLayer>(100)));

    ASSERT_EQ(0, layer0-> forward_data.size());
    ASSERT_EQ(2, layer0->backward_data.size());
    ASSERT_EQ(1, layer1-> forward_data.size());
    ASSERT_EQ(1, layer1->backward_data.size());
    ASSERT_EQ(2, layer2-> forward_data.size());
    ASSERT_EQ(1, layer2->backward_data.size());
    ASSERT_EQ(1, layer3-> forward_data.size());
    ASSERT_EQ(0, layer3->backward_data.size());
}

