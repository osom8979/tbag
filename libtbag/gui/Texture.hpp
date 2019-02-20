/**
 * @file   Texture.hpp
 * @brief  Texture class prototype.
 * @author zer0
 * @date   2019-02-19
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GUI_TEXTURE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GUI_TEXTURE_HPP__

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
 * Texture class prototype.
 *
 * @author zer0
 * @date   2019-02-19
 */
class TBAG_API Texture : public SfNative
{
public:
    Texture();
    Texture(void * handle, no_init_no_ref_t);
    Texture(Texture && obj) TBAG_NOEXCEPT;
    ~Texture();

public:
    Texture & operator =(Texture && obj) TBAG_NOEXCEPT;

public:
    void swap(Texture & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(Texture & lh, Texture & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }
};

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GUI_TEXTURE_HPP__

