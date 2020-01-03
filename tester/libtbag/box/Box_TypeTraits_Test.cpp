/**
 * @file   Box_TypeTraits_Test.cpp
 * @brief  Box class tester.
 * @author zer0
 * @date   2019-12-28
 */

#include <gtest/gtest.h>
#include <libtbag/box/Box.hpp>

using namespace libtbag;
using namespace libtbag::box;

TEST(Box_TypeTraits_Test, is_first_box_data)
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

TEST(Box_TypeTraits_Test, is_first_Box)
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

TEST(Box_TypeTraits_Test, is_last_va_list)
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

TEST(Box_TypeTraits_Test, is_last_ui32_ptr)
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

TEST(Box_TypeTraits_Test, is_first_bdev_and_second_ui64_ptr)
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

TEST(Box_TypeTraits_Test, shape_selector)
{
    using namespace libtbag::box::details;

    EXPECT_TRUE((std::is_same<typename shape_selector<bdev,ui64*,int>::type, shape_args1_t>::value));
    EXPECT_TRUE((std::is_same<typename shape_selector<int>::type, shape_args2_t>::value));

    EXPECT_TRUE((std::is_same<typename shape_selector<bdev&&,ui64*,ui32&&,int&&>::type, shape_args1_t>::value));
    EXPECT_TRUE((std::is_same<typename shape_selector<int&&,int&&>::type, shape_args2_t>::value));

    EXPECT_TRUE((std::is_same<typename shape_selector<bdev,ui64*,ui32,ui32*>::type, shape_dims1_t>::value));
    EXPECT_TRUE((std::is_same<typename shape_selector<ui32,ui32*>::type, shape_dims2_t>::value));

    EXPECT_TRUE((std::is_same<typename shape_selector<box_data&&>::type, shape_like1_t>::value));
    EXPECT_TRUE((std::is_same<typename shape_selector<Box&&>::type, shape_like2_t>::value));

    EXPECT_TRUE((std::is_same<typename shape_selector<Box,Box>::type, shape_error_t>::value));
}

