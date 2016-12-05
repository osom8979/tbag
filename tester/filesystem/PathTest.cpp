/**
 * @file   PathTest.cpp
 * @brief  Path class tester.
 * @author zer0
 * @date   2016-04-07
 */

#include <gtest/gtest.h>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/filesystem/Common.hpp>

#include <iostream>

using namespace libtbag;
using namespace libtbag::filesystem;

TEST(PathTest, StyleChecker)
{
#if defined(WIN32) || defined(_WIN32)
    ASSERT_TRUE(Path::isWindowsStyle());
    ASSERT_FALSE(Path::isPosixStyle());
#else
    ASSERT_FALSE(Path::isWindowsStyle());
    ASSERT_TRUE(Path::isPosixStyle());
#endif
}

TEST(PathTest, Constructors_1)
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

TEST(PathTest, Constructors_2)
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
    char const * const TEMP = "/1/2/3/4/5.test";

    Path const path1(TEMP);
    ASSERT_STREQ("5.test", path1.getName().c_str());

    Path const path2("");
    ASSERT_STREQ("", path2.getName().c_str());
}

TEST(PathTest, SplitNodesWithCanonical_1)
{
    std::string home = getHomeDir();
    std::string work = getWorkDir();

    char const * const TEMP1 = "~/TEMP1/TEMP2/../TEMP3/./../../TEMP4";
    char const * const TEMP2 = "TEMP2/.";

    Path path1(TEMP1);
    Path path2(TEMP2);

    path1.updateCanonical();
    path2.updateCanonical();

    ASSERT_EQ(Path(home + "/TEMP4").getGenericString(), path1.getGenericString());
    ASSERT_EQ(Path(work + "/TEMP2").getGenericString(), path2.getGenericString());
}

TEST(PathTest, SplitNodesWithCanonical_2)
{
#if defined(WIN32) || defined(_WIN32)
    char const * const TEMP = "/TEMP";
#else
    char const * const TEMP = "C:\\TEMP";
#endif

    Path const temp(TEMP);
    Path const path = Path(temp.splitNodesWithCanonical());
    ASSERT_STREQ(TEMP, temp.getString().c_str());
}

TEST(PathTest, FilesystemOperators)
{
    char const * const FILENAME = "__path_test.dir/filesystem_operators.dir";
    Path path = Path::getExeDir() / FILENAME;

    ASSERT_FALSE(path.createDir());
    ASSERT_TRUE(path.createDirWithRecursive());

    ASSERT_TRUE(path.exists());
    ASSERT_TRUE(path.isExecutable());
    ASSERT_TRUE(path.isWritable());
    ASSERT_TRUE(path.isReadable());

    ASSERT_FALSE(path.isRegularFile());
    ASSERT_TRUE(path.isDirectory());
    ASSERT_FALSE(path.removeFile());

    ASSERT_FALSE(path.getParent().removeDir());
    ASSERT_TRUE(path.getParent().removeDirWithRecursive());

    ASSERT_EQ(0U, path.scanDir().size());
}

TEST(PathTest, SpecialDirectories)
{
    ASSERT_EQ(Path::getWorkDir().getString(), getWorkDir());
    ASSERT_EQ(Path::getHomeDir().getString(), getHomeDir());
    ASSERT_EQ(Path::getExePath().getString(), getExePath());
}

