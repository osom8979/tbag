/**
 * @file   MqPipelineTest.cpp
 * @brief  MqPipeline class tester.
 * @author zer0
 * @date   2018-12-07
 */

#include <gtest/gtest.h>
#include <libtbag/mq/MqPipeline.hpp>

using namespace libtbag;
using namespace libtbag::mq;

TEST(MqPipelineTest, Default)
{
    MqPipeline object;
    ASSERT_TRUE(true);
}

