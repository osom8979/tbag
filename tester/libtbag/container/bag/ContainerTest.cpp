/**
 * @file   ContainerTest.cpp
 * @brief  Container class tester.
 * @author zer0
 * @date   2018-07-28
 */

#include <gtest/gtest.h>
#include <libtbag/container/bag/Container.hpp>
#include <libtbag/memory/TestAllocator.hpp>
#include <ostream>

using namespace libtbag;
using namespace libtbag::container;
using namespace libtbag::container::bag;

struct ContainerElementTest
{
    static int constructor;
    static int destructor;

    static int copy_constructor;
    static int move_constructor;

    static int copy;
    static int move;

    int value = 0;

    ContainerElementTest()
    {
        constructor++;
    }

    ContainerElementTest(ContainerElementTest const & obj) : value(obj.value)
    {
        copy_constructor++;
    }

    ContainerElementTest(ContainerElementTest && obj) TBAG_NOEXCEPT : value(obj.value)
    {
        move_constructor++;
    }

    ~ContainerElementTest()
    {
        destructor++;
    }

    ContainerElementTest & operator =(ContainerElementTest const & obj)
    {
        copy++;
        if (this != &obj) {
            value = obj.value;
        }
        return *this;
    }

    ContainerElementTest & operator =(ContainerElementTest && obj) TBAG_NOEXCEPT
    {
        move++;
        if (this != &obj) {
            std::swap(value, obj.value);
        }
        return *this;
    }

    ContainerElementTest & operator =(int v)
    {
        value = v;
        return *this;
    }

    operator int() const
    {
        return value;
    }

    friend void swap(ContainerElementTest & lh, ContainerElementTest & rh) TBAG_NOEXCEPT
    {
        lh = std::move(rh);
    }
};

using ElementAllocator = libtbag::memory::TestAllocator<ContainerElementTest>;

int ContainerElementTest::constructor = 0;
int ContainerElementTest::destructor  = 0;
int ContainerElementTest::copy_constructor = 0;
int ContainerElementTest::move_constructor = 0;
int ContainerElementTest::copy = 0;
int ContainerElementTest::move = 0;

static void __clear_container_element() TBAG_NOEXCEPT
{
    ContainerElementTest::constructor = 0;
    ContainerElementTest::destructor  = 0;
    ContainerElementTest::copy_constructor = 0;
    ContainerElementTest::move_constructor = 0;
    ContainerElementTest::copy = 0;
    ContainerElementTest::move = 0;
}

TEST(ContainerTest, Default)
{
    __clear_container_element();
    Container<ContainerElementTest, ElementAllocator> con;
    ASSERT_EQ(nullptr, con.data());
    ASSERT_EQ(0, con.size());
    ASSERT_TRUE(con.empty());
    ASSERT_NE(0, con.max_size());
    ASSERT_EQ(0, con.capacity());

    ASSERT_EQ(0, con.allocator().allocate_count);
    ASSERT_EQ(0, con.allocator().deallocate_count);
    ASSERT_EQ(0, con.allocator().allocate_elem_size);
    ASSERT_EQ(0, con.allocator().deallocate_elem_size);
    ASSERT_EQ(0, con.allocator().construct_count);
    ASSERT_EQ(0, con.allocator().destroy_count);

    ASSERT_EQ(0, ContainerElementTest::constructor);
    ASSERT_EQ(0, ContainerElementTest::destructor);
    ASSERT_EQ(0, ContainerElementTest::copy_constructor);
    ASSERT_EQ(0, ContainerElementTest::move_constructor);
    ASSERT_EQ(0, ContainerElementTest::copy);
    ASSERT_EQ(0, ContainerElementTest::move);
}

TEST(ContainerTest, Copy)
{
    Container<ContainerElementTest> con1;
    con1.resize(2);
    ASSERT_EQ(2, con1.capacity());
    ASSERT_EQ(2, con1.size());
    int i = 0;
    for (auto & elem : con1) {
        elem = i++;
    }
    ASSERT_EQ(2, con1.size());
    ASSERT_EQ(0, con1[0]);
    ASSERT_EQ(1, con1[1]);

    Container<ContainerElementTest> con2;
    con2.copy(con1);
    ASSERT_NE(nullptr, con2.data());
    ASSERT_EQ(2, con2.capacity());
    ASSERT_EQ(2, con2.size());
    ASSERT_EQ(0, con2[0]);
    ASSERT_EQ(1, con2[1]);
}

TEST(ContainerTest, Constructor)
{
    Container<ContainerElementTest> con1(8);
    ASSERT_EQ(8, con1.capacity());
    ASSERT_TRUE(con1.empty());

    con1.resize(2);
    ASSERT_EQ(8, con1.capacity());
    ASSERT_EQ(2, con1.size());
    int i = 0;
    for (auto & elem : con1) {
        elem = i++;
    }
    ASSERT_EQ(2, con1.size());
    ASSERT_EQ(0, con1[0]);
    ASSERT_EQ(1, con1[1]);

    Container<ContainerElementTest> con2 = con1;
    ASSERT_NE(con1.data(), con2.data());
    ASSERT_NE(nullptr, con1.data());
    ASSERT_NE(nullptr, con2.data());
    ASSERT_EQ(2, con1.size());
    ASSERT_EQ(0, con1[0]);
    ASSERT_EQ(1, con1[1]);
    ASSERT_EQ(2, con2.size());
    ASSERT_EQ(0, con2[0]);
    ASSERT_EQ(1, con2[1]);

    Container<ContainerElementTest> con3 = std::move(con2);
    ASSERT_EQ(nullptr, con2.data());
    ASSERT_EQ(0, con2.size());
    ASSERT_EQ(0, con2.capacity());
    ASSERT_EQ(2, con3.size());
    ASSERT_EQ(0, con3[0]);
    ASSERT_EQ(1, con3[1]);
}

TEST(ContainerTest, Coverage)
{
    Container<int> con(1);
    // @formatter:off
    con.  begin();
    con.    end();
    con. rbegin();
    con.   rend();
    con. cbegin();
    con.   cend();
    con.crbegin();
    con.  crend();
    con.  front();
    con.   back();
    // @formatter:on

    Container<int> const const_con(1);
    // @formatter:off
    const_con.  begin();
    const_con.    end();
    const_con. rbegin();
    const_con.   rend();
    const_con. cbegin();
    const_con.   cend();
    const_con.crbegin();
    const_con.  crend();
    const_con.  front();
    const_con.   back();
    // @formatter:on
}

TEST(ContainerTest, ClearUtility)
{
    __clear_container_element();
    Container<ContainerElementTest, ElementAllocator> con;
    ASSERT_EQ(nullptr, con.data());
    ASSERT_EQ(0, con.size());
    ASSERT_EQ(0, con.capacity());

    con.reserve(2);
    ASSERT_NE(nullptr, con.data());
    ASSERT_EQ(0, con.size());
    ASSERT_EQ(2, con.capacity());
    ASSERT_EQ(1, con.allocator().allocate_count);
    ASSERT_EQ(0, con.allocator().deallocate_count);
    ASSERT_EQ(2, con.allocator().allocate_elem_size);
    ASSERT_EQ(0, con.allocator().deallocate_elem_size);
    ASSERT_EQ(0, con.allocator().construct_count);
    ASSERT_EQ(0, con.allocator().destroy_count);
    ASSERT_EQ(0, ContainerElementTest::constructor);
    ASSERT_EQ(0, ContainerElementTest::destructor);
    ASSERT_EQ(0, ContainerElementTest::copy_constructor);
    ASSERT_EQ(0, ContainerElementTest::move_constructor);
    ASSERT_EQ(0, ContainerElementTest::copy);
    ASSERT_EQ(0, ContainerElementTest::move);

    __clear_container_element();
    con.allocator().clear();
    ASSERT_EQ(0, con.allocator().allocate_count);
    ASSERT_EQ(0, con.allocator().deallocate_count);
    ASSERT_EQ(0, con.allocator().allocate_elem_size);
    ASSERT_EQ(0, con.allocator().deallocate_elem_size);
    ASSERT_EQ(0, con.allocator().construct_count);
    ASSERT_EQ(0, con.allocator().destroy_count);
    ASSERT_EQ(0, ContainerElementTest::constructor);
    ASSERT_EQ(0, ContainerElementTest::destructor);
    ASSERT_EQ(0, ContainerElementTest::copy_constructor);
    ASSERT_EQ(0, ContainerElementTest::move_constructor);
    ASSERT_EQ(0, ContainerElementTest::copy);
    ASSERT_EQ(0, ContainerElementTest::move);
}

TEST(ContainerTest, Reserve)
{
    __clear_container_element();
    Container<ContainerElementTest, ElementAllocator> con;
    ASSERT_EQ(nullptr, con.data());
    ASSERT_EQ(0, con.size());
    ASSERT_EQ(0, con.capacity());

    con.reserve(2);
    ASSERT_NE(nullptr, con.data());
    ASSERT_EQ(0, con.size());
    ASSERT_EQ(2, con.capacity());

    __clear_container_element();
    con.allocator().clear();
    con.resize(1);
    ASSERT_NE(nullptr, con.data());
    ASSERT_EQ(1, con.size());
    ASSERT_EQ(2, con.capacity());

    ASSERT_EQ(0, con.allocator().allocate_count);
    ASSERT_EQ(0, con.allocator().deallocate_count);
    ASSERT_EQ(0, con.allocator().allocate_elem_size);
    ASSERT_EQ(0, con.allocator().deallocate_elem_size);
    ASSERT_EQ(1, con.allocator().construct_count);
    ASSERT_EQ(0, con.allocator().destroy_count);
    ASSERT_EQ(1, ContainerElementTest::constructor);
    ASSERT_EQ(0, ContainerElementTest::destructor);
    ASSERT_EQ(0, ContainerElementTest::copy_constructor);
    ASSERT_EQ(0, ContainerElementTest::move_constructor);
    ASSERT_EQ(0, ContainerElementTest::copy);
    ASSERT_EQ(0, ContainerElementTest::move);
}

TEST(ContainerTest, Reserve_Minus)
{
    __clear_container_element();
    Container<ContainerElementTest, ElementAllocator> con;
    con.reserve(2);
    con.resize(1);

    __clear_container_element();
    con.allocator().clear();
    con.reserve(1);
    ASSERT_NE(nullptr, con.data());
    ASSERT_EQ(1, con.size());
    ASSERT_EQ(2, con.capacity());

    ASSERT_EQ(0, con.allocator().allocate_count);
    ASSERT_EQ(0, con.allocator().deallocate_count);
    ASSERT_EQ(0, con.allocator().allocate_elem_size);
    ASSERT_EQ(0, con.allocator().deallocate_elem_size);
    ASSERT_EQ(0, con.allocator().construct_count);
    ASSERT_EQ(0, con.allocator().destroy_count);
    ASSERT_EQ(0, ContainerElementTest::constructor);
    ASSERT_EQ(0, ContainerElementTest::destructor);
    ASSERT_EQ(0, ContainerElementTest::copy_constructor);
    ASSERT_EQ(0, ContainerElementTest::move_constructor);
    ASSERT_EQ(0, ContainerElementTest::copy);
    ASSERT_EQ(0, ContainerElementTest::move);

    __clear_container_element();
    con.allocator().clear();
    con.resize(0);
    ASSERT_NE(nullptr, con.data());
    ASSERT_EQ(0, con.size());
    ASSERT_EQ(2, con.capacity());

    ASSERT_EQ(0, con.allocator().allocate_count);
    ASSERT_EQ(0, con.allocator().deallocate_count);
    ASSERT_EQ(0, con.allocator().allocate_elem_size);
    ASSERT_EQ(0, con.allocator().deallocate_elem_size);
    ASSERT_EQ(0, con.allocator().construct_count);
    ASSERT_EQ(1, con.allocator().destroy_count);
    ASSERT_EQ(0, ContainerElementTest::constructor);
    ASSERT_EQ(1, ContainerElementTest::destructor);
    ASSERT_EQ(0, ContainerElementTest::copy_constructor);
    ASSERT_EQ(0, ContainerElementTest::move_constructor);
    ASSERT_EQ(0, ContainerElementTest::copy);
    ASSERT_EQ(0, ContainerElementTest::move);
}

TEST(ContainerTest, Reserve_Plus)
{
    __clear_container_element();
    Container<ContainerElementTest, ElementAllocator> con;
    con.reserve(2);
    con.resize(0);

    __clear_container_element();
    con.allocator().clear();
    con.reserve(5);
    ASSERT_NE(nullptr, con.data());
    ASSERT_EQ(0, con.size());
    ASSERT_EQ(5, con.capacity());

    ASSERT_EQ(1, con.allocator().allocate_count);
    ASSERT_EQ(1, con.allocator().deallocate_count);
    ASSERT_EQ(5, con.allocator().allocate_elem_size);
    ASSERT_EQ(2, con.allocator().deallocate_elem_size);
    ASSERT_EQ(0, con.allocator().construct_count);
    ASSERT_EQ(0, con.allocator().destroy_count);
    ASSERT_EQ(0, ContainerElementTest::constructor);
    ASSERT_EQ(0, ContainerElementTest::destructor);
    ASSERT_EQ(0, ContainerElementTest::copy_constructor);
    ASSERT_EQ(0, ContainerElementTest::move_constructor);
    ASSERT_EQ(0, ContainerElementTest::copy);
    ASSERT_EQ(0, ContainerElementTest::move);
}

TEST(ContainerTest, Resize)
{
    __clear_container_element();
    Container<ContainerElementTest, ElementAllocator> con;
    ASSERT_EQ(nullptr, con.data());
    ASSERT_EQ(0, con.size());
    ASSERT_EQ(0, con.capacity());

    con.resize(2);
    ASSERT_NE(nullptr, con.data());
    ASSERT_EQ(2, con.size());
    ASSERT_EQ(2, con.capacity());
    ASSERT_EQ(1, con.allocator().allocate_count);
    ASSERT_EQ(0, con.allocator().deallocate_count);
    ASSERT_EQ(2, con.allocator().allocate_elem_size);
    ASSERT_EQ(0, con.allocator().deallocate_elem_size);
    ASSERT_EQ(2, con.allocator().construct_count);
    ASSERT_EQ(0, con.allocator().destroy_count);
    ASSERT_EQ(2, ContainerElementTest::constructor);
    ASSERT_EQ(0, ContainerElementTest::destructor);
    ASSERT_EQ(0, ContainerElementTest::copy_constructor);
    ASSERT_EQ(0, ContainerElementTest::move_constructor);
    ASSERT_EQ(0, ContainerElementTest::copy);
    ASSERT_EQ(0, ContainerElementTest::move);

    __clear_container_element();
    con.allocator().clear();
    ASSERT_NE(nullptr, con.data());
    ASSERT_EQ(2, con.size());
    ASSERT_EQ(2, con.capacity());
    ASSERT_EQ(0, con.allocator().allocate_count);
    ASSERT_EQ(0, con.allocator().deallocate_count);
    ASSERT_EQ(0, con.allocator().allocate_elem_size);
    ASSERT_EQ(0, con.allocator().deallocate_elem_size);
    ASSERT_EQ(0, con.allocator().construct_count);
    ASSERT_EQ(0, con.allocator().destroy_count);
    ASSERT_EQ(0, ContainerElementTest::constructor);
    ASSERT_EQ(0, ContainerElementTest::destructor);
    ASSERT_EQ(0, ContainerElementTest::copy_constructor);
    ASSERT_EQ(0, ContainerElementTest::move_constructor);
    ASSERT_EQ(0, ContainerElementTest::copy);
    ASSERT_EQ(0, ContainerElementTest::move);
}

TEST(ContainerTest, Resize_0)
{
    __clear_container_element();
    Container<ContainerElementTest, ElementAllocator> con;
    con.resize(2);

    __clear_container_element();
    con.allocator().clear();
    con.resize(0);
    ASSERT_NE(nullptr, con.data());
    ASSERT_EQ(0, con.size());
    ASSERT_EQ(2, con.capacity());
    ASSERT_EQ(0, con.allocator().allocate_count);
    ASSERT_EQ(0, con.allocator().deallocate_count);
    ASSERT_EQ(0, con.allocator().allocate_elem_size);
    ASSERT_EQ(0, con.allocator().deallocate_elem_size);
    ASSERT_EQ(0, con.allocator().construct_count);
    ASSERT_EQ(2, con.allocator().destroy_count);
    ASSERT_EQ(0, ContainerElementTest::constructor);
    ASSERT_EQ(2, ContainerElementTest::destructor);
    ASSERT_EQ(0, ContainerElementTest::copy_constructor);
    ASSERT_EQ(0, ContainerElementTest::move_constructor);
    ASSERT_EQ(0, ContainerElementTest::copy);
    ASSERT_EQ(0, ContainerElementTest::move);
}

TEST(ContainerTest, Resize_MinusPlus)
{
    __clear_container_element();
    Container<ContainerElementTest, ElementAllocator> con;
    con.resize(2);
    con.resize(1);

    __clear_container_element();
    con.allocator().clear();
    con.resize(2);
    ASSERT_NE(nullptr, con.data());
    ASSERT_EQ(2, con.size());
    ASSERT_EQ(2, con.capacity());
    ASSERT_EQ(0, con.allocator().allocate_count);
    ASSERT_EQ(0, con.allocator().deallocate_count);
    ASSERT_EQ(0, con.allocator().allocate_elem_size);
    ASSERT_EQ(0, con.allocator().deallocate_elem_size);
    ASSERT_EQ(1, con.allocator().construct_count);
    ASSERT_EQ(0, con.allocator().destroy_count);
    ASSERT_EQ(1, ContainerElementTest::constructor);
    ASSERT_EQ(0, ContainerElementTest::destructor);
    ASSERT_EQ(0, ContainerElementTest::copy_constructor);
    ASSERT_EQ(0, ContainerElementTest::move_constructor);
    ASSERT_EQ(0, ContainerElementTest::copy);
    ASSERT_EQ(0, ContainerElementTest::move);

    __clear_container_element();
    con.allocator().clear();
    con.resize(3);
    ASSERT_NE(nullptr, con.data());
    ASSERT_EQ(3, con.size());
    ASSERT_EQ(3, con.capacity());
    ASSERT_EQ(1, con.allocator().allocate_count);
    ASSERT_EQ(1, con.allocator().deallocate_count);
    ASSERT_EQ(3, con.allocator().allocate_elem_size);
    ASSERT_EQ(2, con.allocator().deallocate_elem_size);
    ASSERT_EQ(1, con.allocator().construct_count);
    ASSERT_EQ(0, con.allocator().destroy_count);
    ASSERT_EQ(1, ContainerElementTest::constructor);
    ASSERT_EQ(0, ContainerElementTest::destructor);
    ASSERT_EQ(0, ContainerElementTest::copy_constructor);
    ASSERT_EQ(0, ContainerElementTest::move_constructor);
    ASSERT_EQ(0, ContainerElementTest::copy);
    ASSERT_EQ(2, ContainerElementTest::move);

    __clear_container_element();
    con.allocator().clear();
    con.resize(2);
    ASSERT_NE(nullptr, con.data());
    ASSERT_EQ(2, con.size());
    ASSERT_EQ(3, con.capacity());
    ASSERT_EQ(0, con.allocator().allocate_count);
    ASSERT_EQ(0, con.allocator().deallocate_count);
    ASSERT_EQ(0, con.allocator().allocate_elem_size);
    ASSERT_EQ(0, con.allocator().deallocate_elem_size);
    ASSERT_EQ(0, con.allocator().construct_count);
    ASSERT_EQ(1, con.allocator().destroy_count);
    ASSERT_EQ(0, ContainerElementTest::constructor);
    ASSERT_EQ(1, ContainerElementTest::destructor);
    ASSERT_EQ(0, ContainerElementTest::copy_constructor);
    ASSERT_EQ(0, ContainerElementTest::move_constructor);
    ASSERT_EQ(0, ContainerElementTest::copy);
    ASSERT_EQ(0, ContainerElementTest::move);

    __clear_container_element();
    con.allocator().clear();
    con.resize(4);
    ASSERT_NE(nullptr, con.data());
    ASSERT_EQ(4, con.size());
    ASSERT_EQ(4, con.capacity());
    ASSERT_EQ(1, con.allocator().allocate_count);
    ASSERT_EQ(1, con.allocator().deallocate_count);
    ASSERT_EQ(4, con.allocator().allocate_elem_size);
    ASSERT_EQ(3, con.allocator().deallocate_elem_size);
    ASSERT_EQ(2, con.allocator().construct_count);
    ASSERT_EQ(0, con.allocator().destroy_count);
    ASSERT_EQ(2, ContainerElementTest::constructor);
    ASSERT_EQ(0, ContainerElementTest::destructor);
    ASSERT_EQ(0, ContainerElementTest::copy_constructor);
    ASSERT_EQ(0, ContainerElementTest::move_constructor);
    ASSERT_EQ(0, ContainerElementTest::copy);
    ASSERT_EQ(2, ContainerElementTest::move);
}

TEST(ContainerTest, Clear)
{
    __clear_container_element();
    Container<ContainerElementTest, ElementAllocator> con;
    con.resize(4);

    __clear_container_element();
    con.allocator().clear();
    con.clear();
    ASSERT_NE(nullptr, con.data());
    ASSERT_EQ(0, con.size());
    ASSERT_EQ(4, con.capacity());
    ASSERT_EQ(0, con.allocator().allocate_count);
    ASSERT_EQ(0, con.allocator().deallocate_count);
    ASSERT_EQ(0, con.allocator().allocate_elem_size);
    ASSERT_EQ(0, con.allocator().deallocate_elem_size);
    ASSERT_EQ(0, con.allocator().construct_count);
    ASSERT_EQ(4, con.allocator().destroy_count);
    ASSERT_EQ(0, ContainerElementTest::constructor);
    ASSERT_EQ(4, ContainerElementTest::destructor);
    ASSERT_EQ(0, ContainerElementTest::copy_constructor);
    ASSERT_EQ(0, ContainerElementTest::move_constructor);
    ASSERT_EQ(0, ContainerElementTest::copy);
    ASSERT_EQ(0, ContainerElementTest::move);
}

