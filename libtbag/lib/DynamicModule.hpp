/**
 * @file   DynamicModule.hpp
 * @brief  DynamicModule class prototype.
 * @author zer0
 * @date   2016-04-17
 * @date   2018-11-06 (Rename: SharedLibrary -> DynamicModule)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LIB_DYNAMICMODULE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LIB_DYNAMICMODULE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <cassert>
#include <string>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace lib {

/**
 * DynamicModule class prototype.
 *
 * @author zer0
 * @date   2016-04-17
 * @date   2018-11-06 (Rename: SharedLibrary -> DynamicModule)
 */
class TBAG_API DynamicModule : private Noncopyable
{
public:
    using FakeLib = void;

private:
    bool _open;
    FakeLib * _lib;

public:
    DynamicModule();
    explicit DynamicModule(std::string const & path);
    ~DynamicModule();

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
    /**
     * Call the function of the dynamic module.
     *
     * @warning
     *  In perfect forwarding, <code>void*</code> can be replaced with <code>void*&</code>,
     *  so you must force cast all symbols.
     *
     * @remarks
     *  Example:
     *  @code{.cpp}
     *   DynamicModule module;
     *   module.open("module.so");
     *   module.call<void>("func", (void*)context, (char const *)data, (int)size);
     *  @endcode
     */
    template <typename ReturnT, typename ... Arg>
    ReturnT call(char const * name, Arg && ... args) const
    {
        assert(name != nullptr);
        typedef ReturnT (*SignatureT) (Arg ...);
        SignatureT func = (SignatureT)symbol(name);
        assert(func != nullptr);
        return func(std::forward<Arg>(args) ...);
    }
};

} // namespace lib

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LIB_DYNAMICMODULE_HPP__

