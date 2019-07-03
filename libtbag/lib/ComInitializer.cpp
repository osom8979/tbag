/**
 * @file   ComInitializer.cpp
 * @brief  ComInitializer class implementation.
 * @author zer0
 * @date   2017-09-05
 */

#include <libtbag/lib/ComInitializer.hpp>

#if defined(HAVE_COINITIALIZEEX_FUNC)
#include <Objbase.h>
#pragma comment(lib, "ole32.lib")
#else
# include <libtbag/dummy/Win32.hpp>
using namespace ::libtbag::dummy::win32;
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace lib {

ComInitializer::ComInitializer() : _init(false)
{
    init();
}

ComInitializer::~ComInitializer()
{
    uninit();
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

