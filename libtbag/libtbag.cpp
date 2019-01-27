/**
 * @file   libtbag.cpp
 * @brief  libtbag entry-point implementation.
 * @author zer0
 * @date   2016-10-19
 */

#include <libtbag/libtbag.h>
#include <libtbag/util/SingletonUtils.hpp>
#include <libtbag/locale/Locale.hpp>
#include <libtbag/crypto/SslUtils.hpp>
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
        libtbag::crypto::initializeSsl();
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
        libtbag::crypto::releaseSsl();
        libtbag::uvpp::release();

        _init = false;
        return true;
    }
};

LibtbagInitializer::Mutex LibtbagInitializer::_mutex;
bool LibtbagInitializer::_init = false;

static bool setUp(tbInitParam * param = nullptr)
{
    if (LibtbagInitializer::isInit()) {
        __tbag_debug("It has already been initialized.");
        return true;
    }
    return LibtbagInitializer::init(param);
}

static bool tearDown()
{
    if (!libtbag::LibtbagInitializer::isInit()) {
        __tbag_debug("It has already been release.");
        return true;
    }
    return libtbag::LibtbagInitializer::release();
}

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

// -------------------
// Module entry-point.
// -------------------

static void TBAG_CONSTRUCTOR __tbag_constructor(void)
{
    // ::libtbag::setUp();
}

static void TBAG_DESTRUCTOR __tbag_destructor(void)
{
    // ::libtbag::tearDown();
}

#if defined(TBAG_PLATFORM_WINDOWS)
#include <windows.h>
BOOL WINAPI DllMain(_In_ HINSTANCE hinstDLL,
                    _In_ DWORD     fdwReason,
                    _In_ LPVOID    lpvReserved)
{
    switch (fdwReason) {
    case DLL_PROCESS_ATTACH: break;
    case DLL_PROCESS_DETACH: break;
    case DLL_THREAD_ATTACH:  break;
    case DLL_THREAD_DETACH:  break;
    }
    return TRUE;
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

tbBOOL tbIsInit()
{
    return libtbag::LibtbagInitializer::isInit() ? TB_TRUE : TB_FALSE;
}

void tbInitialize(tbInitParam * param)
{
    libtbag::setUp(param);
}

void tbRelease()
{
    libtbag::tearDown();
}

// -----------------
// Tpot entry-point.
// -----------------

//#include <libtbag/tpot/TpotMain.hpp>

int tbPotMain(int argc, char ** argv, char ** envs)
{
//    return libtbag::tpot::runTpotMain(argc, argv, envs);
}

