/**
 * @file   TmxDataTest.cpp
 * @brief  TmxData class tester.
 * @author zer0
 * @date   2019-07-09
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/tiled/details/TmxData.hpp>
#include <libtbag/util/TestUtils.hpp>
#include <string>

using namespace libtbag;
using namespace libtbag::tiled;
using namespace libtbag::tiled::details;

TEST(TmxDataTest, Default)
{
    TEST_DEFAULT_CONSTRUCTOR(TmxData, obj1)
    TEST_DEFAULT_ASSIGNMENT(TmxData, obj2)
}

struct TmxDataTestDatas
{
    std::string log_message;
    std::string xml;
    TmxData::Encoding encoding;
    TmxData::Compression compression;
    TmxData::DataType data_type;
    TmxData::TileLayerFormat format;
    bool infinie;
};

TEST(TmxDataTest, ReadAndWrite_Data)
{
    TmxDataTestDatas const TEST_DATAS[] = {
            {
                    "XML data format",
                    R"(<data> <tile gid="1"/> <tile gid="2"/> <tile gid="3"/> <tile/> </data>)",
                    TmxData::Encoding::NONE,
                    TmxData::Compression::NONE,
                    TmxData::DataType::GIDS,
                    TmxData::TileLayerFormat::XML,
                    false,
            },
            {
                    "Base64 data format",
                    R"(<data encoding="base64"> AQAAAAIAAAADAAAAAAAAAA== </data>)",
                    TmxData::Encoding::BASE64,
                    TmxData::Compression::NONE,
                    TmxData::DataType::GIDS,
                    TmxData::TileLayerFormat::BASE64,
                    false,
            },
            {
                    "Base64 (gzip) data format",
                    R"(<data encoding="base64" compression="gzip"> H4sIAAAAAAAAE2NkYGBgAmJmBggAALhDZyAQAAAA </data>)",
                    TmxData::Encoding::BASE64,
                    TmxData::Compression::GZIP,
                    TmxData::DataType::GIDS,
                    TmxData::TileLayerFormat::GZIP_BASE64,
                    false,
            },
            {
                    "Base64 (zlib) data format",
                    R"(<data encoding="base64" compression="zlib"> eJxjZGBgYAJiZgYIAAAAUAAH </data>)",
                    TmxData::Encoding::BASE64,
                    TmxData::Compression::ZLIB,
                    TmxData::DataType::GIDS,
                    TmxData::TileLayerFormat::ZLIB_BASE64,
                    false,
            },
            {
                    "CSV data format",
                    R"(<data encoding="csv"> 1,2, 3,0 </data>)",
                    TmxData::Encoding::CSV,
                    TmxData::Compression::NONE,
                    TmxData::DataType::GIDS,
                    TmxData::TileLayerFormat::CSV,
                    false,
            },
    };

    TBAG_SCENARIO("TMX <data> element test loop.") {
        for (auto & test_data : TEST_DATAS) {
            TBAG_GIVEN(test_data.log_message) {
                TmxData data;
                ASSERT_EQ(E_SUCCESS, data.read(test_data.xml));
                ASSERT_EQ(test_data.encoding, data.encoding);
                ASSERT_EQ(test_data.compression, data.compression);
                ASSERT_EQ(test_data.data_type, data.data_type);
                ASSERT_EQ(test_data.format, data.getTileLayerFormat());
                ASSERT_EQ(test_data.infinie, data.isInfinieMap());
                ASSERT_EQ(4, data.gids.size());
                ASSERT_EQ(1, data.gids[0]);
                ASSERT_EQ(2, data.gids[1]);
                ASSERT_EQ(3, data.gids[2]);
                ASSERT_EQ(0, data.gids[3]);

                std::string xml;
                ASSERT_EQ(E_SUCCESS, data.write(xml));

                TmxData data2;
                ASSERT_EQ(E_SUCCESS, data2.read(xml));
                ASSERT_EQ(test_data.encoding, data2.encoding);
                ASSERT_EQ(test_data.compression, data2.compression);
                ASSERT_EQ(test_data.data_type, data2.data_type);
                ASSERT_EQ(test_data.format, data2.getTileLayerFormat());
                ASSERT_EQ(test_data.infinie, data2.isInfinieMap());
                ASSERT_EQ(4, data2.gids.size());
                ASSERT_EQ(1, data2.gids[0]);
                ASSERT_EQ(2, data2.gids[1]);
                ASSERT_EQ(3, data2.gids[2]);
                ASSERT_EQ(0, data2.gids[3]);
            }
        }
    }
}

TEST(TmxDataTest, ReadAndWrite_Chunk)
{
    char const * const TEST_XML = R"(
<data encoding="csv">
  <chunk x="0" y="0" width="16" height="16">
    1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
  </chunk>
</data>
)";

    TmxData data;
    ASSERT_EQ(E_SUCCESS, data.read(TEST_XML));
    ASSERT_EQ(TmxData::Encoding::CSV, data.encoding);
    ASSERT_EQ(TmxData::Compression::NONE, data.compression);
    ASSERT_EQ(TmxData::DataType::CHUNK, data.data_type);
    ASSERT_EQ(TmxData::TileLayerFormat::CSV, data.getTileLayerFormat());
    ASSERT_TRUE(data.isInfinieMap());
    ASSERT_TRUE(data.gids.empty());
    ASSERT_EQ(1, data.chunks.size());
    ASSERT_EQ(16*16, data.chunks[0].gids.size());
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 16; ++j) {
            auto const index = (i*16)+j;
            if (i == 0 && j == 0) {
                ASSERT_EQ(1, data.chunks[0].gids[index]);
            } else if (i == 0 && j == 1) {
                ASSERT_EQ(2, data.chunks[0].gids[index]);
            } else if (i == 1 && j == 0) {
                ASSERT_EQ(3, data.chunks[0].gids[index]);
            } else {
                ASSERT_EQ(0, data.chunks[0].gids[index]);
            }
        }
    }

    std::string xml;
    ASSERT_EQ(E_SUCCESS, data.write(xml));

    TmxData data2;
    ASSERT_EQ(E_SUCCESS, data2.read(TEST_XML));
    ASSERT_EQ(TmxData::Encoding::CSV, data2.encoding);
    ASSERT_EQ(TmxData::Compression::NONE, data2.compression);
    ASSERT_EQ(TmxData::DataType::CHUNK, data2.data_type);
    ASSERT_EQ(TmxData::TileLayerFormat::CSV, data2.getTileLayerFormat());
    ASSERT_TRUE(data2.isInfinieMap());
    ASSERT_TRUE(data2.gids.empty());
    ASSERT_EQ(1, data2.chunks.size());
    ASSERT_EQ(16*16, data2.chunks[0].gids.size());
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 16; ++j) {
            auto const index = (i*16)+j;
            if (i == 0 && j == 0) {
                ASSERT_EQ(1, data2.chunks[0].gids[index]);
            } else if (i == 0 && j == 1) {
                ASSERT_EQ(2, data2.chunks[0].gids[index]);
            } else if (i == 1 && j == 0) {
                ASSERT_EQ(3, data2.chunks[0].gids[index]);
            } else {
                ASSERT_EQ(0, data2.chunks[0].gids[index]);
            }
        }
    }
}

