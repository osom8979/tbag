/**
 * @file   UxSignal.hpp
 * @brief  UxSignal class prototype.
 * @author zer0
 * @date   2018-11-07
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXSIGNAL_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXSIGNAL_HPP__

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
 * UxSignal class prototype.
 *
 * @author zer0
 * @date   2018-11-07
 */
class TBAG_API UxSignal
{
public:
    struct Impl;
    friend struct Impl;

public:
    using SharedImpl = std::shared_ptr<Impl>;

private:
    SharedImpl _impl;

public:
    UxSignal();
    UxSignal(std::nullptr_t) TBAG_NOEXCEPT;
    UxSignal(UxSignal const & obj) TBAG_NOEXCEPT;
    UxSignal(UxSignal && obj) TBAG_NOEXCEPT;
    ~UxSignal();

public:
    UxSignal & operator =(UxSignal const & obj) TBAG_NOEXCEPT;
    UxSignal & operator =(UxSignal && obj) TBAG_NOEXCEPT;

public:
    void copy(UxSignal const & obj) TBAG_NOEXCEPT;
    void swap(UxSignal & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(UxSignal & lh, UxSignal & rh) TBAG_NOEXCEPT { lh.swap(rh); }

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
    friend inline bool operator <(UxSignal const & x, UxSignal const & y) TBAG_NOEXCEPT
    {
        return x.get() < y.get();
    }

    inline bool operator ==(UxSignal const & obj) const TBAG_NOEXCEPT
    {
        return get() == obj.get();
    }

    inline bool operator !=(UxSignal const & obj) const TBAG_NOEXCEPT
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

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXSIGNAL_HPP__

