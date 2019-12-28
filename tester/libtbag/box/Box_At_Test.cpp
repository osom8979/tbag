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
    ASSERT_EQ(1, box.at<Box::si32>(0, 0));
    ASSERT_EQ(2, box.at<Box::si32>(0, 1));
    ASSERT_EQ(3, box.at<Box::si32>(1, 0));
    ASSERT_EQ(4, box.at<Box::si32>(1, 1));
}

TEST(Box_At_Test, Get_ReturnOutput)
{
    Box const box = {{1.0, 2.0}, {3.0, 4.0}};
    ASSERT_EQ(1, box.get<int>(0, 0));
    ASSERT_EQ(2, box.get<int>(0, 1));
    ASSERT_EQ(3, box.get<int>(1, 0));
    ASSERT_EQ(4, box.get<int>(1, 1));

    ui64 ext[] = {0, 0, 0, 0};
    ASSERT_EQ(1, box.get<int>(Box::device_cpu(), ext, 0, 0));
    ASSERT_EQ(2, box.get<int>(Box::device_cpu(), ext, 0, 1));
    ASSERT_EQ(3, box.get<int>(Box::device_cpu(), ext, 1, 0));
    ASSERT_EQ(4, box.get<int>(Box::device_cpu(), ext, 1, 1));

    ui32 dims0[] = {0, 0};
    ui32 dims1[] = {0, 1};
    ui32 dims2[] = {1, 0};
    ui32 dims3[] = {1, 1};
    ASSERT_EQ(1, box.get<int>(2, dims0));
    ASSERT_EQ(2, box.get<int>(2, dims1));
    ASSERT_EQ(3, box.get<int>(2, dims2));
    ASSERT_EQ(4, box.get<int>(2, dims3));

    ASSERT_EQ(1, box.get<int>(Box::device_cpu(), ext, 2, dims0));
    ASSERT_EQ(2, box.get<int>(Box::device_cpu(), ext, 2, dims1));
    ASSERT_EQ(3, box.get<int>(Box::device_cpu(), ext, 2, dims2));
    ASSERT_EQ(4, box.get<int>(Box::device_cpu(), ext, 2, dims3));
}

TEST(Box_At_Test, Get_ArgumentOutput)
{
    Box const box = {{1.0, 2.0}, {3.0, 4.0}};
    int elem0, elem1, elem2, elem3;
    ASSERT_EQ(E_SUCCESS, box.get<int>(&elem0, 0, 0));
    ASSERT_EQ(E_SUCCESS, box.get<int>(&elem1, 0, 1));
    ASSERT_EQ(E_SUCCESS, box.get<int>(&elem2, 1, 0));
    ASSERT_EQ(E_SUCCESS, box.get<int>(&elem3, 1, 1));
    ASSERT_EQ(1, elem0);
    ASSERT_EQ(2, elem1);
    ASSERT_EQ(3, elem2);
    ASSERT_EQ(4, elem3);
    elem0 = 0;
    elem1 = 0;
    elem2 = 0;
    elem3 = 0;

    ui64 ext[] = {0, 0, 0, 0};
    ASSERT_EQ(E_SUCCESS, box.get<int>(&elem0, Box::device_cpu(), ext, 0, 0));
    ASSERT_EQ(E_SUCCESS, box.get<int>(&elem1, Box::device_cpu(), ext, 0, 1));
    ASSERT_EQ(E_SUCCESS, box.get<int>(&elem2, Box::device_cpu(), ext, 1, 0));
    ASSERT_EQ(E_SUCCESS, box.get<int>(&elem3, Box::device_cpu(), ext, 1, 1));
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
    ASSERT_EQ(E_SUCCESS, box.get<int>(&elem0, 2, dims0));
    ASSERT_EQ(E_SUCCESS, box.get<int>(&elem1, 2, dims1));
    ASSERT_EQ(E_SUCCESS, box.get<int>(&elem2, 2, dims2));
    ASSERT_EQ(E_SUCCESS, box.get<int>(&elem3, 2, dims3));
    ASSERT_EQ(1, elem0);
    ASSERT_EQ(2, elem1);
    ASSERT_EQ(3, elem2);
    ASSERT_EQ(4, elem3);
    elem0 = 0;
    elem1 = 0;
    elem2 = 0;
    elem3 = 0;

    ASSERT_EQ(E_SUCCESS, box.get<int>(&elem0, Box::device_cpu(), ext, 2, dims0));
    ASSERT_EQ(E_SUCCESS, box.get<int>(&elem1, Box::device_cpu(), ext, 2, dims1));
    ASSERT_EQ(E_SUCCESS, box.get<int>(&elem2, Box::device_cpu(), ext, 2, dims2));
    ASSERT_EQ(E_SUCCESS, box.get<int>(&elem3, Box::device_cpu(), ext, 2, dims3));
    ASSERT_EQ(1, elem0);
    ASSERT_EQ(2, elem1);
    ASSERT_EQ(3, elem2);
    ASSERT_EQ(4, elem3);
}

