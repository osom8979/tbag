/**
 * @file   StdContainerFacadeTest.cpp
 * @brief  StdContainerFacade class tester.
 * @author zer0
 * @date   2018-07-15
 */

#include <gtest/gtest.h>
#include <libtbag/pattern/StdContainerFacade.hpp>

#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

using namespace libtbag;
using namespace libtbag::pattern;

using TestVector       = std::vector<char>;
using TestMap          = std::map<int, char>;
using TestSet          = std::set<int>;
using TestUnorderedMap = std::unordered_map<int, char>;
using TestUnorderedSet = std::unordered_set<int>;

TEST(StdContainerFacadeTest, HasStdReverseIterator)
{
    ASSERT_TRUE (HasStdReverseIterator<TestVector>      ::value);
    ASSERT_TRUE (HasStdReverseIterator<TestMap>         ::value);
    ASSERT_TRUE (HasStdReverseIterator<TestSet>         ::value);
    ASSERT_FALSE(HasStdReverseIterator<TestUnorderedMap>::value);
    ASSERT_FALSE(HasStdReverseIterator<TestUnorderedSet>::value);

    ASSERT_TRUE (StdContainerFacade<TestVector>      ::has_reverse_iterator);
    ASSERT_TRUE (StdContainerFacade<TestMap>         ::has_reverse_iterator);
    ASSERT_TRUE (StdContainerFacade<TestSet>         ::has_reverse_iterator);
    ASSERT_FALSE(StdContainerFacade<TestUnorderedMap>::has_reverse_iterator);
    ASSERT_FALSE(StdContainerFacade<TestUnorderedSet>::has_reverse_iterator);
}

template <typename T>
static void __coverage_only_iterator(T const & c)
{
    c.size();
    c.max_size();
    c.empty();

    c.begin();
    c.end();

    c.cbegin();
    c.cend();
}

template <typename T>
static void __coverage_only_reverse_iterator(T const & c)
{
    c.rbegin();
    c.rend();

    c.crbegin();
    c.crend();
}

TEST(StdContainerFacadeTest, Coverage)
{
    TestVector       c1;
    TestMap          c2;
    TestSet          c3;
    TestUnorderedMap c4;
    TestUnorderedSet c5;

    StdContainerFacade<TestVector>       temp1(c1);
    StdContainerFacade<TestMap>          temp2(c2);
    StdContainerFacade<TestSet>          temp3(c3);
    StdContainerFacade<TestUnorderedMap> temp4(c4);
    StdContainerFacade<TestUnorderedSet> temp5(c5);

    __coverage_only_reverse_iterator(temp1);
    __coverage_only_reverse_iterator(temp2);
    __coverage_only_reverse_iterator(temp3);

    __coverage_only_iterator(temp1);
    __coverage_only_iterator(temp2);
    __coverage_only_iterator(temp3);
    __coverage_only_iterator(temp4);
    __coverage_only_iterator(temp5);
}

