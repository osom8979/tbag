/**
 * @file   UxLoop.hpp
 * @brief  UxLoop class prototype.
 * @author zer0
 * @date   2018-11-07
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXLOOP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXLOOP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/uvpp/Loop.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvxx {

/**
 * UxLoop class prototype.
 *
 * @author zer0
 * @date   2018-11-07
 */
class TBAG_API UxLoop
{
public:
    struct Impl;
    friend struct Impl;

public:
    using SharedImpl = std::shared_ptr<Impl>;
    using Loop       = libtbag::uvpp::Loop;

private:
    SharedImpl _impl;

public:
    UxLoop();
    UxLoop(std::nullptr_t) TBAG_NOEXCEPT;
    UxLoop(UxLoop const & obj) TBAG_NOEXCEPT;
    UxLoop(UxLoop && obj) TBAG_NOEXCEPT;
    ~UxLoop();

public:
    UxLoop & operator =(UxLoop const & obj) TBAG_NOEXCEPT;
    UxLoop & operator =(UxLoop && obj) TBAG_NOEXCEPT;

public:
    void copy(UxLoop const & obj) TBAG_NOEXCEPT;
    void swap(UxLoop & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(UxLoop & lh, UxLoop & rh) TBAG_NOEXCEPT { lh.swap(rh); }

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
    friend inline bool operator <(UxLoop const & x, UxLoop const & y) TBAG_NOEXCEPT
    {
        return x.get() < y.get();
    }

    inline bool operator ==(UxLoop const & obj) const TBAG_NOEXCEPT
    {
        return get() == obj.get();
    }

    inline bool operator !=(UxLoop const & obj) const TBAG_NOEXCEPT
    {
        return get() != obj.get();
    }

public:
    void reset();

public:
    Loop * getLoop();
    Loop const * getLoop() const;

public:
    bool isRunning() const;

public:
    Err runDefault();
    Err RunOnce();
    Err RunNowait();
};

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXLOOP_HPP__

