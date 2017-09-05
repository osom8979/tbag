/**
 * @file   ComInitializer.cpp
 * @brief  ComInitializer class implementation.
 * @author zer0
 * @date   2017-09-05
 */

#include <libtbag/lib/ComInitializer.hpp>
#include <libtbag/log/Log.hpp>

#if defined(TBAG_PLATFORM_WINDOWS)
#include <windows.h>
#pragma comment(lib, "ole32.lib")
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace lib {

ComInitializer::ComInitializer() : _init(false)
{
#if defined(TBAG_PLATFORM_WINDOWS)
    // We do not care about the result of the function call: any result is OK for us.
    if (CoInitializeEx(0, COINIT_MULTITHREADED) == S_OK) {
        _init = true;
    }
#endif
}

ComInitializer::~ComInitializer()
{
#if defined(TBAG_PLATFORM_WINDOWS)
    if (_init) {
        CoUninitialize();
        _init = true;
    }
#endif
}

} // namespace lib

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

