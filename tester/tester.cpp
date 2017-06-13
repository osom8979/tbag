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
#include <libtbag/log/Log.hpp>

#include <cstdlib>
#include <iostream>
#include <string>

static char const * const DEFAULT_GTEST_FILTER = "-Network*Test.*";

int main(int argc, char **argv)
{
    __tbag_debug(LIBTBAG_MAIN_TITLE);
    __tbag_debug("Default locale name: {}", libtbag::locale::getDefaultIcuLocaleName());

    tbInitialize();

#if defined(TBAG_PLATFORM_WINDOWS)
    libtbag::log::createDefaultConsoleLogger();
#else
    libtbag::log::createDefaultColorConsoleLogger();
#endif
    libtbag::log::setDefaultSeverity(libtbag::log::DEBUG_SEVERITY);

    libtbag::signal::registerDefaultStdTerminateHandler();
    libtbag::signal::registerDefaultHandler();

    libtbag::DemoAsset asset;
    asset.create_temp_dir();

    testing::GTEST_FLAG(filter) = DEFAULT_GTEST_FILTER;
    testing::InitGoogleTest(&argc, argv);
    int const EXIT_CODE = RUN_ALL_TESTS();

    tbRelease();
    return EXIT_CODE;
}

