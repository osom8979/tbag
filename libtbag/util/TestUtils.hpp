/**
 * @file   TestUtils.hpp
 * @brief  Google-test utilities.
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
#include <libtbag/Noncopyable.hpp>
#include <libtbag/filesystem/TempDirGuard.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace util      {
namespace test      {
namespace behaviour {

struct TBAG_API Scenario
{
    std::string const NAME;
    std::string const TAG;

    Scenario(std::string const & name);
    Scenario(std::string const & name, std::string const & tag);
    ~Scenario();

    inline operator bool() const TBAG_NOEXCEPT { return true; }
};

struct TBAG_API Given
{
    Scenario const & SCENARIO;
    std::string const NAME;

    Given(Scenario & parent, std::string const & name);
    ~Given();

    inline operator bool() const TBAG_NOEXCEPT { return true; }
};

struct TBAG_API When
{
    Given const & GIVEN;
    std::string const NAME;

    When(Given & parent, std::string const & name);
    ~When();

    inline operator bool() const TBAG_NOEXCEPT { return true; }
};

struct TBAG_API Then
{
    When const & WHEN;
    std::string const NAME;

    Then(When const & parent, std::string const & name);
    ~Then();

    inline operator bool() const TBAG_NOEXCEPT { return true; }
};

#define TBAG_SCENARIO_NAME  __require_scenario__
#define TBAG_GIVEN_NAME     __require_given__
#define TBAG_WHEN_NAME      __require_when__
#define TBAG_THEN_NAME      __require_then__

#ifndef TBAG_SCENARIO
#define TBAG_SCENARIO(.../*name, tag*/) if (auto TBAG_SCENARIO_NAME = ::libtbag::util::test::behaviour::Scenario(__VA_ARGS__))
#endif

#ifndef TBAG_GIVEN
#define TBAG_GIVEN(name) if (auto TBAG_GIVEN_NAME = ::libtbag::util::test::behaviour::Given(TBAG_SCENARIO_NAME, name))
#endif

#ifndef TBAG_WHEN
#define TBAG_WHEN(name) if (auto TBAG_WHEN_NAME = ::libtbag::util::test::behaviour::When(TBAG_GIVEN_NAME, name))
#endif

#ifndef TBAG_THEN
#define TBAG_THEN(name) if (auto TBAG_THEN_NAME = ::libtbag::util::test::behaviour::Then(TBAG_WHEN_NAME, name))
#endif

} // namespace behaviour
} // namespace test
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

