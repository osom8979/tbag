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
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace lib {

/**
 * SharedLibrary class prototype.
 *
 * @author zer0
 * @date   2016-04-17
 */
class TBAG_API SharedLibrary : private Noncopyable
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

public:
    void * symbol(char const * name) const;
    void * symbol(std::string const & name) const;

public:
    std::string getError() const;

public:
    inline bool isOpen() const TBAG_NOEXCEPT
    { return _open; }

public:
    template <typename ReturnType, typename ... Arg>
    ReturnType call(char const * name, Arg && ... args) const
    {
        typedef ReturnType (*Signature) (Arg ...);
        Signature func = (Signature)symbol(name);
        if (func != nullptr) {
            return func(std::forward<Arg>(args) ...);
        }
        return ReturnType();
    }
};

} // namespace lib

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LIB_SHAREDLIBRARY_HPP__

