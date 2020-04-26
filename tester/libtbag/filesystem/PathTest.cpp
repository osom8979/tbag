/**
 * @file   PathTest.cpp
 * @brief  Path class tester.
 * @author zer0
 * @date   2016-04-07
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/util/TestUtils.hpp>

#include <algorithm>
#include <fstream>

using namespace libtbag;
using namespace libtbag::filesystem;

TEST(PathTest, Constructors1)
{
    char const * const TEMP = "TEMP";

    Path p0;
    Path p1 = Path(TEMP);
    Path p2 = Path(std::string(TEMP));
    Path p3 = Path(TEMP, Path::update_generic());
    Path p4 = Path(std::string(TEMP), Path::update_generic());
    Path p5 = p1;
    Path p6_move = Path(TEMP);
    Path p6 = std::move(p6_move);

    Path p7 = Path(TEMP, Path::update_canonical());
    Path p8 = Path(std::string(TEMP), Path::update_canonical());

    ASSERT_STREQ("", p0.getString().c_str());
    ASSERT_STREQ(TEMP, p1.getString().c_str());
    ASSERT_STREQ(TEMP, p2.getString().c_str());
    ASSERT_STREQ(TEMP, p3.getString().c_str());
    ASSERT_STREQ(TEMP, p4.getString().c_str());
    ASSERT_STREQ(TEMP, p5.getString().c_str());
    ASSERT_STREQ(TEMP, p6.getString().c_str());
}

TEST(PathTest, Constructors2)
{
    std::vector<std::string> nodes;
    nodes.push_back("1");
    nodes.push_back("2");

    Path p = nodes;
    ASSERT_STREQ("1/2", p.getGenericString().c_str());
}

TEST(PathTest, InitializerList)
{
    Path p0 = {"/", "TE", "M", "P"};
    ASSERT_STREQ("/TE/M/P", p0.getGenericString().c_str());
}

TEST(PathTest, AssignOperators)
{
    char const * const TEMP = "TEMP";

    auto rvalue_test = [&]() -> Path {
        return Path(TEMP);
    };

    Path const TEMP_PATH = Path(TEMP);
    std::string const TEMP_STRING = TEMP;
    ASSERT_STREQ(TEMP, TEMP_PATH.getString().c_str());

    Path p0;

    p0 = TEMP;
    ASSERT_STREQ(TEMP, p0.getString().c_str());

    p0 = TEMP_STRING;
    ASSERT_STREQ(TEMP, p0.getString().c_str());

    p0 = std::string(TEMP);
    ASSERT_STREQ(TEMP, p0.getString().c_str());

    p0 = TEMP_PATH;
    ASSERT_STREQ(TEMP, p0.getString().c_str());

    p0 = rvalue_test();
    ASSERT_STREQ(TEMP, p0.getString().c_str());
}

TEST(PathTest, GetName)
{
    Path const path1("/1/2/3/4/5.test");
    ASSERT_STREQ("5.test", path1.getName().c_str());

    Path const path2("");
    ASSERT_STREQ("", path2.getName().c_str());

    Path const path3("/1/2/3/4.dir/5.test.ext");
    ASSERT_STREQ("5.test.ext", path3.getName().c_str());
}

TEST(PathTest, GetNameWithoutExtension)
{
    Path const path1("/1/2/3/4.dir/5.test.ext");
    ASSERT_STREQ("5", path1.getNameWithoutExtension().c_str());

    Path const path2("");
    ASSERT_STREQ("", path2.getNameWithoutExtension().c_str());

    Path const path3("/1/2/3/4.dir/5_test_ext");
    ASSERT_STREQ("5_test_ext", path3.getNameWithoutExtension().c_str());
}

TEST(PathTest, GetExtensionName)
{
    Path const path1("/1/2/3/4.dir/5.test.ext");
    ASSERT_STREQ(".test.ext", path1.getExtensionName().c_str());

    Path const path2("");
    ASSERT_STREQ("", path2.getExtensionName().c_str());

    Path const path3("/1/2/3/4.dir/5_test_ext");
    ASSERT_STREQ("", path3.getExtensionName().c_str());
}

TEST(PathTest, SplitNodes_Canonical1)
{
    std::string home = Path::getHomeDir();
    std::string work = Path::getWorkDir();

    char const * const TEMP1 = "~/TEMP1/TEMP2/../TEMP3/./../../TEMP4";
    char const * const TEMP2 = "TEMP2/.";

    Path path1(TEMP1);
    Path path2(TEMP2);

    path1.updateCanonical();
    path2.updateCanonical();

    ASSERT_EQ(Path(home + "/TEMP4").getGenericString(), path1.getGenericString());
    ASSERT_EQ(Path(work + "/TEMP2").getGenericString(), path2.getGenericString());
}

TEST(PathTest, SplitNodes_Canonical2)
{
#if defined(TBAG_PLATFORM_WINDOWS)
    char const * const TEMP = "/TEMP";
#else
    char const * const TEMP = "C:\\TEMP";
#endif

    Path const temp(TEMP);
    Path const path = Path(temp.splitNodesWithCanonical());
    ASSERT_STREQ(TEMP, temp.getString().c_str());
}

TEST(PathTest, ExistsNode)
{
    Path const test_path("exists/node/with/canonical/exists");
    ASSERT_EQ(0, test_path.findNode("exists"));
    ASSERT_EQ(1, test_path.findNode("node"));
    ASSERT_EQ(2, test_path.findNode("with"));
    ASSERT_EQ(3, test_path.findNode("canonical"));
    ASSERT_EQ(4, test_path.findNode("exists", 1));
    ASSERT_EQ(static_cast<std::size_t>(Path::nop), test_path.findNode("__"));
}

TEST(PathTest, TestSuffix)
{
    Path const test_path(".exists.txt");
    ASSERT_TRUE(test_path.testSuffix(""));
    ASSERT_TRUE(test_path.testSuffix("t"));
    ASSERT_TRUE(test_path.testSuffix(".txt"));
    ASSERT_TRUE(test_path.testSuffix("s.txt"));
    ASSERT_TRUE(test_path.testSuffix(".exists.txt"));

    ASSERT_FALSE(test_path.testSuffix("1"));
    ASSERT_FALSE(test_path.testSuffix(".tx1"));
    ASSERT_FALSE(test_path.testSuffix("s.tx1"));
    ASSERT_FALSE(test_path.testSuffix(".exist1.txt"));
    ASSERT_FALSE(test_path.testSuffix("a.exists.txt"));

    Path const test_path1("");
    ASSERT_TRUE(test_path1.testSuffix(""));
    ASSERT_FALSE(test_path1.testSuffix("1"));

    Path const test_path2(".");
    ASSERT_TRUE(test_path2.testSuffix(""));
    ASSERT_TRUE(test_path2.testSuffix("."));
    ASSERT_FALSE(test_path2.testSuffix(".."));
    ASSERT_FALSE(test_path2.testSuffix("1"));
}

TEST(PathTest, SpecialDirectories)
{
    ASSERT_FALSE(Path::getWorkDir().getString().empty());
    ASSERT_FALSE(Path::getHomeDir().getString().empty());
    ASSERT_FALSE(Path::getExePath().getString().empty());
    ASSERT_FALSE(Path::getExeDir ().getString().empty());
}

