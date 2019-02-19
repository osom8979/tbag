/**
 * @file   SfRenderStates.hpp
 * @brief  SfRenderStates class prototype.
 * @author zer0
 * @date   2019-02-19
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_SFPP_SFRENDERSTATES_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_SFPP_SFRENDERSTATES_HPP__

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
 * SfRenderStates class prototype.
 *
 * @author zer0
 * @date   2019-02-19
 */
class TBAG_API SfRenderStates : public SfNative
{
public:
    SfRenderStates();
    SfRenderStates(SfRenderStates && obj) TBAG_NOEXCEPT;
    ~SfRenderStates();

public:
    SfRenderStates & operator =(SfRenderStates && obj) TBAG_NOEXCEPT;

public:
    void swap(SfRenderStates & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(SfRenderStates & lh, SfRenderStates & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }
};

} // namespace sfpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SFPP_SFRENDERSTATES_HPP__

