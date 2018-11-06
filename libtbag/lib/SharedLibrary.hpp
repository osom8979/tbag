/**
 * @file   SharedLibrary.hpp
 * @brief  SharedLibrary class prototype.
 * @author zer0
 * @date   2018-11-06
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LIB_SHAREDLIBRARY_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LIB_SHAREDLIBRARY_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/lib/DynamicModule.hpp>

#include <string>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace lib {

/**
 * SharedLibrary class prototype.
 *
 * @author zer0
 * @date   2018-11-06
 */
class TBAG_API SharedLibrary
{
public:
    using SharedModule = std::shared_ptr<DynamicModule>;

private:
    SharedModule _module;

public:
    SharedLibrary();
    explicit SharedLibrary(std::string const & path);
    SharedLibrary(std::nullptr_t) TBAG_NOEXCEPT;
    SharedLibrary(SharedLibrary const & obj) TBAG_NOEXCEPT;
    SharedLibrary(SharedLibrary && obj) TBAG_NOEXCEPT;
    ~SharedLibrary();

public:
    SharedLibrary & operator =(SharedLibrary const & obj) TBAG_NOEXCEPT;
    SharedLibrary & operator =(SharedLibrary && obj) TBAG_NOEXCEPT;

public:
    void copy(SharedLibrary const & obj) TBAG_NOEXCEPT;
    void swap(SharedLibrary & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(SharedLibrary & lh, SharedLibrary & rh) TBAG_NOEXCEPT { lh.swap(rh); }

public:
    inline bool exists() const TBAG_NOEXCEPT
    { return static_cast<bool>(_module); }

    inline operator bool() const TBAG_NOEXCEPT
    { return exists(); }

public:
    inline DynamicModule       * get()       TBAG_NOEXCEPT { return _module.get(); }
    inline DynamicModule const * get() const TBAG_NOEXCEPT { return _module.get(); }

    inline DynamicModule       * operator ->()       TBAG_NOEXCEPT { return get(); }
    inline DynamicModule const * operator ->() const TBAG_NOEXCEPT { return get(); }

    inline DynamicModule       & operator *()       TBAG_NOEXCEPT { return *get(); }
    inline DynamicModule const & operator *() const TBAG_NOEXCEPT { return *get(); }

public:
    /**
     * Implemented for std::less<> compatibility.
     *
     * @see std::set
     * @see std::map
     * @see std::less
     */
    friend inline bool operator <(SharedLibrary const & x, SharedLibrary const & y) TBAG_NOEXCEPT
    {
        return x.get() < y.get();
    }

    inline bool operator ==(SharedLibrary const & obj) const TBAG_NOEXCEPT
    {
        return get() == obj.get();
    }

    inline bool operator !=(SharedLibrary const & obj) const TBAG_NOEXCEPT
    {
        return get() != obj.get();
    }

public:
    void reset();

public:
    bool open(std::string const & path);
    void close();

public:
    void * symbol(char const * name) const;
    void * symbol(std::string const & name) const;

public:
    std::string getError() const;

public:
    bool isOpen() const TBAG_NOEXCEPT;

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

