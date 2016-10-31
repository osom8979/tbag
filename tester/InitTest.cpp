/**
 * @file   InitTest.cpp
 * @brief  Test initialize.
 * @author zer0
 * @date   2016-10-31
 */

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/loop/UvEventLoop.hpp>

/** Initialize object. */
struct InitTest
{
    InitTest()
    {
        libtbag::loop::UvEventLoop loop;
        loop.runDefault();
    }

    ~InitTest()
    { /* EMPTY. */ }
};

#if defined(TBAG_STATIC_API)
static InitTest g_init_tester;
#endif

