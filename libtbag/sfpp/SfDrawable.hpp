/**
 * @file   SfDrawable.hpp
 * @brief  SfDrawable class prototype.
 * @author zer0
 * @date   2019-02-13
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_SFPP_SFDRAWABLE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_SFPP_SFDRAWABLE_HPP__

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
 * SfDrawable class prototype.
 *
 * @author zer0
 * @date   2019-02-13
 */
class TBAG_API SfDrawable : public SfNative
{
public:
    explicit SfDrawable(SfType type, no_init_t, bool ref = true);
    explicit SfDrawable(SfType type, bool ref = true);
    virtual ~SfDrawable();
};

} // namespace sfpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SFPP_SFDRAWABLE_HPP__

