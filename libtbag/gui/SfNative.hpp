/**
 * @file   SfNative.hpp
 * @brief  SfNative class prototype.
 * @author zer0
 * @date   2019-02-12
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GUI_NATIVE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GUI_NATIVE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/container/Pointer.hpp>
#include <libtbag/graphic/Color.hpp>
#include <libtbag/geometry/Point2.hpp>
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
    using Pointer = libtbag::container::Pointer<void>;

    using Channel = libtbag::graphic::Channel;
    using Rgb24   = libtbag::graphic::Rgb24;
    using Rgb32   = libtbag::graphic::Rgb32;
    using Color   = libtbag::graphic::Color;

    using Pointb  = libtbag::geometry::Pointb;
    using Pointc  = libtbag::geometry::Pointc;
    using Points  = libtbag::geometry::Points;
    using Pointi  = libtbag::geometry::Pointi;
    using Pointu  = libtbag::geometry::Pointu;
    using Pointl  = libtbag::geometry::Pointl;
    using Pointll = libtbag::geometry::Pointll;
    using Pointf  = libtbag::geometry::Pointf;
    using Pointd  = libtbag::geometry::Pointd;
    using Point   = libtbag::geometry::Point;

    using Sizeb  = libtbag::geometry::Sizeb;
    using Sizec  = libtbag::geometry::Sizec;
    using Sizes  = libtbag::geometry::Sizes;
    using Sizei  = libtbag::geometry::Sizei;
    using Sizeu  = libtbag::geometry::Sizeu;
    using Sizel  = libtbag::geometry::Sizel;
    using Sizell = libtbag::geometry::Sizell;
    using Sizef  = libtbag::geometry::Sizef;
    using Sized  = libtbag::geometry::Sized;
    using Size   = libtbag::geometry::Size;

    using Rectb  = libtbag::geometry::Rectb;
    using Rectc  = libtbag::geometry::Rectc;
    using Rects  = libtbag::geometry::Rects;
    using Recti  = libtbag::geometry::Recti;
    using Rectu  = libtbag::geometry::Rectu;
    using Rectl  = libtbag::geometry::Rectl;
    using Rectll = libtbag::geometry::Rectll;
    using Rectf  = libtbag::geometry::Rectf;
    using Rectd  = libtbag::geometry::Rectd;
    using Rect   = libtbag::geometry::Rect;

public:
    TBAG_CONSTEXPR static Channel const CHANNEL_MIN = libtbag::graphic::channel_min();
    TBAG_CONSTEXPR static Channel const CHANNEL_MAX = libtbag::graphic::channel_max();

public:
    struct no_init_t { /* EMPTY. */ };
    struct no_init_no_ref_t { /* EMPTY. */ };

public:
    TBAG_CONSTEXPR static no_init_t const no_init = no_init_t{};
    TBAG_CONSTEXPR static no_init_no_ref_t const no_init_no_ref = no_init_no_ref_t{};

private:
    SfType _type;
    bool _ref;

public:
    explicit SfNative(SfType type, no_init_no_ref_t);
    explicit SfNative(SfType type, no_init_t, bool ref = true);
    explicit SfNative(SfType type, bool ref = true);
    SfNative(SfNative && obj) TBAG_NOEXCEPT;
    ~SfNative();

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

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GUI_NATIVE_HPP__

