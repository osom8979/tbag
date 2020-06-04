/**
 * @file   ModelNet_Sequence_Test.cpp
 * @brief  ModelNet sequence tester.
 * @author zer0
 * @date   2020-06-02
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

TEST(ModelNet_Sequence_Test, Case1)
{
    ModelNet net;

    auto node0 = net.addNewNode<LayerBase>();
    auto node1 = net.addNewNode<LayerBase>();
    auto node2 = net.addNewNode<LayerBase>();
    auto node3 = net.addNewNode<LayerBase>();
    ASSERT_EQ(0, node0.id());
    ASSERT_EQ(1, node1.id());
    ASSERT_EQ(2, node2.id());
    ASSERT_EQ(3, node3.id());

    /* ~~~~~~~~~~~~~~~~~~~~~~~ */
    /*                         */
    /*  0 -+-> 2 -+            */
    /*     |      |            */
    /*     |      |            */
    /*     +------+-> 1 --> 3  */
    /*                         */
    /* ~~~~~~~~~~~~~~~~~~~~~~~ */
    net.addArc(node0, node1);
    net.addArc(node0, node2);
    net.addArc(node1, node3);
    net.addArc(node2, node1);

    std::vector<int> sequence;
    ASSERT_EQ(4, net.backward({3}, ModelNet::MAX_RUN_DEPTH, nullptr,
                                      &sequence, false));
    ASSERT_EQ(5, sequence.size());
    ASSERT_EQ(3, sequence[0]);
    ASSERT_EQ(1, sequence[1]);

    if (sequence[2] == 0) {
        ASSERT_EQ(0, sequence[2]);
        ASSERT_EQ(2, sequence[3]);
    } else {
        ASSERT_EQ(2, sequence[2]);
        ASSERT_EQ(0, sequence[3]);
    }

    ASSERT_EQ(0, sequence[4]);
}

