/**
 * @file   libtbag.cpp
 * @brief  libtbag entry-point implementation.
 * @author zer0
 * @date   2016-10-19
 */

#include <libtbag/libtbag.h>
#include <libtbag/util/SingletonUtils.hpp>
#include <libtbag/locale/Locale.hpp>
#include <libtbag/encrypt/SslUtils.hpp>
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
    inline static bool isInit()
    {
        Guard guard(_mutex);
        return _init;
    }

    static bool init(tbInitParam * param = nullptr)
    {
        Guard guard(_mutex);
        if (_init == true) {
            __tbag_debug("It has already been initialized.");
            return false;
        }

        libtbag::uvpp::initialize();
        libtbag::encrypt::initializeSsl();
        libtbag::util::createSingletonObjects();

        _init = true;
        return true;
    }

    static bool release()
    {
        Guard guard(_mutex);
        if (_init == false) {
            __tbag_debug("It has already been release.");
            return false;
        }

        libtbag::util::releaseSingletonObjects();
        libtbag::encrypt::releaseSsl();
        libtbag::uvpp::release();

        _init = false;
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

#if defined(TBAG_PLATFORM_WINDOWS)
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
#endif // defined(TBAG_PLATFORM_WINDOWS)

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
    if (libtbag::LibtbagInitializer::isInit()) {
        __tbag_debug("It has already been initialized.");
        return TB_TRUE;
    }
    return libtbag::LibtbagInitializer::init(param) ? TB_TRUE : TB_FALSE;
}

tbBOOL tbRelease()
{
    if (libtbag::LibtbagInitializer::isInit() == false) {
        __tbag_debug("It has already been release.");
        return TB_TRUE;
    }
    return libtbag::LibtbagInitializer::release() ? TB_TRUE : TB_FALSE;
}

// -----------------
// Tpot entry-point.
// -----------------

#include <libtbag/tpot/TpotMain.hpp>

int tbPotMain(int argc, char ** argv, char ** envs)
{
    return libtbag::tpot::runTpotMain(argc, argv, envs);
}

