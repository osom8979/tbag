/**
 * @file   StoragePrefTest.cpp
 * @brief  StoragePref class tester.
 * @author zer0
 * @date   2020-02-25
 */

#include <gtest/gtest.h>
#include <libtbag/pref/details/StoragePref.hpp>
#include <libtbag/util/TestUtils.hpp>

using namespace libtbag;
using namespace libtbag::pref;
using namespace libtbag::pref::details;

TEST(StoragePrefTest, Default)
{
    tttDir_Automatic();
    auto const dir = tttDir_Get();
    auto const test_dir = dir / "test.dir";

    TBAG_CONSTEXPR static char const * const TEST_XML = R"__xml__(
        <storage root='${STORAGE_TEMP_DIR}'>
          <user name='test' absolute='false' raw='true'>test.dir</user>
        </storage>)__xml__";

    StoragePref pref;
    ASSERT_TRUE(pref.readFromXmlText(TEST_XML));

    StoragePref::Environments envs;
    envs.push("STORAGE_TEMP_DIR", dir.toString());

    auto const storage = pref.loadStorage(envs);
    auto const test_layout_path = storage.getLayout("test");
    ASSERT_EQ(test_dir, test_layout_path);
}

