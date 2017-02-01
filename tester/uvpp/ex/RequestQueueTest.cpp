/**
 * @file   RequestQueueTest.cpp
 * @brief  RequestQueue libuv class tester.
 * @author zer0
 * @date   2016-12-31
 */

#include <gtest/gtest.h>
#include <libtbag/uv/ex/RequestQueue.hpp>
#include <libtbag/uv/Handle.hpp>

using namespace libtbag;
using namespace libtbag::uv;
using namespace libtbag::uv::ex;

TEST(RequestQueueTest, Default)
{
    Handle handle(UvHandleType::IDLE);
    WriteQueue req;

    ASSERT_TRUE(req.getPrepareEmpty());
    ASSERT_TRUE(req.getActiveEmpty());
    ASSERT_EQ(0, req.getPrepareSize());
    ASSERT_EQ(0, req.getActiveSize());

    auto w1 = req.create(&handle);
    auto w2 = req.create(&handle);

    ASSERT_TRUE(req.getPrepareEmpty());
    ASSERT_FALSE(req.getActiveEmpty());
    ASSERT_EQ(0, req.getPrepareSize());
    ASSERT_EQ(2, req.getActiveSize());

    auto w1_find = req.find(w1);
    auto w2_find = req.find(w2);

    ASSERT_NE(w1.lock().get(), w2_find.lock().get());
    ASSERT_EQ(w1.lock().get(), w1_find.lock().get());
    ASSERT_EQ(w2.lock().get(), w2_find.lock().get());

    req.release(w1_find);
    req.release(w2_find);

    ASSERT_FALSE(req.getPrepareEmpty());
    ASSERT_TRUE(req.getActiveEmpty());
    ASSERT_EQ(2, req.getPrepareSize());
    ASSERT_EQ(0, req.getActiveSize());

    req.clear();

    ASSERT_TRUE(req.getPrepareEmpty());
    ASSERT_TRUE(req.getActiveEmpty());
    ASSERT_EQ(0, req.getPrepareSize());
    ASSERT_EQ(0, req.getActiveSize());
}

