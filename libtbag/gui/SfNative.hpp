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

public:
    TBAG_CONSTEXPR static Channel const CHANNEL_MIN = libtbag::graphic::channel_min();
    TBAG_CONSTEXPR static Channel const CHANNEL_MAX = libtbag::graphic::channel_max();

public:
    struct no_init_t { /* EMPTY. */ };

public:
    TBAG_CONSTEXPR static no_init_t const no_init = no_init_t{};

private:
    SfType const TYPE;
    void * _user;

public:
    explicit SfNative(SfType type, no_init_t);
    explicit SfNative(SfType type);
    explicit SfNative(int type);
    virtual ~SfNative();

public:
    // @formatter:off
    inline void         setUserData(void * data) TBAG_NOEXCEPT { _user = data; }
    inline void       * getUserData()            TBAG_NOEXCEPT { return _user; }
    inline void const * getUserData()      const TBAG_NOEXCEPT { return _user; }
    // @formatter:on

public:
    inline char const * const getTypeName() TBAG_NOEXCEPT
    {
        return libtbag::gui::getSfTypeName(TYPE);
    }
};

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GUI_SFNATIVE_HPP__

