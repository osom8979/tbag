/**
 * @file   ComInitializer.cpp
 * @brief  ComInitializer class implementation.
 * @author zer0
 * @date   2017-09-05
 */

#include <libtbag/lib/ComInitializer.hpp>
#include <libtbag/log/Log.hpp>

#if defined(HAVE_COINITIALIZEEX_FUNC)
#include <Objbase.h>
#pragma comment(lib, "ole32.lib")
#else
#include <libtbag/proxy/windows/Dummy.hpp>
using namespace libtbag::proxy::windows;
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace lib {

ComInitializer::ComInitializer() : _init(false)
{
    // EMPTY.
}

ComInitializer::~ComInitializer()
{
    // EMPTY.
}

bool ComInitializer::init()
{
    Guard g(_mutex);
    if (_init == false) {
        // We do not care about the result of the function call: any result is OK for us.
        _init = (CoInitializeEx(0, COINIT_MULTITHREADED) == S_OK ? true : false);
    }
    return _init;
}

void ComInitializer::uninit()
{
    Guard g(_mutex);
    if (_init) {
        CoUninitialize();
        _init = false;
    }
}

} // namespace lib

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

