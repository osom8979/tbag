/**
 * @file   IteratorBypassTest.cpp
 * @brief  IteratorBypass class tester.
 * @author zer0
 * @date   2018-07-15
 * @date   2018-07-31 (Rename: StdContainerFacadeTest -> IteratorBypassTest)
 * @date   2018-08-05 (Move directory: libtbag/iterator/facade -> libtbag/iterator)
 */

#include <gtest/gtest.h>
#include <libtbag/iterator/IteratorBypass.hpp>

#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

using namespace libtbag;
using namespace libtbag::iterator;

TEST(IteratorBypassTest, HasReverseIterator)
{
    using TestVector       = std::vector<char>;
    using TestMap          = std::map<int, char>;
    using TestSet          = std::set<int>;
    using TestUnorderedMap = std::unordered_map<int, char>;
    using TestUnorderedSet = std::unordered_set<int>;

    ASSERT_TRUE (HasReverseIterator<TestVector>      ::value);
    ASSERT_TRUE (HasReverseIterator<TestMap>         ::value);
    ASSERT_TRUE (HasReverseIterator<TestSet>         ::value);
    ASSERT_FALSE(HasReverseIterator<TestUnorderedMap>::value);
    ASSERT_FALSE(HasReverseIterator<TestUnorderedSet>::value);
}

struct BypassTestVector
{
    using VectorType = std::vector<int>;
    VectorType temp;
    TBAG_ITERATOR_VECTOR_BYPASS(VectorType, temp);
};

TEST(IteratorBypassTest, VectorCoverage)
{
    BypassTestVector v;
    v.temp.resize(1);
    v.max_size();
    v.size();
    v.empty();
    v[0];
    v.at(0);
    v.begin();
    v.end();
    v.cbegin();
    v.cend();
    v.rbegin();
    v.rend();
    v.crbegin();
    v.crend();
    v.clear();
}

struct BypassTestSet
{
    using SetType = std::set<int>;
    SetType temp;
    TBAG_ITERATOR_SET_BYPASS(SetType, temp);
};

TEST(IteratorBypassTest, SetCoverage)
{
    BypassTestSet v;
    v.temp.insert(0);

    v.max_size();
    v.size();
    v.empty();
    v.begin();
    v.end();
    v.cbegin();
    v.cend();
    v.clear();
}

struct BypassTestMap
{
    using MapType = std::map<int, int>;
    MapType temp;
    TBAG_ITERATOR_MAP_BYPASS(MapType, temp);
};

TEST(IteratorBypassTest, MapCoverage)
{
    BypassTestMap v;
    v.temp.insert(std::make_pair(0, 0));

    v.max_size();
    v.size();
    v.empty();
    v.begin();
    v.end();
    v.cbegin();
    v.cend();
    v.clear();
}

