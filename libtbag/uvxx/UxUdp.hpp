/**
 * @file   UxUdp.hpp
 * @brief  UxUdp class prototype.
 * @author zer0
 * @date   2018-11-07
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXUDP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXUDP_HPP__

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
 * UxUdp class prototype.
 *
 * @author zer0
 * @date   2018-11-07
 */
class TBAG_API UxUdp
{
public:
    struct Impl;
    friend struct Impl;

public:
    using SharedImpl = std::shared_ptr<Impl>;

private:
    SharedImpl _impl;

public:
    UxUdp();
    UxUdp(std::nullptr_t) TBAG_NOEXCEPT;
    UxUdp(UxUdp const & obj) TBAG_NOEXCEPT;
    UxUdp(UxUdp && obj) TBAG_NOEXCEPT;
    ~UxUdp();

public:
    UxUdp & operator =(UxUdp const & obj) TBAG_NOEXCEPT;
    UxUdp & operator =(UxUdp && obj) TBAG_NOEXCEPT;

public:
    void copy(UxUdp const & obj) TBAG_NOEXCEPT;
    void swap(UxUdp & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(UxUdp & lh, UxUdp & rh) TBAG_NOEXCEPT { lh.swap(rh); }

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
    friend inline bool operator <(UxUdp const & x, UxUdp const & y) TBAG_NOEXCEPT
    {
        return x.get() < y.get();
    }

    inline bool operator ==(UxUdp const & obj) const TBAG_NOEXCEPT
    {
        return get() == obj.get();
    }

    inline bool operator !=(UxUdp const & obj) const TBAG_NOEXCEPT
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

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXUDP_HPP__

