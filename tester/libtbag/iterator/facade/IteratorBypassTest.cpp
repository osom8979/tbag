/**
 * @file   IteratorBypassTest.cpp
 * @brief  IteratorBypass class tester.
 * @author zer0
 * @date   2018-07-15
 * @date   2018-07-31 (Rename: StdContainerFacadeTest -> IteratorBypassTest)
 */

#include <gtest/gtest.h>
#include <libtbag/iterator/facade/IteratorBypass.hpp>

#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

using namespace libtbag;
using namespace libtbag::iterator;
using namespace libtbag::iterator::facade;

using TestVector       = std::vector<char>;
using TestMap          = std::map<int, char>;
using TestSet          = std::set<int>;
using TestUnorderedMap = std::unordered_map<int, char>;
using TestUnorderedSet = std::unordered_set<int>;

TEST(IteratorBypassTest, HasReverseIterator)
{
    ASSERT_TRUE (HasReverseIterator<TestVector>      ::value);
    ASSERT_TRUE (HasReverseIterator<TestMap>         ::value);
    ASSERT_TRUE (HasReverseIterator<TestSet>         ::value);
    ASSERT_FALSE(HasReverseIterator<TestUnorderedMap>::value);
    ASSERT_FALSE(HasReverseIterator<TestUnorderedSet>::value);

    ASSERT_TRUE (IteratorBypass<TestVector>      ::has_reverse_iterator);
    ASSERT_TRUE (IteratorBypass<TestMap>         ::has_reverse_iterator);
    ASSERT_TRUE (IteratorBypass<TestSet>         ::has_reverse_iterator);
    ASSERT_FALSE(IteratorBypass<TestUnorderedMap>::has_reverse_iterator);
    ASSERT_FALSE(IteratorBypass<TestUnorderedSet>::has_reverse_iterator);
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

TEST(IteratorBypassTest, Coverage)
{
    TestVector       c1;
    TestMap          c2;
    TestSet          c3;
    TestUnorderedMap c4;
    TestUnorderedSet c5;

    IteratorBypass<TestVector>       temp1(c1);
    IteratorBypass<TestMap>          temp2(c2);
    IteratorBypass<TestSet>          temp3(c3);
    IteratorBypass<TestUnorderedMap> temp4(c4);
    IteratorBypass<TestUnorderedSet> temp5(c5);

    __coverage_only_reverse_iterator(temp1);
    __coverage_only_reverse_iterator(temp2);
    __coverage_only_reverse_iterator(temp3);

    __coverage_only_iterator(temp1);
    __coverage_only_iterator(temp2);
    __coverage_only_iterator(temp3);
    __coverage_only_iterator(temp4);
    __coverage_only_iterator(temp5);
}

