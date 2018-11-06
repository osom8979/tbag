/**
 * @file   DynamicModule.cpp
 * @brief  DynamicModule class implementation.
 * @author zer0
 * @date   2016-07-07
 * @date   2018-11-06 (Rename: SharedLibrary -> DynamicModule)
 */

#include <libtbag/lib/DynamicModule.hpp>
#include <libtbag/predef.hpp>

#include <cassert>
#include <cstring>
#include <string>

#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace lib {

template <typename T>
inline uv_lib_t * uv_lib_cast(T * object)
{
    return static_cast<uv_lib_t*>(object);
}

// -----------------------------
// DynamicModule implementation.
// -----------------------------

DynamicModule::DynamicModule() : _open(false), _lib(new uv_lib_t)
{
    ::memset(_lib, 0x00, sizeof(uv_lib_t));
}

DynamicModule::DynamicModule(std::string const & path) : DynamicModule()
{
    if (!open(path)) {
        throw std::bad_alloc();
    }
}

DynamicModule::~DynamicModule()
{
    close();
    assert(_lib != nullptr);
    delete uv_lib_cast(_lib);
}

bool DynamicModule::open(std::string const & path)
{
    _open = (::uv_dlopen(path.c_str(), uv_lib_cast(_lib)) == 0);
    return _open;
}

void DynamicModule::close()
{
    if (_open) {
        ::uv_dlclose(uv_lib_cast(_lib));
        _open = false;
    }
}

void * DynamicModule::symbol(char const * name) const
{
    if (_open) {
        void * result = nullptr;
        if (::uv_dlsym(uv_lib_cast(_lib), name, &result) == 0) {
            return result;
        }
    }
    return nullptr;
}

void * DynamicModule::symbol(std::string const & name) const
{
    return symbol(name.c_str());
}

std::string DynamicModule::getError() const
{
    return std::string(uv_dlerror(uv_lib_cast(_lib)));
}

} // namespace lib

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

