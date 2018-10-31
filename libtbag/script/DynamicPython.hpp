/**
 * @file   DynamicPython.hpp
 * @brief  DynamicPython class prototype.
 * @author zer0
 * @date   2018-10-31
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_DYNAMICPYTHON_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_DYNAMICPYTHON_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace script {

/**
 * DynamicPython class prototype.
 *
 * @author zer0
 * @date   2018-10-31
 */
class TBAG_API DynamicPython
{
public:
    struct Impl;
    friend struct Impl;

public:
    using SharedImpl = std::shared_ptr<Impl>;

private:
    SharedImpl _impl;

public:
    DynamicPython();
    DynamicPython(std::nullptr_t) TBAG_NOEXCEPT;
    DynamicPython(DynamicPython const & obj) TBAG_NOEXCEPT;
    DynamicPython(DynamicPython && obj) TBAG_NOEXCEPT;
    ~DynamicPython();

public:
    DynamicPython & operator =(DynamicPython const & obj) TBAG_NOEXCEPT;
    DynamicPython & operator =(DynamicPython && obj) TBAG_NOEXCEPT;

public:
    void copy(DynamicPython const & obj) TBAG_NOEXCEPT;
    void swap(DynamicPython & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(DynamicPython & lh, DynamicPython & rh) TBAG_NOEXCEPT { lh.swap(rh); }

public:
    inline bool exists() const TBAG_NOEXCEPT
    { return static_cast<bool>(_impl); }

    inline operator bool() const TBAG_NOEXCEPT
    { return exists(); }

public:
    inline Impl       * get()       TBAG_NOEXCEPT { return _impl.get(); }
    inline Impl const * get() const TBAG_NOEXCEPT { return _impl.get(); }

public:
    /**
     * Implemented for std::less<> compatibility.
     *
     * @see std::set
     * @see std::map
     * @see std::less
     */
    friend inline bool operator <(DynamicPython const & x, DynamicPython const & y) TBAG_NOEXCEPT
    {
        return x.get() < y.get();
    }

    inline bool operator ==(DynamicPython const & obj) const TBAG_NOEXCEPT
    {
        return get() == obj.get();
    }

public:
    bool isOpen() const;
    std::string getPath() const;

public:
    bool open(std::string const & path);
    void close();

public:
    void Py_Initialize();
    void Py_Finalize();
};

} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_DYNAMICPYTHON_HPP__

