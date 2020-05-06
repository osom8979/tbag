/**
 * @file   Path_Scan_Test.cpp
 * @brief  Path class tester.
 * @author zer0
 * @date   2020-04-26
 */

#include <gtest/gtest.h>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/filesystem/File.hpp>
#include <libtbag/util/TestUtils.hpp>

#include <algorithm>

using namespace libtbag;
using namespace libtbag::filesystem;

struct PathScanFixture : public testing::Test
{
    TBAG_CONSTEXPR static char const * const TEST_DIR1 = "dir";
    TBAG_CONSTEXPR static char const * const TEST_DIR2 = "dir/sub";
    TBAG_CONSTEXPR static char const * const TEST_DIR3 = "dir/.sub";
    TBAG_CONSTEXPR static char const * const TEST_DIR4 = ".dir";

    TBAG_CONSTEXPR static char const * const TEST_PATH1 = "file";
    TBAG_CONSTEXPR static char const * const TEST_PATH2 = ".file";

    TBAG_CONSTEXPR static char const * const TEST_PATH3 = "dir/file";
    TBAG_CONSTEXPR static char const * const TEST_PATH4 = "dir/.file";
    TBAG_CONSTEXPR static char const * const TEST_PATH5 = ".dir/file";
    TBAG_CONSTEXPR static char const * const TEST_PATH6 = ".dir/.file";

    TBAG_CONSTEXPR static char const * const TEST_PATH7 = "dir/sub/file.txt";
    TBAG_CONSTEXPR static char const * const TEST_PATH8 = "dir/sub/.file.txt";
    TBAG_CONSTEXPR static char const * const TEST_PATH9 = "dir/.sub/file.txt";
    TBAG_CONSTEXPR static char const * const TEST_PATH10= "dir/.sub/.file.txt";

    TBAG_CONSTEXPR static char const * const TEST_CONTENT = "TEST";

    void initDirectory(Path const & dir)
    {
        ASSERT_TRUE((dir/TEST_DIR1).createDir());
        ASSERT_TRUE((dir/TEST_DIR2).createDir());
        ASSERT_TRUE((dir/TEST_DIR3).createDir());
        ASSERT_TRUE((dir/TEST_DIR4).createDir());

        ASSERT_EQ(E_SUCCESS, writeFile(dir/TEST_PATH1, TEST_CONTENT));
        ASSERT_EQ(E_SUCCESS, writeFile(dir/TEST_PATH2, TEST_CONTENT));
        ASSERT_EQ(E_SUCCESS, writeFile(dir/TEST_PATH3, TEST_CONTENT));
        ASSERT_EQ(E_SUCCESS, writeFile(dir/TEST_PATH4, TEST_CONTENT));
        ASSERT_EQ(E_SUCCESS, writeFile(dir/TEST_PATH5, TEST_CONTENT));
        ASSERT_EQ(E_SUCCESS, writeFile(dir/TEST_PATH6, TEST_CONTENT));
        ASSERT_EQ(E_SUCCESS, writeFile(dir/TEST_PATH7, TEST_CONTENT));
        ASSERT_EQ(E_SUCCESS, writeFile(dir/TEST_PATH8, TEST_CONTENT));
        ASSERT_EQ(E_SUCCESS, writeFile(dir/TEST_PATH9, TEST_CONTENT));
        ASSERT_EQ(E_SUCCESS, writeFile(dir/TEST_PATH10, TEST_CONTENT));
    }

    template <typename L, typename R>
    static bool exists_path(std::vector<L> const & paths, R const & path)
    {
        for (auto const & cursor : paths) {
            if (Path(cursor) == Path(path)) {
                return true;
            }
        }
        return false;
    }

    static bool exists_dir(std::vector<Path> const & paths, Path const & path)
    {
        return path.isDirectory() && exists_path(paths, path);
    }

    static bool exists_file(std::vector<Path> const & paths, Path const & path)
    {
        return path.isRegularFile() && exists_path(paths, path);
    }
};

TEST_F(PathScanFixture, ScanDir)
{
    tttDir_Automatic();
    auto const TEMP_DIR = tttDir_Get();
    initDirectory(TEMP_DIR);

    ASSERT_EQ(4, TEMP_DIR.scanDir().size());
    ASSERT_EQ(2, TEMP_DIR.scanDir(Path::DIRENT_DIR).size());
    ASSERT_EQ(2, TEMP_DIR.scanDir(Path::DIRENT_FILE).size());
}

TEST_F(PathScanFixture, ScanRecurrentDir)
{
    tttDir_Automatic();
    auto const TEMP_DIR = tttDir_Get();
    initDirectory(TEMP_DIR);

    auto const paths = TEMP_DIR.scanRecurrentDir();
    ASSERT_TRUE(exists_dir(paths, (TEMP_DIR/TEST_DIR1)));
    ASSERT_TRUE(exists_dir(paths, (TEMP_DIR/TEST_DIR2)));
    ASSERT_TRUE(exists_dir(paths, (TEMP_DIR/TEST_DIR3)));
    ASSERT_TRUE(exists_dir(paths, (TEMP_DIR/TEST_DIR4)));
    ASSERT_TRUE(exists_file(paths, (TEMP_DIR/TEST_PATH1)));
    ASSERT_TRUE(exists_file(paths, (TEMP_DIR/TEST_PATH2)));
    ASSERT_TRUE(exists_file(paths, (TEMP_DIR/TEST_PATH3)));
    ASSERT_TRUE(exists_file(paths, (TEMP_DIR/TEST_PATH4)));
    ASSERT_TRUE(exists_file(paths, (TEMP_DIR/TEST_PATH5)));
    ASSERT_TRUE(exists_file(paths, (TEMP_DIR/TEST_PATH6)));
    ASSERT_TRUE(exists_file(paths, (TEMP_DIR/TEST_PATH7)));
    ASSERT_TRUE(exists_file(paths, (TEMP_DIR/TEST_PATH8)));
    ASSERT_TRUE(exists_file(paths, (TEMP_DIR/TEST_PATH9)));
    ASSERT_TRUE(exists_file(paths, (TEMP_DIR/TEST_PATH10)));
}

TEST_F(PathScanFixture, ScanRecurrentNameOnly)
{
    tttDir_Automatic();
    auto const TEMP_DIR = tttDir_Get();
    initDirectory(TEMP_DIR);

    auto const names = TEMP_DIR.scanRecurrentNameOnly(Path::DIRENT_DIR);
    for (auto const & name : names) {
        std::cout << name << std::endl;
    }
    ASSERT_EQ(4, names.size());
    ASSERT_TRUE(exists_path(names, std::string(TEST_DIR1)));
    ASSERT_TRUE(exists_path(names, std::string(TEST_DIR2)));
    ASSERT_TRUE(exists_path(names, std::string(TEST_DIR3)));
    ASSERT_TRUE(exists_path(names, std::string(TEST_DIR4)));
}

TEST_F(PathScanFixture, ScanRecurrentFiles)
{
    tttDir_Automatic();
    auto const TEMP_DIR = tttDir_Get();
    initDirectory(TEMP_DIR);

    auto const names1 = TEMP_DIR.scanRecurrentFiles({".txt"}, false);
    for (auto const & name : names1) {
        std::cout << name << std::endl;
    }
    ASSERT_EQ(4, names1.size());
    ASSERT_TRUE(exists_path(names1, std::string(TEST_PATH7)));
    ASSERT_TRUE(exists_path(names1, std::string(TEST_PATH8)));
    ASSERT_TRUE(exists_path(names1, std::string(TEST_PATH9)));
    ASSERT_TRUE(exists_path(names1, std::string(TEST_PATH10)));

    auto const names2 = TEMP_DIR.scanRecurrentFiles({".txt"}, true);
    ASSERT_EQ(1, names2.size());
    ASSERT_TRUE(exists_path(names2, std::string(TEST_PATH7)));
}

