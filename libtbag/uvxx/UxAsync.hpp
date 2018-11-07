/**
 * @file   UxAsync.hpp
 * @brief  UxAsync class prototype.
 * @author zer0
 * @date   2018-11-07
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXASYNC_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXASYNC_HPP__

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
 * UxAsync class prototype.
 *
 * @author zer0
 * @date   2018-11-07
 */
class TBAG_API UxAsync
{
public:
    struct Impl;
    friend struct Impl;

public:
    using SharedImpl = std::shared_ptr<Impl>;

private:
    SharedImpl _impl;

public:
    UxAsync();
    UxAsync(std::nullptr_t) TBAG_NOEXCEPT;
    UxAsync(UxAsync const & obj) TBAG_NOEXCEPT;
    UxAsync(UxAsync && obj) TBAG_NOEXCEPT;
    ~UxAsync();

public:
    UxAsync & operator =(UxAsync const & obj) TBAG_NOEXCEPT;
    UxAsync & operator =(UxAsync && obj) TBAG_NOEXCEPT;

public:
    void copy(UxAsync const & obj) TBAG_NOEXCEPT;
    void swap(UxAsync & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(UxAsync & lh, UxAsync & rh) TBAG_NOEXCEPT { lh.swap(rh); }

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
    friend inline bool operator <(UxAsync const & x, UxAsync const & y) TBAG_NOEXCEPT
    {
        return x.get() < y.get();
    }

    inline bool operator ==(UxAsync const & obj) const TBAG_NOEXCEPT
    {
        return get() == obj.get();
    }

    inline bool operator !=(UxAsync const & obj) const TBAG_NOEXCEPT
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

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXASYNC_HPP__

