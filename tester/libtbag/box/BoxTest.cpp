/**
 * @file   BoxTest.cpp
 * @brief  Box class tester.
 * @author zer0
 * @date   2019-05-16
 */

#include <gtest/gtest.h>
#include <libtbag/box/Box.hpp>
#include <libtbag/util/TestUtils.hpp>

using namespace libtbag;
using namespace libtbag::box;

TEST(BoxTest, Default)
{
    Box b0;
    ASSERT_TRUE(b0);

    Box b1_move;
    ASSERT_TRUE(b1_move);
    auto const * b1_data = b1_move.base();
    Box b2 = std::move(b1_move);
    ASSERT_TRUE(b2);
    auto const * b2_data = b2.base();
    ASSERT_EQ(b1_data, b2_data);

    Box b3;
    Box b4 = b3;
    ASSERT_TRUE(b3);
    ASSERT_TRUE(b4);
    auto const * b3_data = b3.base();
    auto const * b4_data = b4.base();
    ASSERT_EQ(b3_data, b4_data);

    Box b5;
    ASSERT_TRUE(b5);
    Box b6(nullptr);
    ASSERT_FALSE(b6);
    b6 = b5;
    auto const * b5_data = b5.base();
    auto const * b6_data = b6.base();
    ASSERT_EQ(b5_data, b6_data);
}

TEST(BoxTest, GetBtype)
{
    auto const char_btype = get_btype<char>();
    auto const si8_btype = get_btype<si8>();
    ASSERT_EQ(char_btype, si8_btype);
}

TEST(BoxTest, CreateIfNotExists)
{
    Box box(nullptr);
    ASSERT_FALSE(box);
    box.createIfNotExists();
    ASSERT_TRUE(box);
}

TEST(BoxTest, is_first_box_data)
{
    EXPECT_FALSE(is_first_box_data<Box const &&>::value);
    EXPECT_FALSE(is_first_box_data<Box const &>::value);
    EXPECT_FALSE(is_first_box_data<Box const *>::value);
    EXPECT_FALSE(is_first_box_data<Box const>::value);
    EXPECT_FALSE(is_first_box_data<Box &&>::value);
    EXPECT_FALSE(is_first_box_data<Box &>::value);
    EXPECT_FALSE(is_first_box_data<Box *>::value);
    EXPECT_FALSE(is_first_box_data<Box>::value);

    using namespace libtbag::box::details;
    EXPECT_TRUE(is_first_box_data<box_data const &&>::value);
    EXPECT_TRUE(is_first_box_data<box_data const &>::value);
    EXPECT_TRUE(is_first_box_data<box_data const *>::value);
    EXPECT_TRUE(is_first_box_data<box_data const>::value);
    EXPECT_TRUE(is_first_box_data<box_data &&>::value);
    EXPECT_TRUE(is_first_box_data<box_data &>::value);
    EXPECT_TRUE(is_first_box_data<box_data *>::value);
    EXPECT_TRUE(is_first_box_data<box_data>::value);

    EXPECT_TRUE((is_first_box_data<box_data,int>::value));
    EXPECT_TRUE((is_first_box_data<box_data,int,double>::value));

    EXPECT_FALSE((is_first_box_data<int,box_data>::value));
    EXPECT_FALSE((is_first_box_data<double,int,box_data>::value));
}

TEST(BoxTest, is_first_Box)
{
    EXPECT_TRUE(is_first_Box<Box const &&>::value);
    EXPECT_TRUE(is_first_Box<Box const &>::value);
    EXPECT_TRUE(is_first_Box<Box const *>::value);
    EXPECT_TRUE(is_first_Box<Box const>::value);
    EXPECT_TRUE(is_first_Box<Box &>::value);
    EXPECT_TRUE(is_first_Box<Box *>::value);
    EXPECT_TRUE(is_first_Box<Box>::value);

    using namespace libtbag::box::details;
    EXPECT_FALSE(is_first_Box<box_data const &&>::value);
    EXPECT_FALSE(is_first_Box<box_data const &>::value);
    EXPECT_FALSE(is_first_Box<box_data const *>::value);
    EXPECT_FALSE(is_first_Box<box_data const>::value);
    EXPECT_FALSE(is_first_Box<box_data &>::value);
    EXPECT_FALSE(is_first_Box<box_data *>::value);
    EXPECT_FALSE(is_first_Box<box_data>::value);

    EXPECT_TRUE((is_first_Box<Box,int>::value));
    EXPECT_TRUE((is_first_Box<Box,int,double>::value));

    EXPECT_FALSE((is_first_Box<int,Box>::value));
    EXPECT_FALSE((is_first_Box<double,int,Box>::value));
}

TEST(BoxTest, is_last_va_list)
{
    EXPECT_TRUE(is_last_va_list<va_list const &&>::value);
    EXPECT_TRUE(is_last_va_list<va_list const &>::value);
    EXPECT_TRUE(is_last_va_list<va_list const>::value);
    EXPECT_TRUE(is_last_va_list<va_list &&>::value);
    EXPECT_TRUE(is_last_va_list<va_list &>::value);
    EXPECT_TRUE(is_last_va_list<va_list>::value);

    EXPECT_FALSE(is_last_va_list<va_list const *>::value);
    EXPECT_FALSE(is_last_va_list<va_list *>::value);

    EXPECT_FALSE(is_last_va_list<int const &&>::value);
    EXPECT_FALSE(is_last_va_list<int const &>::value);
    EXPECT_FALSE(is_last_va_list<int const *>::value);
    EXPECT_FALSE(is_last_va_list<int const>::value);
    EXPECT_FALSE(is_last_va_list<int &&>::value);
    EXPECT_FALSE(is_last_va_list<int &>::value);
    EXPECT_FALSE(is_last_va_list<int *>::value);
    EXPECT_FALSE(is_last_va_list<int>::value);

    EXPECT_TRUE((is_last_va_list<int,va_list>::value));
    EXPECT_TRUE((is_last_va_list<double,int,va_list>::value));

    EXPECT_FALSE((is_last_va_list<va_list,int>::value));
    EXPECT_FALSE((is_last_va_list<va_list,int,double>::value));
}

TEST(BoxTest, is_last_ui32_ptr)
{
    using namespace libtbag::box::details;
    EXPECT_FALSE(is_last_ui32_ptr<ui32 const &&>::value);
    EXPECT_FALSE(is_last_ui32_ptr<ui32 const &>::value);
    EXPECT_FALSE(is_last_ui32_ptr<ui32 const>::value);
    EXPECT_FALSE(is_last_ui32_ptr<ui32 &&>::value);
    EXPECT_FALSE(is_last_ui32_ptr<ui32 &>::value);
    EXPECT_FALSE(is_last_ui32_ptr<ui32>::value);

    EXPECT_TRUE(is_last_ui32_ptr<ui32 const *>::value);
    EXPECT_TRUE(is_last_ui32_ptr<ui32 *>::value);

    EXPECT_FALSE(is_last_ui32_ptr<int const &&>::value);
    EXPECT_FALSE(is_last_ui32_ptr<int const &>::value);
    EXPECT_FALSE(is_last_ui32_ptr<int const *>::value);
    EXPECT_FALSE(is_last_ui32_ptr<int const>::value);
    EXPECT_FALSE(is_last_ui32_ptr<int &&>::value);
    EXPECT_FALSE(is_last_ui32_ptr<int &>::value);
    EXPECT_FALSE(is_last_ui32_ptr<int *>::value);
    EXPECT_FALSE(is_last_ui32_ptr<int>::value);

    EXPECT_TRUE((is_last_ui32_ptr<int,ui32*>::value));
    EXPECT_TRUE((is_last_ui32_ptr<double,int,ui32*>::value));

    EXPECT_FALSE((is_last_ui32_ptr<ui32*,int>::value));
    EXPECT_FALSE((is_last_ui32_ptr<ui32*,int,double>::value));
}

TEST(BoxTest, is_first_bdev_and_second_ui64_ptr)
{
    using namespace libtbag::box::details;
    EXPECT_FALSE(is_first_bdev_and_second_ui64_ptr<ui64 const &&>::value);
    EXPECT_FALSE(is_first_bdev_and_second_ui64_ptr<ui64 const &>::value);
    EXPECT_FALSE(is_first_bdev_and_second_ui64_ptr<ui64 const *>::value);
    EXPECT_FALSE(is_first_bdev_and_second_ui64_ptr<ui64 const>::value);
    EXPECT_FALSE(is_first_bdev_and_second_ui64_ptr<ui64 &&>::value);
    EXPECT_FALSE(is_first_bdev_and_second_ui64_ptr<ui64 &>::value);
    EXPECT_FALSE(is_first_bdev_and_second_ui64_ptr<ui64 *>::value);
    EXPECT_FALSE(is_first_bdev_and_second_ui64_ptr<ui64>::value);

    EXPECT_FALSE(is_first_bdev_and_second_ui64_ptr<bdev const &&>::value);
    EXPECT_FALSE(is_first_bdev_and_second_ui64_ptr<bdev const &>::value);
    EXPECT_FALSE(is_first_bdev_and_second_ui64_ptr<bdev const *>::value);
    EXPECT_FALSE(is_first_bdev_and_second_ui64_ptr<bdev const>::value);
    EXPECT_FALSE(is_first_bdev_and_second_ui64_ptr<bdev &&>::value);
    EXPECT_FALSE(is_first_bdev_and_second_ui64_ptr<bdev &>::value);
    EXPECT_FALSE(is_first_bdev_and_second_ui64_ptr<bdev *>::value);
    EXPECT_FALSE(is_first_bdev_and_second_ui64_ptr<bdev>::value);

    EXPECT_FALSE((is_first_bdev_and_second_ui64_ptr<bdev,ui64 const &&>::value));
    EXPECT_FALSE((is_first_bdev_and_second_ui64_ptr<bdev,ui64 const &>::value));
    EXPECT_FALSE((is_first_bdev_and_second_ui64_ptr<bdev,ui64 const>::value));
    EXPECT_FALSE((is_first_bdev_and_second_ui64_ptr<bdev,ui64 &&>::value));
    EXPECT_FALSE((is_first_bdev_and_second_ui64_ptr<bdev,ui64 &>::value));
    EXPECT_FALSE((is_first_bdev_and_second_ui64_ptr<bdev,ui64>::value));

    EXPECT_TRUE((is_first_bdev_and_second_ui64_ptr<bdev,ui64 const *>::value));
    EXPECT_TRUE((is_first_bdev_and_second_ui64_ptr<bdev,ui64 *>::value));
    EXPECT_TRUE((is_first_bdev_and_second_ui64_ptr<bdev,ui64 const *,int>::value));
    EXPECT_TRUE((is_first_bdev_and_second_ui64_ptr<bdev,ui64 *,int>::value));

    EXPECT_FALSE((is_first_bdev_and_second_ui64_ptr<int,bdev,ui64 const *>::value));
    EXPECT_FALSE((is_first_bdev_and_second_ui64_ptr<int,bdev,ui64 *>::value));
}

