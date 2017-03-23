/**
 * @file   libtbag.cpp
 * @brief  libtbag entry-point implementation.
 * @author zer0
 * @date   2016-10-19
 */

#include <libtbag/libtbag.h>
#include <libtbag/util/SingletonUtils.hpp>
#include <libtbag/locale/Locale.hpp>
#include <libtbag/uvpp/UvCommon.hpp>
#include <libtbag/log/Log.hpp>

#include <mutex>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

/**
 * Initializer helper class.
 *
 * @author zer0
 * @date   2016-12-24
 */
struct LibtbagInitializer
{
public:
    using Mutex = std::mutex;
    using Guard = std::lock_guard<Mutex>;

private:
    static Mutex _mutex;
    static bool  _init;

public:
    static bool init(tbInitParam * param = nullptr)
    {
        Guard guard(_mutex);
        if (_init == true) {
            __tbag_debug("It has already been initialized.");
            return false;
        }

        libtbag::uvpp::initialize();
        libtbag::util::initSingletonObjects();

        __tbag_debug(LIBTBAG_MAIN_TITLE);
        __tbag_debug("Default locale name: {}", libtbag::locale::getDefaultIcuLocaleName());

        return true;
    }

    static bool release()
    {
        Guard guard(_mutex);
        if (_init == false) {
            __tbag_debug("It has already been relased.");
            return false;
        }

        libtbag::util::releaseSingletonObjects();
        libtbag::uvpp::release();

        return true;
    }
};

LibtbagInitializer::Mutex LibtbagInitializer::_mutex;
bool LibtbagInitializer::_init = false;

static bool setUp()
{
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

#if defined(__PLATFORM_WINDOWS__)
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
#endif // defined(__PLATFORM_WINDOWS__)

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

tbBOOL tbInitialize(tbInitParam * param)
{
    return libtbag::LibtbagInitializer::init(param) ? AB_TRUE : AB_FALSE;
}

tbBOOL tbRelease()
{
    return libtbag::LibtbagInitializer::release() ? AB_TRUE : AB_FALSE;
}

