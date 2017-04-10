/**
 * @file   SharedLibrary.hpp
 * @brief  SharedLibrary class prototype.
 * @author zer0
 * @date   2016-04-17
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LIB_SHAREDLIBRARY_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LIB_SHAREDLIBRARY_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace lib {

TBAG_API std::string getLibraryPrefix();
TBAG_API std::string getLibrarySuffix();
TBAG_API std::string getLibraryName(std::string const & name);

/**
 * SharedLibrary class prototype.
 *
 * @author zer0
 * @date   2016-04-17
 */
class TBAG_API SharedLibrary : public Noncopyable
{
public:
    using FakeLib = void;

private:
    bool _open;
    FakeLib * _lib;

public:
    SharedLibrary();
    SharedLibrary(std::string const & path);
    ~SharedLibrary();

public:
    bool open(std::string const & path);
    void close();
    void * symbol(std::string const & name);

public:
    std::string getError() const;

public:
    inline bool isOpen() const TBAG_NOEXCEPT
    { return _open; }

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
};

} // namespace lib

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LIB_SHAREDLIBRARY_HPP__

