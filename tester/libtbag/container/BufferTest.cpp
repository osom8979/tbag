/**
 * @file   BufferTest.cpp
 * @brief  Buffer class tester.
 * @author zer0
 * @date   2017-08-01
 */

#include <gtest/gtest.h>
#include <libtbag/container/Buffer.hpp>

using namespace libtbag;
using namespace libtbag::container;

TEST(BufferTest, Coverage)
{
    Buffer<int> buf1;
    Buffer<int> buf2(buf1);
    Buffer<int> buf3(std::move(buf2));
    Buffer<int> buf4(10U);
    Buffer<int> buf5{1, 2, 3};

    Buffer<int> buf6;
    Buffer<int> buf7;

    buf6 = buf4;
    buf7 = std::move(buf5);

    Buffer<int> buf;
    Buffer<int> const cbuf;
     buf.begin();
    cbuf.begin();
     buf.end();
    cbuf.end();

     buf.rbegin();
    cbuf.rbegin();
     buf.rend();
    cbuf.rend();

    buf.cbegin();
    buf.cend();
    buf.crbegin();
    buf.crend();

    cbuf.cbegin();
    cbuf.cend();
    cbuf.crbegin();
    cbuf.crend();

     buf.empty();
    cbuf.empty();
     buf.size();
    cbuf.size();

     buf[0];
    cbuf[0];

     buf.front();
    cbuf.front();
     buf.back();
    cbuf.back();

     buf.data();
    cbuf.data();
}

