/**
 * @file   SfNative.hpp
 * @brief  SfNative class prototype.
 * @author zer0
 * @date   2019-02-12
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GUI_SFNATIVE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GUI_SFNATIVE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/container/Pointer.hpp>
#include <libtbag/graphic/Color.hpp>
#include <libtbag/geometry/Point.hpp>
#include <libtbag/geometry/Rect.hpp>
#include <libtbag/gui/SfCommon.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gui {

/**
 * SfNative class prototype.
 *
 * @author zer0
 * @date   2019-02-12
 */
class TBAG_API SfNative : public libtbag::container::Pointer<void>, private Noncopyable
{
public:
    using Parent  = libtbag::container::Pointer<void>;

    using Channel = libtbag::graphic::Channel;
    using Rgb24   = libtbag::graphic::Rgb24;
    using Rgb32   = libtbag::graphic::Rgb32;
    using Color   = libtbag::graphic::Color;

    using Pointi = libtbag::geometry::Pointi;
    using Pointu = libtbag::geometry::Pointu;
    using Pointl = libtbag::geometry::Pointl;
    using Pointf = libtbag::geometry::Pointf;
    using Pointd = libtbag::geometry::Pointd;
    using Point  = libtbag::geometry::Point;

    using Sizei = libtbag::geometry::Sizei;
    using Sizeu = libtbag::geometry::Sizeu;
    using Sizel = libtbag::geometry::Sizel;
    using Sizef = libtbag::geometry::Sizef;
    using Sized = libtbag::geometry::Sized;
    using Size  = libtbag::geometry::Size;

    using Recti = libtbag::geometry::Recti;
    using Rectu = libtbag::geometry::Rectu;
    using Rectl = libtbag::geometry::Rectl;
    using Rectf = libtbag::geometry::Rectf;
    using Rectd = libtbag::geometry::Rectd;
    using Rect  = libtbag::geometry::Rect;

public:
    TBAG_CONSTEXPR static Channel const CHANNEL_MIN = libtbag::graphic::channel_min();
    TBAG_CONSTEXPR static Channel const CHANNEL_MAX = libtbag::graphic::channel_max();

public:
    struct no_init_t { /* EMPTY. */ };

public:
    TBAG_CONSTEXPR static no_init_t const no_init = no_init_t{};

private:
    SfType _type;
    bool _ref;

public:
    explicit SfNative(SfType type, no_init_t, bool ref = true);
    explicit SfNative(SfType type, bool ref = true);
    SfNative(SfNative && obj) TBAG_NOEXCEPT;
    virtual ~SfNative();

public:
    SfNative & operator =(SfNative && obj) TBAG_NOEXCEPT;

public:
    void swap(SfNative & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(SfNative & lh, SfNative & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

protected:
    void setHandle(void * handle) TBAG_NOEXCEPT { ptr = handle; }

public:
    inline bool isRef() const TBAG_NOEXCEPT { return _ref; }
    inline void   ref() TBAG_NOEXCEPT { _ref = true; }
    inline void unref() TBAG_NOEXCEPT { _ref = false; }

public:
    inline SfType getType() const TBAG_NOEXCEPT
    { return _type; }

    inline char const * const getTypeName() const TBAG_NOEXCEPT
    { return libtbag::gui::getSfTypeName(_type); }
};

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GUI_SFNATIVE_HPP__

