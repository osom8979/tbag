/**
 * @file   Shader.hpp
 * @brief  Shader class prototype.
 * @author zer0
 * @date   2019-02-19
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GUI_SHADER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GUI_SHADER_HPP__

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
 * Shader class prototype.
 *
 * @author zer0
 * @date   2019-02-19
 */
class TBAG_API Shader : public SfNative
{
public:
    Shader();
    Shader(Shader && obj) TBAG_NOEXCEPT;
    ~Shader();

public:
    Shader & operator =(Shader && obj) TBAG_NOEXCEPT;

public:
    void swap(Shader & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(Shader & lh, Shader & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }
};

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GUI_SHADER_HPP__

