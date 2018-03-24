/**
 * @file   TestUtils.hpp
 * @brief  TestUtils class prototype.
 * @author zer0
 * @date   2016-06-30
 * @date   2018-03-24 (Rename: tester/DemoAsset -> libtbag/util/TestUtils)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UTIL_TESTUTILS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UTIL_TESTUTILS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/filesystem/TempDirGuard.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace util {

// EMPTY.

} // namespace util

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#ifndef TBAG_TEST_TEMP_DIR_NAME
#define TBAG_TEST_TEMP_DIR_NAME __tbag_tester_temp_dir__
#endif

/**
 * @def TBAG_TEST_TEMP_DIR
 * @breif Tbag Test Temp Directory.
 */
#ifndef TBAG_TEST_TEMP_DIR
#define TBAG_TEST_TEMP_DIR(is_first_create, is_last_remove) \
    ::libtbag::filesystem::TempDirGuard TBAG_TEST_TEMP_DIR_NAME( \
            test_info_->test_case_name(), \
            test_info_->name(), \
            is_first_create, \
            is_last_remove); \
    ASSERT_TRUE(TBAG_TEST_TEMP_DIR_NAME.getDir().isDirectory());
#endif

/**
 * @def TBAG_TEST_TEMP_DIR_GET
 * @breif Obtain the current temp directory.
 */
#ifndef TBAG_TEST_TEMP_DIR_GET
#define TBAG_TEST_TEMP_DIR_GET() TBAG_TEST_TEMP_DIR_NAME.getDir()
#endif

#ifndef tttDir
#define tttDir(c, r) TBAG_TEST_TEMP_DIR(c, r)
#endif

#ifndef tttDir_Automatic
#define tttDir_Automatic() tttDir(true, true)
#endif

#ifndef tttDir_AutoCreate
#define tttDir_AutoCreate() tttDir(true, false)
#endif

#ifndef tttDir_AutoRemove
#define tttDir_AutoRemove() tttDir(false, true)
#endif

#ifndef tttDir_Manually
#define tttDir_Manually() tttDir(false, false)
#endif

#ifndef tttDir_Get
#define tttDir_Get() TBAG_TEST_TEMP_DIR_GET()
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UTIL_TESTUTILS_HPP__

