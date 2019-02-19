/**
 * @file   SfShader.hpp
 * @brief  SfShader class prototype.
 * @author zer0
 * @date   2019-02-19
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_SFPP_SFSHADER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_SFPP_SFSHADER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/sfpp/SfNative.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace sfpp {

/**
 * SfShader class prototype.
 *
 * @author zer0
 * @date   2019-02-19
 */
class TBAG_API SfShader : public SfNative
{
public:
    SfShader();
    SfShader(SfShader && obj) TBAG_NOEXCEPT;
    ~SfShader();

public:
    SfShader & operator =(SfShader && obj) TBAG_NOEXCEPT;

public:
    void swap(SfShader & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(SfShader & lh, SfShader & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }
};

} // namespace sfpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SFPP_SFSHADER_HPP__

