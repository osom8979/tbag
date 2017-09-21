/**
 * @file   PointerTest.cpp
 * @brief  Pointer class tester.
 * @author zer0
 * @date   2016-10-16
 */

#include <gtest/gtest.h>
#include <libtbag/container/Pointer.hpp>
#include <iostream>

using namespace libtbag;
using namespace libtbag::container;

struct PointerTestDemo
{
    int value = 0;

    PointerTestDemo(int v) : value(v)
    { /* EMPTY. */ }

    PointerTestDemo() = default;
    ~PointerTestDemo() = default;
};

TEST(PointerTest, CoverageOnly)
{
    PointerTestDemo * i1 = new PointerTestDemo(100);
    PointerTestDemo * i2 = new PointerTestDemo(200);

    Pointer<PointerTestDemo> p1 = i1;
    Pointer<PointerTestDemo> p2 = i2;

    bool r1 = (p1 == p2);
    bool r2 = (p1 != p2);
    bool r3 = (p1  < p2);
    bool r4 = (p1  > p2);
    bool r5 = static_cast<bool>(p1);

    ASSERT_EQ(100, p1->value);
    ASSERT_EQ(200, p2->value);

    delete i1;
    delete i2;
}

TEST(PointerTest, Constructor)
{
    int * i1 = new int(100);
    int * i2 = new int(200);

    Pointer<int> p0;
    ASSERT_EQ(nullptr, p0.get());

    Pointer<int> p1 = i1;
    ASSERT_EQ(i1, p1.get());

    Pointer<int> p2;
    p2 = i2;
    ASSERT_EQ(i2, p2.get());

    Pointer<int> p3 = p1;
    ASSERT_EQ(i1, p1.get());
    ASSERT_EQ(i1, p3.get());

    Pointer<int> p4 = std::move(p1);
    ASSERT_EQ(nullptr, p1.get());
    ASSERT_EQ(i1, p3.get());
    ASSERT_EQ(i1, p4.get());

    p3 = nullptr;
    p3 = std::move(p4);
    ASSERT_EQ(i1, p3.get());
    ASSERT_EQ(nullptr, p4.get());

    delete i1;
    delete i2;
}

TEST(PointerTest, Cast)
{
    PointerTestDemo * i1 = new PointerTestDemo(100);
    Pointer<void> p0((void*)i1);

    ASSERT_EQ(100, p0.cast<PointerTestDemo>()->value);
    ASSERT_EQ(100, p0.forceCast<PointerTestDemo>()->value);

    Pointer<PointerTestDemo> p1 = p0.to<PointerTestDemo>();
    ASSERT_EQ(100, p1->value);

    delete i1;
}

TEST(PointerTest, ToString)
{
    int temp = 0;
    Pointer<int> ptr(&temp);
    std::cout << "Pointer::toString(): " << ptr.toString() << std::endl;
}

