/**
 * @file   TmxChunkTest.cpp
 * @brief  TmxChunk class tester.
 * @author zer0
 * @date   2019-07-10
 */

#include <gtest/gtest.h>
#include <libtbag/tiled/details/TmxChunk.hpp>
#include <libtbag/util/TestUtils.hpp>
#include <string>

using namespace libtbag;
using namespace libtbag::tiled;
using namespace libtbag::tiled::details;

TEST(TmxChunkTest, Default)
{
    TEST_DEFAULT_CONSTRUCTOR(TmxChunk, obj1)
    TEST_DEFAULT_ASSIGNMENT(TmxChunk, obj2)
}

template <TmxChunk::TileLayerFormat v>
struct format_value_t
{
    TBAG_CONSTEXPR static TmxChunk::TileLayerFormat const value = v;
};

using         xml_format_value = format_value_t<TmxChunk::TileLayerFormat::XML>;
using      base64_format_value = format_value_t<TmxChunk::TileLayerFormat::BASE64>;
using gzip_base64_format_value = format_value_t<TmxChunk::TileLayerFormat::GZIP_BASE64>;
using zlib_base64_format_value = format_value_t<TmxChunk::TileLayerFormat::ZLIB_BASE64>;
using         csv_format_value = format_value_t<TmxChunk::TileLayerFormat::CSV>;

template <typename T>
struct TileLayerFormatTestFeature : public ::testing::Test
{
    TBAG_CONSTEXPR static TmxChunk::TileLayerFormat const get_format_value() TBAG_NOEXCEPT
    { return T::value; }
};

using test_format_types = ::testing::Types<
        xml_format_value,
        base64_format_value,
        gzip_base64_format_value,
        zlib_base64_format_value,
        csv_format_value>;
TYPED_TEST_CASE(TileLayerFormatTestFeature, test_format_types);

TYPED_TEST(TileLayerFormatTestFeature, ReadAndWrite)
{
    char const * const TEST_XML = R"(
<chunk x="30" y="40" width="10" height="20">
  <tile/>
  <tile gid="1"/>
  <tile gid="49"/>
  <tile gid="50"/>
  <tile gid="1"/>
  <tile/>
  <tile gid="1"/>
  <tile gid="2"/>
  <tile gid="3"/>
  <tile/>
</chunk>)";

    TmxChunk chunk;
    ASSERT_EQ(E_SUCCESS, chunk.read(TEST_XML));
    ASSERT_EQ(30, chunk.x);
    ASSERT_EQ(40, chunk.y);
    ASSERT_EQ(10, chunk.width);
    ASSERT_EQ(20, chunk.height);
    ASSERT_EQ(10, chunk.gids.size());
    ASSERT_EQ( 0, chunk.gids[0]);
    ASSERT_EQ( 1, chunk.gids[1]);
    ASSERT_EQ(49, chunk.gids[2]);
    ASSERT_EQ(50, chunk.gids[3]);
    ASSERT_EQ( 1, chunk.gids[4]);
    ASSERT_EQ( 0, chunk.gids[5]);
    ASSERT_EQ( 1, chunk.gids[6]);
    ASSERT_EQ( 2, chunk.gids[7]);
    ASSERT_EQ( 3, chunk.gids[8]);
    ASSERT_EQ( 0, chunk.gids[9]);

    std::string xml;
    ASSERT_EQ(E_SUCCESS, chunk.write(xml, this->get_format_value()));
#if 0
    std::cout << xml << std::endl;
#endif

    TmxChunk chunk2;
    ASSERT_EQ(E_SUCCESS, chunk2.read(xml, this->get_format_value()));
    ASSERT_EQ(30, chunk2.x);
    ASSERT_EQ(40, chunk2.y);
    ASSERT_EQ(10, chunk2.width);
    ASSERT_EQ(20, chunk2.height);
    ASSERT_EQ(10, chunk2.gids.size());
    ASSERT_EQ( 0, chunk2.gids[0]);
    ASSERT_EQ( 1, chunk2.gids[1]);
    ASSERT_EQ(49, chunk2.gids[2]);
    ASSERT_EQ(50, chunk2.gids[3]);
    ASSERT_EQ( 1, chunk2.gids[4]);
    ASSERT_EQ( 0, chunk2.gids[5]);
    ASSERT_EQ( 1, chunk2.gids[6]);
    ASSERT_EQ( 2, chunk2.gids[7]);
    ASSERT_EQ( 3, chunk2.gids[8]);
    ASSERT_EQ( 0, chunk2.gids[9]);
}

