/**
 * @file   PathTest.cpp
 * @brief  Path class tester.
 * @author zer0
 * @date   2016-04-07
 */

#include <gtest/gtest.h>
#include <libtbag/filesystem/Path.hpp>

using namespace libtbag;
using namespace libtbag::filesystem;

TEST(PathStaticTest, StyleChecker)
{
#if defined(WIN32) || defined(_WIN32)
    ASSERT_TRUE(Path::isWindowsStyle());
    ASSERT_FALSE(Path::isPosixStyle());
#else
    ASSERT_FALSE(Path::isWindowsStyle());
    ASSERT_TRUE(Path::isPosixStyle());
#endif
}

TEST(PathStaticTest, Constructors)
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

    ASSERT_STREQ(p0.getString().c_str(), "");
    ASSERT_STREQ(p1.getString().c_str(), TEMP);
    ASSERT_STREQ(p2.getString().c_str(), TEMP);
    ASSERT_STREQ(p3.getString().c_str(), TEMP);
    ASSERT_STREQ(p4.getString().c_str(), TEMP);
    ASSERT_STREQ(p5.getString().c_str(), TEMP);
    ASSERT_STREQ(p6.getString().c_str(), TEMP);
}

TEST(PathStaticTest, initializer_list)
{
    Path p0 = {"/", "TE", "M", "P"};
    ASSERT_EQ(p0.getString(), std::string("/TE/M/P"));
}

TEST(PathStaticTest, AssignOperators)
{
    char const * const TEMP = "TEMP";

    auto rvalue_test = [&]() -> Path {
        return Path(TEMP);
    };

    Path const TEMP_PATH = Path(TEMP);
    std::string const TEMP_STRING = TEMP;
    ASSERT_STREQ(TEMP_PATH.getString().c_str(), TEMP);

    Path p0;

    p0 = TEMP;
    ASSERT_STREQ(p0.getString().c_str(), TEMP);

    p0 = TEMP_STRING;
    ASSERT_STREQ(p0.getString().c_str(), TEMP);

    p0 = std::string(TEMP);
    ASSERT_STREQ(p0.getString().c_str(), TEMP);

    p0 = TEMP_PATH;
    ASSERT_STREQ(p0.getString().c_str(), TEMP);

    p0 = rvalue_test();
    ASSERT_STREQ(p0.getString().c_str(), TEMP);
}

TEST(PathStaticTest, isProhibitedFilename)
{
    std::string const FILENAME1 = "TEMP";
    ASSERT_FALSE(Path::isProhibitedFilenameOfWindows(FILENAME1));
    ASSERT_FALSE(Path::isProhibitedFilenameOfPosix(FILENAME1));
    
    std::string const FILENAME2 = "TE\\MP";
    ASSERT_TRUE(Path::isProhibitedFilenameOfWindows(FILENAME2));
    ASSERT_FALSE(Path::isProhibitedFilenameOfPosix(FILENAME2));

    std::string const FILENAME3 = "TE/MP";
    ASSERT_TRUE(Path::isProhibitedFilenameOfWindows(FILENAME3));
    ASSERT_TRUE(Path::isProhibitedFilenameOfPosix(FILENAME3));
}

TEST(PathStaticTest, removeLastSeparator)
{
    std::string const PREFIX = "\\/TE/\\MP";
    std::string const WINDOWS_SUFFIX = "\\\\";
    std::string const TEMP = PREFIX + WINDOWS_SUFFIX + "//";

    std::string windows = Path::removeLastSeparatorOfWindows(TEMP);
    std::string posix   = Path::removeLastSeparatorOfPosix(TEMP);

    ASSERT_EQ(windows, PREFIX);
    ASSERT_EQ(posix, PREFIX + WINDOWS_SUFFIX);
}

TEST(PathStaticTest, replaceHomeDirectoryShortcut_1)
{
    std::string const HOME = Common::getHomeDir();
    std::string const TEMP = "~";
    ASSERT_EQ(Path::replaceHomeDirectoryShortcut(TEMP), HOME);
}

TEST(PathStaticTest, replaceHomeDirectoryShortcut_2)
{
    std::string const HOME = Common::getHomeDir();
    std::string const TEMP = "TEMP";
    std::string const TEST = std::string("~/") + TEMP;

    Path p1 = Path(Path::replaceHomeDirectoryShortcut(TEMP), Path::update_generic());
    Path p2 = Path(Common::getHomeDir(), Path::update_generic());
    p2.append(TEMP);
    ASSERT_EQ(p2.getNative(), p2.getNative());
}

// Fixture.

class PathTest : public ::testing::Test
{
public:
    std::string ws;
    std::string ps;
    std::string gs;
    std::string s;

    std::string depth0;
    std::string depth1;
    std::string depth2;
    std::string depth3;
    std::string depth4;
    std::string depth5;

    std::string test_path;
    std::string windows_path;
    std::string posix_path;
    std::string generic_path;
    std::string canonical_path;

    Path path;

public:
    PathTest() {
        ws = PATH_SEPARATOR_STRING_OF_WINDOWS;
        ps = PATH_SEPARATOR_STRING_OF_POSIX;
        gs = GetGenericPathSeparatorString();
        s  = GetPathSeparatorString();
    }

    virtual ~PathTest() {
        __EMPTY_BLOCK__
    }

public:
    virtual void SetUp() override {
        __EMPTY_BLOCK__
    }

    virtual void TearDown() override {
        __EMPTY_BLOCK__
    }

public:
    void setupOfWindows() {
        depth0 = "O:";
        depth1 = "Temp";
        depth2 = "Directory";
        depth3 = "..";
        depth4 = ".";
        depth5 = "File.tmp";

        test_path       = R"(O:\\Temp\Directory/..\.//\\/File.tmp\/)";
        windows_path    = R"(O:\Temp\File.tmp)";
        posix_path      = R"(O:/Temp/Directory/.././File.tmp)";
        generic_path    = R"(O:/Temp/Directory/.././File.tmp)";
        canonical_path  = R"(O:/Temp/File.tmp)";

        path = Path(test_path);
    }

    void setupOfPosix() {
        depth0 = "/";
        depth1 = "Temp";
        depth2 = "\\Directory";
        depth3 = "..";
        depth4 = ".";
        depth5 = "File.tmp";

        test_path       = R"(/Temp/\Directory/.././////File.tmp//)";
        windows_path    = R"(/Temp/File.tmp)";
        posix_path      = R"(/Temp/\Directory/.././File.tmp)";
        generic_path    = R"(/Temp/\Directory/.././File.tmp)";
        canonical_path  = R"(/Temp/File.tmp)";

        path = Path(test_path);
    }
};

TEST_F(PathTest, getGeneric)
{
    setupOfWindows();
    ASSERT_EQ(Path::getGenericOfWindows(test_path), generic_path);

    setupOfPosix();
    ASSERT_EQ(Path::getGenericOfPosix(test_path), generic_path);
}

TEST_F(PathTest, getNative)
{
#if defined(WIN32) || defined(_WIN32)
    setupOfWindows();
    ASSERT_EQ(path.getNative(), windows_path);
#else
    setupOfPosix();
    ASSERT_EQ(path.getNative(), posix_path);
#endif
}

TEST_F(PathTest, getRootDir)
{
    setupOfWindows();
    ASSERT_EQ(Path::getRootDirOfWindows(test_path), depth0);

    setupOfPosix();
    ASSERT_EQ(Path::getRootDirOfPosix(test_path), depth0);
}

TEST_F(PathTest, isAbsolute)
{
    setupOfWindows();
    ASSERT_TRUE(Path::isAbsoluteOfWindows(test_path));
    ASSERT_FALSE(Path::isAbsoluteOfPosix(test_path));

    setupOfPosix();
    ASSERT_TRUE(Path::isAbsoluteOfPosix(test_path));
    ASSERT_FALSE(Path::isAbsoluteOfWindows(test_path));
}

TEST_F(PathTest, append)
{
    Path p0;
    p0.append(depth0);
    p0.append(depth1, std::string(GetGenericPathSeparatorString()));
    p0 /= depth2;
    p0 += depth3;
    p0.append(depth4);
    p0.append(depth5);
    ASSERT_EQ(p0.getGeneric(), generic_path);

    p0 = "";
    p0.append("/");
    ASSERT_EQ(p0.getString(), std::string("/"));
}

TEST_F(PathTest, getParentOfWindows)
{
    setupOfWindows();
    std::string test1 = Path::getParentOfWindows(test_path);
    std::string test2 = R"(O:\\Temp\Directory/..\.//\\)";
    ASSERT_EQ(test1, test2);

    test1 = Path::getParentOfWindows(test1);
    test2 = R"(O:\\Temp\Directory/..)";
    ASSERT_EQ(test1, test2);

    test1 = Path::getParentOfWindows(test1);
    test2 = R"(O:\\Temp\Directory)";
    ASSERT_EQ(test1, test2);

    test1 = Path::getParentOfWindows(test1);
    test2 = R"(O:\\Temp)";
    ASSERT_EQ(test1, test2);

    test1 = Path::getParentOfWindows(test1);
    test2 = R"(O:\)";
    ASSERT_EQ(test1, test2);

    test1 = Path::getParentOfWindows(test1);
    test2 = "";
    ASSERT_EQ(test1, test2);

    // OTHER.
    test1 = Path::getParentOfWindows("TEST_PATH");
    test2 = "";
    ASSERT_EQ(test1, test2);
}

TEST_F(PathTest, getParentOfPosix)
{
    setupOfPosix();
    std::string test1 = Path::getParentOfPosix(test_path);
    std::string test2 = R"(/Temp/\Directory/.././///)";
    ASSERT_EQ(test1, test2);

    test1 = Path::getParentOfPosix(test1);
    test2 = R"(/Temp/\Directory/..)";
    ASSERT_EQ(test1, test2);

    test1 = Path::getParentOfPosix(test1);
    test2 = R"(/Temp/\Directory)";
    ASSERT_EQ(test1, test2);

    test1 = Path::getParentOfPosix(test1);
    test2 = R"(/Temp)";
    ASSERT_EQ(test1, test2);

    test1 = Path::getParentOfPosix(test1);
    test2 = "/";
    ASSERT_EQ(test1, test2);

    test1 = Path::getParentOfPosix(test1);
    test2 = "";
    ASSERT_EQ(test1, test2);

    // OTHER.
    test1 = Path::getParentOfPosix("TEST_PATH");
    test2 = "";
    ASSERT_EQ(test1, test2);
}

TEST_F(PathTest, splitNodesOfWindows)
{
    setupOfWindows();
    Path::Nodes nodes = Path::splitNodesOfWindows(test_path);
    ASSERT_EQ(nodes.size(), 6U);
    ASSERT_EQ(nodes[0], depth0);
    ASSERT_EQ(nodes[1], depth1);
    ASSERT_EQ(nodes[2], depth2);
    ASSERT_EQ(nodes[3], depth3);
    ASSERT_EQ(nodes[4], depth4);
    ASSERT_EQ(nodes[5], depth5);
}

TEST_F(PathTest, splitNodesOfPosix)
{
    setupOfPosix();
    Path::Nodes nodes = Path::splitNodesOfPosix(test_path);
    ASSERT_EQ(nodes.size(), 6U);
    ASSERT_EQ(nodes[0], depth0);
    ASSERT_EQ(nodes[1], depth1);
    ASSERT_EQ(nodes[2], depth2);
    ASSERT_EQ(nodes[3], depth3);
    ASSERT_EQ(nodes[4], depth4);
    ASSERT_EQ(nodes[5], depth5);
}

