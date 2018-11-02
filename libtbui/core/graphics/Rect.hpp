/**
 * @file   Rect.hpp
 * @brief  Rect class prototype.
 * @author zer0
 * @date   2018-11-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBUI_CORE_GRAPHICS_RECT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBUI_CORE_GRAPHICS_RECT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbui/export.h>

#include <memory>

namespace libtbui  {
namespace core     {
namespace graphics {

/**
 * Rect class prototype.
 *
 * @author zer0
 * @date   2018-11-03
 */
class TBUI_API Rect
{
public:
    struct Impl;
    friend struct Impl;

public:
    using SharedImpl = std::shared_ptr<Impl>;

private:
    SharedImpl _impl;

public:
    Rect();
    Rect(std::nullptr_t) TBAG_NOEXCEPT;
    Rect(Rect const & obj) TBAG_NOEXCEPT;
    Rect(Rect && obj) TBAG_NOEXCEPT;
    ~Rect();

public:
    Rect & operator =(Rect const & obj) TBAG_NOEXCEPT;
    Rect & operator =(Rect && obj) TBAG_NOEXCEPT;

public:
    void copy(Rect const & obj) TBAG_NOEXCEPT;
    void swap(Rect & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(Rect & lh, Rect & rh) TBAG_NOEXCEPT { lh.swap(rh); }

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
    friend inline bool operator <(Rect const & x, Rect const & y) TBAG_NOEXCEPT
    {
        return x.get() < y.get();
    }

    inline bool operator ==(Rect const & obj) const TBAG_NOEXCEPT
    {
        return get() == obj.get();
    }

    inline bool operator !=(Rect const & obj) const TBAG_NOEXCEPT
    {
        return get() != obj.get();
    }

public:
    void reset();
};

} // namespace graphics
} // namespace core
} // namespace libtbui

#endif // __INCLUDE_LIBTBAG__LIBTBUI_CORE_GRAPHICS_RECT_HPP__

