/**
 * @file   BlendMode.hpp
 * @brief  BlendMode class prototype.
 * @author zer0
 * @date   2019-02-19
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GUI_BLENDMODE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GUI_BLENDMODE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/gui/SfNative.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gui {

/**
 * BlendMode class prototype.
 *
 * @author zer0
 * @date   2019-02-19
 */
class TBAG_API BlendMode : public SfNative
{
public:
    enum class Factor
    {
        F_ZERO,
        F_ONE,
        F_SRC_COLOR,
        F_ONE_MINUS_SRC_COLOR,
        F_DST_COLOR,
        F_ONE_MINUS_DST_COLOR,
        F_SRC_ALPHA,
        F_ONE_MINUS_SRC_ALPHA,
        F_DST_ALPHA,
        F_ONE_MINUS_DST_ALPHA
    };

    enum class Equation
    {
        E_ADD,
        E_SUBTRACT,
        E_REVERSE_SUBTRACT
    };

public:
    BlendMode();
    BlendMode(Factor source, Factor destination, Equation blend = Equation::E_ADD);
    BlendMode(Factor color_source,
              Factor color_destination,
              Equation color_blend,
              Factor alpha_source,
              Factor alpha_destination,
              Equation alpha_blend);
    BlendMode(void * handle, no_init_no_ref_t);
    BlendMode(BlendMode const & obj);
    BlendMode(BlendMode && obj) TBAG_NOEXCEPT;
    virtual ~BlendMode();

public:
    BlendMode & operator =(BlendMode const & obj);
    BlendMode & operator =(BlendMode && obj) TBAG_NOEXCEPT;

public:
    void swap(BlendMode & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(BlendMode & lh, BlendMode & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    void     setColorSrcFactor(Factor f) TBAG_NOEXCEPT;
    Factor   getColorSrcFactor() const TBAG_NOEXCEPT;
    void     setColorDstFactor(Factor f) TBAG_NOEXCEPT;
    Factor   getColorDstFactor() const TBAG_NOEXCEPT;
    void     setColorEquation (Equation e) TBAG_NOEXCEPT;
    Equation getColorEquation () const TBAG_NOEXCEPT;

public:
    void     setAlphaSrcFactor(Factor f) TBAG_NOEXCEPT;
    Factor   getAlphaSrcFactor() const TBAG_NOEXCEPT;
    void     setAlphaDstFactor(Factor f) TBAG_NOEXCEPT;
    Factor   getAlphaDstFactor() const TBAG_NOEXCEPT;
    void     setAlphaEquation (Equation e) TBAG_NOEXCEPT;
    Equation getAlphaEquation () const TBAG_NOEXCEPT;
};

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GUI_BLENDMODE_HPP__

