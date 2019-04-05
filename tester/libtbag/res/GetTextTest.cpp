/**
 * @file   GetTextTest.cpp
 * @brief  GetText class tester.
 * @author zer0
 * @date   2018-11-06
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/res/GetText.hpp>
#include <libtbag/filesystem/File.hpp>
#include <libtbag/filesystem/Path.hpp>

using namespace libtbag;
using namespace libtbag::res;

TBAG_CONSTEXPR static char const * const GET_TEXT_EN_XML = R"(
<resource>
    <text name='test'>TEST_EN</text>
</resource>
)";

TBAG_CONSTEXPR static char const * const GET_TEXT_KO_XML = R"(
<resource>
    <text name='test'>TEST_KO</text>
</resource>
)";

TEST(GetTextTest, Static)
{
    // clang-format off
    ASSERT_STREQ("resource", std::string(GetText::ROOT_TAG).c_str());
    ASSERT_STREQ("text"    , std::string(GetText::PROP_TAG).c_str());
    ASSERT_STREQ("name"    , std::string(GetText::NAME_ATT).c_str());
    // clang-format on
}

TEST(GetTextTest, Read)
{
    tttDir_Automatic();
    auto const PATH = tttDir_Get();
    std::string const EN_NAME = "en";
    std::string const KO_NAME = "ko";
    auto const EN_PATH = PATH / EN_NAME;
    auto const KO_PATH = PATH / KO_NAME;

    using namespace libtbag::filesystem;
    ASSERT_EQ(E_SUCCESS, writeFile(EN_PATH.toString(), GET_TEXT_EN_XML));
    ASSERT_EQ(E_SUCCESS, writeFile(KO_PATH.toString(), GET_TEXT_KO_XML));
    ASSERT_TRUE(EN_PATH.isRegularFile());
    ASSERT_TRUE(KO_PATH.isRegularFile());

    GetText text;
    ASSERT_TRUE(text.empty());
    ASSERT_TRUE(text.load(PATH));
    ASSERT_FALSE(text.empty());
    ASSERT_EQ(2, text.size());
    ASSERT_TRUE(text.getLanguage().empty());
    ASSERT_EQ(nullptr, text.current());

    ASSERT_TRUE(text.get("test").empty());
    ASSERT_STREQ("TEST_EN", text.get(EN_NAME, "test").c_str());
    ASSERT_STREQ("TEST_KO", text.get(KO_NAME, "test").c_str());

    text.setLanguage(EN_NAME, false);
    ASSERT_TRUE(text.updateCache());

    ASSERT_NE(nullptr, text.current());
    ASSERT_STREQ("TEST_EN", text.get("test").c_str());
    ASSERT_STREQ("TEST_EN", text.get(EN_NAME, "test").c_str());
    ASSERT_STREQ("TEST_KO", text.get(KO_NAME, "test").c_str());
}

TEST(GetTextTest, Save)
{
    tttDir_Automatic();
    auto const PATH = tttDir_Get();
    std::string const EN_NAME = "en";
    std::string const KO_NAME = "ko";

    std::string const KEY = "test";
    std::string const VAL_EN = "val_en";
    std::string const VAL_KO = "val_ko";

    GetText text;
    ASSERT_TRUE(text.empty());
    ASSERT_TRUE(text.getLanguage().empty());
    ASSERT_EQ(nullptr, text.current());

    text.set(EN_NAME, KEY, VAL_EN);
    text.set(KO_NAME, KEY, VAL_KO);
    ASSERT_FALSE(text.empty());
    ASSERT_EQ(2, text.size());

    ASSERT_TRUE(text.save(PATH));

    GetText text2;
    ASSERT_TRUE(text2.load(PATH));
    ASSERT_FALSE(text2.empty());
    ASSERT_EQ(2, text2.size());
    ASSERT_EQ(VAL_EN, text.get(EN_NAME, KEY));
    ASSERT_EQ(VAL_KO, text.get(KO_NAME, KEY));
}

