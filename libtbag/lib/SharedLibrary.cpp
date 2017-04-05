/**
 * @file   SharedLibrary.cpp
 * @brief  SharedLibrary class implementation.
 * @author zer0
 * @date   2016-07-7
 */

#include <libtbag/lib/SharedLibrary.hpp>
#include <libtbag/predef.hpp>

#include <cassert>
#include <cstring>
#include <string>

#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace lib {

std::string getLibraryPrefix()
{
#if defined(TBAG_PLATFORM_WINDOWS) && !defined(TBAG_PLATFORM_MINGW)
    return "";
#else
    return "lib";
#endif
}

std::string getLibrarySuffix()
{
#if defined(TBAG_PLATFORM_WINDOWS)
# if defined(TBAG_PLATFORM_MINGW)
    return ".dll.so";
# else
    return ".dll";
# endif
#elif defined(TBAG_PLATFORM_MACOS)
    return ".dylib";
#else
    return ".so";
#endif
}

std::string getLibraryName(std::string const & name)
{
    return getLibraryPrefix() + name + getLibrarySuffix();
}

template <typename T>
inline uv_lib_t * uv_lib_cast(T * object)
{
    return static_cast<uv_lib_t*>(object);
}

// -----------------------------
// SharedLibrary implementation.
// -----------------------------

SharedLibrary::SharedLibrary() : _open(false), _lib(new uv_lib_t)
{
    memset(_lib, 0x00, sizeof(uv_lib_t));
}

SharedLibrary::SharedLibrary(std::string const & path) : SharedLibrary()
{
    this->open(path);
}

SharedLibrary::~SharedLibrary()
{
    this->close();

    assert(_lib != nullptr);
    delete uv_lib_cast(_lib);
}

bool SharedLibrary::open(std::string const & path)
{
    _open = (uv_dlopen(path.c_str(), uv_lib_cast(_lib)) == 0);
    return _open;
}

void SharedLibrary::close()
{
    if (_open == true) {
        uv_dlclose(uv_lib_cast(_lib));
        _open = false;
    }
}

void * SharedLibrary::symbol(std::string const & name)
{
    if (_open == true) {
        void * result = nullptr;
        if (uv_dlsym(uv_lib_cast(_lib), name.c_str(), &result) == 0) {
            return result;
        }
    }
    return nullptr;
}

std::string SharedLibrary::getError() const
{
    return std::string(uv_dlerror(uv_lib_cast(_lib)));
}

} // namespace lib

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

