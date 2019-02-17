/**
 * @file   SfRenderTarget.hpp
 * @brief  SfRenderTarget class prototype.
 * @author zer0
 * @date   2019-02-16
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_SFPP_SFRENDERTARGET_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_SFPP_SFRENDERTARGET_HPP__

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
 * SfRenderTarget class prototype.
 *
 * @author zer0
 * @date   2019-02-16
 */
class TBAG_API SfRenderTarget : public SfNative
{
public:
    explicit SfRenderTarget(SfType type, no_init_t, bool ref = true);
    explicit SfRenderTarget(SfType type, bool ref = true);
    virtual ~SfRenderTarget();
};

} // namespace sfpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SFPP_SFRENDERTARGET_HPP__

