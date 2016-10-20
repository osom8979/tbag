/**
 * @file   ModuleMain.cpp
 * @brief  Module entry-point.
 * @author zer0
 * @date   2016-10-19
 */

#include <libtbag/config.h>
#include <libtbag/macro/attributes.hpp>
#include <libtbag/macro/utils.hpp>
#include <libtbag/loop/UvEventLoop.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

static bool setUp()
{
    libtbag::loop::UvEventLoop loop;
    loop.runDefault();

    return true;
}

static bool tearDown()
{
    return true;
}

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

static void TBAG_CONSTRUCTOR
__tbag_constructor(void)
{
    ::libtbag::setUp();
}

static void TBAG_DESTRUCTOR
__tbag_destructor(void)
{
    ::libtbag::tearDown();
}

#if defined(__OS_WINDOWS__)
#include <windows.h>
BOOL WINAPI DllMain(_In_ HINSTANCE hinstDLL
                  , _In_ DWORD     fdwReason
                  , _In_ LPVOID    lpvReserved)
{
    bool result = false;
    switch (fdwReason) {
    case DLL_PROCESS_ATTACH: result = ::libtbag::setUp();    break;
    case DLL_PROCESS_DETACH: result = ::libtbag::tearDown(); break;
    case DLL_THREAD_ATTACH:  result = ::libtbag::setUp();    break;
    case DLL_THREAD_DETACH:  result = ::libtbag::tearDown(); break;
    }
    return (result == true ? TRUE : FALSE);
}
#endif // defined(__OS_WINDOWS__)

