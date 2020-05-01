/**
 * @file   CwdGuardTest.cpp
 * @brief  CwdGuard class tester.
 * @author zer0
 * @date   2020-05-01
 */

#include <gtest/gtest.h>
#include <libtbag/filesystem/CwdGuard.hpp>
#include <libtbag/util/TestUtils.hpp>

using namespace libtbag;
using namespace libtbag::filesystem;

TEST(CwdGuardTest, Default)
{
    tttDir_AutoCreate();
    auto const dir = tttDir_Get();
    auto const cwd = Path::getWorkDir();
    COMMENT("Save state") {
        CwdGuard const G(dir);
        ASSERT_EQ(dir, Path::getWorkDir());
    }
    ASSERT_EQ(cwd, Path::getWorkDir());
}

