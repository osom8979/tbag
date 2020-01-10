/**
 * @file   Box_At_Test.cpp
 * @brief  Box class tester.
 * @author zer0
 * @date   2019-12-27
 */

#include <gtest/gtest.h>
#include <libtbag/box/Box.hpp>

using namespace libtbag;
using namespace libtbag::box;

TEST(Box_At_Test, At)
{
    Box box = {{1, 2}, {3, 4}};
    ASSERT_EQ(1, box.at<si32>(0, 0));
    ASSERT_EQ(2, box.at<si32>(0, 1));
    ASSERT_EQ(3, box.at<si32>(1, 0));
    ASSERT_EQ(4, box.at<si32>(1, 1));
}

TEST(Box_At_Test, At_Complex128)
{
    Box box = {1.0+1.0i, 2.0+2.0i};
    ASSERT_EQ(1, box.at<c128>(0).real());
    ASSERT_EQ(1, box.at<c128>(0).imag());
    ASSERT_EQ(2, box.at<c128>(1).real());
    ASSERT_EQ(2, box.at<c128>(1).imag());
}

TEST(Box_At_Test, Get_ReturnOutput)
{
    Box const box = {{1.0, 2.0}, {3.0, 4.0}};
    ASSERT_EQ(1, box.get<int>(0, 0));
    ASSERT_EQ(2, box.get<int>(0, 1));
    ASSERT_EQ(3, box.get<int>(1, 0));
    ASSERT_EQ(4, box.get<int>(1, 1));

    ui64 ext[] = {0, 0, 0, 0};
    ASSERT_EQ(1, box.get<int>(device_cpu(), ext, 0, 0));
    ASSERT_EQ(2, box.get<int>(device_cpu(), ext, 0, 1));
    ASSERT_EQ(3, box.get<int>(device_cpu(), ext, 1, 0));
    ASSERT_EQ(4, box.get<int>(device_cpu(), ext, 1, 1));

    ui32 dims0[] = {0, 0};
    ui32 dims1[] = {0, 1};
    ui32 dims2[] = {1, 0};
    ui32 dims3[] = {1, 1};
    ASSERT_EQ(1, box.get<int>(2, dims0));
    ASSERT_EQ(2, box.get<int>(2, dims1));
    ASSERT_EQ(3, box.get<int>(2, dims2));
    ASSERT_EQ(4, box.get<int>(2, dims3));

    ASSERT_EQ(1, box.get<int>(device_cpu(), ext, 2, dims0));
    ASSERT_EQ(2, box.get<int>(device_cpu(), ext, 2, dims1));
    ASSERT_EQ(3, box.get<int>(device_cpu(), ext, 2, dims2));
    ASSERT_EQ(4, box.get<int>(device_cpu(), ext, 2, dims3));
}

TEST(Box_At_Test, Get_ArgumentOutput)
{
    Box const box = {{1.0, 2.0}, {3.0, 4.0}};
    int elem0, elem1, elem2, elem3;
    ASSERT_EQ(E_SUCCESS, box.get(&elem0, 0, 0));
    ASSERT_EQ(E_SUCCESS, box.get(&elem1, 0, 1));
    ASSERT_EQ(E_SUCCESS, box.get(&elem2, 1, 0));
    ASSERT_EQ(E_SUCCESS, box.get(&elem3, 1, 1));
    ASSERT_EQ(1, elem0);
    ASSERT_EQ(2, elem1);
    ASSERT_EQ(3, elem2);
    ASSERT_EQ(4, elem3);
    elem0 = 0;
    elem1 = 0;
    elem2 = 0;
    elem3 = 0;

    ui64 ext[] = {0, 0, 0, 0};
    ASSERT_EQ(E_SUCCESS, box.get(&elem0, device_cpu(), ext, 0, 0));
    ASSERT_EQ(E_SUCCESS, box.get(&elem1, device_cpu(), ext, 0, 1));
    ASSERT_EQ(E_SUCCESS, box.get(&elem2, device_cpu(), ext, 1, 0));
    ASSERT_EQ(E_SUCCESS, box.get(&elem3, device_cpu(), ext, 1, 1));
    ASSERT_EQ(1, elem0);
    ASSERT_EQ(2, elem1);
    ASSERT_EQ(3, elem2);
    ASSERT_EQ(4, elem3);
    elem0 = 0;
    elem1 = 0;
    elem2 = 0;
    elem3 = 0;

    ui32 dims0[] = {0, 0};
    ui32 dims1[] = {0, 1};
    ui32 dims2[] = {1, 0};
    ui32 dims3[] = {1, 1};
    ASSERT_EQ(E_SUCCESS, box.get(&elem0, 2, dims0));
    ASSERT_EQ(E_SUCCESS, box.get(&elem1, 2, dims1));
    ASSERT_EQ(E_SUCCESS, box.get(&elem2, 2, dims2));
    ASSERT_EQ(E_SUCCESS, box.get(&elem3, 2, dims3));
    ASSERT_EQ(1, elem0);
    ASSERT_EQ(2, elem1);
    ASSERT_EQ(3, elem2);
    ASSERT_EQ(4, elem3);
    elem0 = 0;
    elem1 = 0;
    elem2 = 0;
    elem3 = 0;

    ASSERT_EQ(E_SUCCESS, box.get(&elem0, device_cpu(), ext, 2, dims0));
    ASSERT_EQ(E_SUCCESS, box.get(&elem1, device_cpu(), ext, 2, dims1));
    ASSERT_EQ(E_SUCCESS, box.get(&elem2, device_cpu(), ext, 2, dims2));
    ASSERT_EQ(E_SUCCESS, box.get(&elem3, device_cpu(), ext, 2, dims3));
    ASSERT_EQ(1, elem0);
    ASSERT_EQ(2, elem1);
    ASSERT_EQ(3, elem2);
    ASSERT_EQ(4, elem3);
}

TEST(Box_At_Test, Set_Get)
{
    Box box = {{0, 0}, {0, 0}};
    ASSERT_EQ(E_SUCCESS, box.set(10.0, 0, 0));
    ASSERT_EQ(E_SUCCESS, box.set(20.0, 0, 1));
    ASSERT_EQ(E_SUCCESS, box.set(30.0, 1, 0));
    ASSERT_EQ(E_SUCCESS, box.set(40.0, 1, 1));
    ASSERT_EQ(10, box.get<int>(0, 0));
    ASSERT_EQ(20, box.get<int>(0, 1));
    ASSERT_EQ(30, box.get<int>(1, 0));
    ASSERT_EQ(40, box.get<int>(1, 1));

    ui64 ext[] = {0, 0, 0, 0};
    ASSERT_EQ(E_SUCCESS, box.set(1.0, device_cpu(), ext, 0, 0));
    ASSERT_EQ(E_SUCCESS, box.set(2.0, device_cpu(), ext, 0, 1));
    ASSERT_EQ(E_SUCCESS, box.set(3.0, device_cpu(), ext, 1, 0));
    ASSERT_EQ(E_SUCCESS, box.set(4.0, device_cpu(), ext, 1, 1));
    ASSERT_EQ(1, box.get<int>(0, 0));
    ASSERT_EQ(2, box.get<int>(0, 1));
    ASSERT_EQ(3, box.get<int>(1, 0));
    ASSERT_EQ(4, box.get<int>(1, 1));

    ui32 dims0[] = {0, 0};
    ui32 dims1[] = {0, 1};
    ui32 dims2[] = {1, 0};
    ui32 dims3[] = {1, 1};
    ASSERT_EQ(E_SUCCESS, box.set(100u, 2, dims0));
    ASSERT_EQ(E_SUCCESS, box.set(200u, 2, dims1));
    ASSERT_EQ(E_SUCCESS, box.set(300u, 2, dims2));
    ASSERT_EQ(E_SUCCESS, box.set(400u, 2, dims3));
    ASSERT_EQ(100, box.get<int>(0, 0));
    ASSERT_EQ(200, box.get<int>(0, 1));
    ASSERT_EQ(300, box.get<int>(1, 0));
    ASSERT_EQ(400, box.get<int>(1, 1));

    ASSERT_EQ(E_SUCCESS, box.set(11.0f, device_cpu(), ext, 2, dims0));
    ASSERT_EQ(E_SUCCESS, box.set(12.0f, device_cpu(), ext, 2, dims1));
    ASSERT_EQ(E_SUCCESS, box.set(13.0f, device_cpu(), ext, 2, dims2));
    ASSERT_EQ(E_SUCCESS, box.set(14.0f, device_cpu(), ext, 2, dims3));
    ASSERT_EQ(11, box.get<int>(0, 0));
    ASSERT_EQ(12, box.get<int>(0, 1));
    ASSERT_EQ(13, box.get<int>(1, 0));
    ASSERT_EQ(14, box.get<int>(1, 1));
}

TEST(Box_At_Test, Set_Get_Cast)
{
    Box box = {0.0};
    ASSERT_EQ(E_SUCCESS, box.set(100.0, 0));
    ASSERT_EQ(100, box.get<ui16>(0));
}

