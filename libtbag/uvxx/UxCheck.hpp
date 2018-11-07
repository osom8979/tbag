/**
 * @file   UxCheck.hpp
 * @brief  UxCheck class prototype.
 * @author zer0
 * @date   2018-11-07
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXCHECK_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXCHECK_HPP__

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

namespace uvxx {

/**
 * UxCheck class prototype.
 *
 * @author zer0
 * @date   2018-11-07
 */
class TBAG_API UxCheck
{
public:
    struct Impl;
    friend struct Impl;

public:
    using SharedImpl = std::shared_ptr<Impl>;

private:
    SharedImpl _impl;

public:
    UxCheck();
    UxCheck(std::nullptr_t) TBAG_NOEXCEPT;
    UxCheck(UxCheck const & obj) TBAG_NOEXCEPT;
    UxCheck(UxCheck && obj) TBAG_NOEXCEPT;
    ~UxCheck();

public:
    UxCheck & operator =(UxCheck const & obj) TBAG_NOEXCEPT;
    UxCheck & operator =(UxCheck && obj) TBAG_NOEXCEPT;

public:
    void copy(UxCheck const & obj) TBAG_NOEXCEPT;
    void swap(UxCheck & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(UxCheck & lh, UxCheck & rh) TBAG_NOEXCEPT { lh.swap(rh); }

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
    friend inline bool operator <(UxCheck const & x, UxCheck const & y) TBAG_NOEXCEPT
    {
        return x.get() < y.get();
    }

    inline bool operator ==(UxCheck const & obj) const TBAG_NOEXCEPT
    {
        return get() == obj.get();
    }

    inline bool operator !=(UxCheck const & obj) const TBAG_NOEXCEPT
    {
        return get() != obj.get();
    }

public:
    void reset();
};

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXCHECK_HPP__

