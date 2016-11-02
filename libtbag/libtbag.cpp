/**
 * @file   libtbag.cpp
 * @brief  libtbag entry-point implementation.
 * @author zer0
 * @date   2016-10-19
 */

#include <libtbag/libtbag.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

static bool setUp()
{
    // Don't use libuv initialize.
    return true;
}

static bool tearDown()
{
    return true;
}

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

// -------------------
// Module entry-point.
// -------------------

static void TBAG_CONSTRUCTOR __tbag_constructor(void)
{
    ::libtbag::setUp();
}

static void TBAG_DESTRUCTOR __tbag_destructor(void)
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

// -----------------------
// libtbag implementation.
// -----------------------

int tbGetMajorVersion()
{
    return LIBTBAG_VERSION_MAJOR;
}

int tbGetMinorVersion()
{
    return LIBTBAG_VERSION_MINOR;
}

int tbGetPatchVersion()
{
    return LIBTBAG_VERSION_PATCH;
}

#include <libtbag/loop/UvEventLoop.hpp>

int tbInitialize()
{
    libtbag::loop::UvEventLoop loop;
    loop.runDefault();
    return 0;
}

