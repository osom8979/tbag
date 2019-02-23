/**
 * @file   tester.cpp
 * @brief  Tester project entry-point.
 * @author zer0
 * @date   2016-11-01
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/libtbag.h>
#include <libtbag/signal/SignalHandler.hpp>
#include <libtbag/locale/Locale.hpp>
#include <libtbag/log/Log.hpp>

#include <cstdlib>
#include <iostream>
#include <string>

#if defined(DISABLE_LUAJIT_EXE_FLAGS)
TBAG_CONSTEXPR static char const * const DEFAULT_GTEST_FILTER = "-Sol2*";
#else
TBAG_CONSTEXPR static char const * const DEFAULT_GTEST_FILTER = "*";
#endif

int main(int argc, char **argv)
{
    tbInitialize();

#if defined(TBAG_PLATFORM_WINDOWS)
    libtbag::log::createDefaultStdoutLogger();
#else
    libtbag::log::createDefaultColorStdoutLogger();
#endif
    libtbag::log::setDefaultSeverity(libtbag::log::DEBUG_SEVERITY);

    libtbag::signal::registerDefaultStdTerminateHandler();
    libtbag::signal::registerDefaultHandler();

    tDLogI(LIBTBAG_MAIN_TITLE);
    tDLogI("Default locale name: {}", libtbag::locale::getDefaultIcuLocaleName());

    libtbag::DemoAsset asset;
    asset.create_temp_dir();

    testing::GTEST_FLAG(filter) = DEFAULT_GTEST_FILTER;
    testing::InitGoogleTest(&argc, argv);
    int const EXIT_CODE = RUN_ALL_TESTS();

    tbRelease();
    return EXIT_CODE;
}

