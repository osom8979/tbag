/**
 * @file   FileKeyStoreTest.cpp
 * @brief  FileKeyStore class tester.
 * @author zer0
 * @date   2017-12-07
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/security/store/FileKeyStore.hpp>

using namespace libtbag;
using namespace libtbag::security;
using namespace libtbag::security::store;

TEST(FileKeyStoreTest, Default)
{
    tttDir(true, true);
    auto const FILE_PATH = tttDirGet() / "store";

    FileKeyStore object(FILE_PATH.toString());
}

