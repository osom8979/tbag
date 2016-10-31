/**
 * @file   InitTest.cpp
 * @brief  Test initialize.
 * @author zer0
 * @date   2016-10-31
 */

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/loop/UvEventLoop.hpp>

static void initTest()
{
    libtbag::loop::UvEventLoop loop;
    loop.runDefault();
}

#if defined(TBAG_STATIC_API)
initTest();
#endif

