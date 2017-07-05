/**
 * @file   CachedBuffersTest.cpp
 * @brief  CachedBuffers class tester.
 * @author zer0
 * @date   2017-07-05
 */

#include <gtest/gtest.h>
#include <libtbag/container/CachedBuffers.hpp>
#include <sstream>

using namespace libtbag;
using namespace libtbag::container;

using Buffers = CachedBuffers<char>;
using binf    = Buffers::binf;
using binfs   = Buffers::binfs;

TEST(CachedBuffersTest, ConstCachedBuffers)
{
    Buffers const buf;
    ASSERT_TRUE(buf.info().empty());
    ASSERT_TRUE(buf.empty());
    ASSERT_EQ(0, buf.size());
}

static void genTestData(std::string const & prefix,
                        std::size_t size,
                        std::vector<std::string> & strings,
                        std::vector<Buffers::binf> & infos)
{
    strings.resize(size);
    infos.resize(size);

    for (std::size_t i = 0; i < size; ++i) {
        std::stringstream ss;
        ss << prefix << i;
        strings[i] = ss.str();
        infos[i].buffer = &strings[i][0];
        infos[i].size = strings[i].size();
    }
}

TEST(CachedBuffersTest, Default)
{
    Buffers buffers;
    std::vector<std::string> strings;
    std::vector<Buffers::binf> infos;

    std::size_t const SIZE = 5;
    std::size_t i = 0;
    binfs result;

    genTestData("BUF", SIZE, strings, infos);
    buffers.assign(infos.data(), SIZE);
    result = buffers.info();
    ASSERT_EQ(SIZE, strings.size());
    ASSERT_EQ(SIZE, infos.size());
    ASSERT_EQ(SIZE, buffers.datas().size());
    ASSERT_EQ(SIZE, buffers.sizes().size());
    ASSERT_EQ(SIZE, buffers.size());
    for (i = 0; i < SIZE; ++i) {
        std::string const CURRENT_STRING(result[i].buffer, result[i].buffer + result[i].size);
        ASSERT_EQ(strings[i].size(), result[i].size);
        ASSERT_EQ(strings[i], CURRENT_STRING);
    }

    Buffers buffers2 = std::move(buffers);
    ASSERT_TRUE(buffers.datas().empty());
    ASSERT_TRUE(buffers.sizes().empty());
    ASSERT_TRUE(buffers.empty());

    ASSERT_EQ(SIZE, buffers2.datas().size());
    ASSERT_EQ(SIZE, buffers2.sizes().size());
    ASSERT_EQ(SIZE, buffers2.size());

    buffers2.clear();
    ASSERT_TRUE(buffers2.datas().empty());
    ASSERT_TRUE(buffers2.sizes().empty());
    ASSERT_TRUE(buffers2.empty());
}

TEST(CachedBuffersTest, FewSizeToMuchSize)
{
    Buffers buffers;
    std::vector<std::string> strings;
    std::vector<Buffers::binf> infos;

    std::size_t const FIRST_SIZE = 5;
    std::size_t const SIZE = 10;
    std::size_t i = 0;
    binfs result;

    genTestData("BUF", FIRST_SIZE, strings, infos);
    buffers.assign(infos.data(), FIRST_SIZE);

    genTestData("TEST", SIZE, strings, infos);
    buffers.assign(infos.data(), SIZE);
    result = buffers.info();
    ASSERT_EQ(SIZE, strings.size());
    ASSERT_EQ(SIZE, infos.size());
    ASSERT_EQ(SIZE, buffers.datas().size());
    ASSERT_EQ(SIZE, buffers.sizes().size());
    ASSERT_EQ(SIZE, buffers.size());
    for (i = 0; i < SIZE; ++i) {
        std::string const CURRENT_STRING(result[i].buffer, result[i].buffer + result[i].size);
        ASSERT_EQ(strings[i].size(), result[i].size);
        ASSERT_EQ(strings[i], CURRENT_STRING);
    }
}

TEST(CachedBuffersTest, MuchSizeToFewSize)
{
    Buffers buffers;
    std::vector<std::string> strings;
    std::vector<Buffers::binf> infos;

    std::size_t const FIRST_SIZE = 9;
    std::size_t const SIZE = 6;
    std::size_t i = 0;
    binfs result;

    assert(0 <= COMPARE_AND(FIRST_SIZE) <= 9);
    assert(0 <= COMPARE_AND(SIZE) <= 9);

    std::string const FIRST_PREFIX = "TEST_BUFFER";
    genTestData(FIRST_PREFIX, FIRST_SIZE, strings, infos);
    buffers.assign(infos.data(), FIRST_SIZE);

    std::string const PREFIX = "BUF";
    genTestData(PREFIX, SIZE, strings, infos);
    buffers.assign(infos.data(), SIZE);
    result = buffers.info();

    ASSERT_EQ(SIZE, strings.size());
    ASSERT_EQ(SIZE, infos.size());
    ASSERT_EQ(FIRST_SIZE, buffers.datas().size());
    ASSERT_EQ(FIRST_SIZE, buffers.sizes().size());
    ASSERT_EQ(SIZE, buffers.size());

    for (i = 0; i < SIZE; ++i) {
        ASSERT_EQ(FIRST_PREFIX.size() + 1, buffers.datas()[i].size());
        ASSERT_EQ(PREFIX.size() + 1, buffers.sizes()[i]);

        std::string const CURRENT_STRING(result[i].buffer, result[i].buffer + result[i].size);
        ASSERT_EQ(strings[i].size(), result[i].size);
        ASSERT_EQ(strings[i], CURRENT_STRING);
    }
}

