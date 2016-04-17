/**
 * @file   SharedLibrary.hpp
 * @brief  SharedLibrary class prototype.
 * @author zer0
 * @date   2016-04-17
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_SHAREDLIBRARY_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_SHAREDLIBRARY_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/Noncopyable.hpp>

#include <cstring>
#include <string>

#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

/**
 * SharedLibrary class prototype.
 *
 * @author zer0
 * @date   2016-04-17
 */
class SharedLibrary : public Noncopyable
{
private:
    bool _open;
    uv_lib_t _lib;

public:
    SharedLibrary() : _open(false) {
        memset(&this->_lib, 0x00, sizeof(this->_lib));
    }

    SharedLibrary(std::string const & path) : SharedLibrary() {
        open(path);
    }

    ~SharedLibrary() {
        close();
    }

public:
    bool open(std::string const & path) {
        if (uv_dlopen(path.c_str(), &this->_lib) == 0) {
            this->_open = true;
        } else {
            this->_open = false;
        }
        return this->_open;
    }

    void close() {
        if (this->_open) {
            uv_dlclose(&this->_lib);
            this->_open = false;
        }
    }

    void * symbol(std::string const & name) {
        if (this->_open) {
            void * result = nullptr;
            if (uv_dlsym(&this->_lib, name.c_str(), &result) == 0) {
                return result;
            }
        }
        return nullptr;
    }

public:
    template <typename ReturnType, typename ... Arg>
    ReturnType call(std::string const & name, Arg ... args) {
        typedef ReturnType (*Signature) (Arg ...);
        Signature func = (Signature) symbol(name.c_str());
        if (func != nullptr) {
            return func(args ...);
        }
        return ReturnType();
    }

    template <typename ... Arg>
    void callVoid(std::string const & name, Arg ... args) {
        typedef void (*Signature) (Arg ...);
        Signature func = (Signature) symbol(name.c_str());
        if (func != nullptr) {
            func(args ...);
        }
    }

public:
    std::string getError() const {
        return std::string(uv_dlerror(&this->_lib));
    }
};


// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#ifndef SHARED_LIBRARY_SYMBOL
#define SHARED_LIBRARY_SYMBOL
#endif // SHARED_LIBRARY_SYMBOL



#endif // __INCLUDE_LIBTBAG__LIBTBAG_SHAREDLIBRARY_HPP__

