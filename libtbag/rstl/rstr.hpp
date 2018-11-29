/**
 * @file   rstr.hpp
 * @brief  rstr class prototype.
 * @author zer0
 * @date   2018-11-29
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_RSTL_RSTR_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_RSTL_RSTR_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
// Include preprocessor.

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace rstl {

// Forward declaration.

/**
 * rstr class prototype.
 *
 * @author zer0
 * @date   2018-11-29
 */
class TBAG_API rstr
{
private:
    // Insert member variables.

public:
    rstr();
    rstr(rstr const & obj);
    rstr(rstr && obj) TBAG_NOEXCEPT;
    ~rstr();

public:
    rstr & operator =(rstr const & obj);
    rstr & operator =(rstr && obj) TBAG_NOEXCEPT;

public:
    void copy(rstr const & obj);
    void swap(rstr & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(rstr & lh, rstr & rh) TBAG_NOEXCEPT { lh.swap(rh); }
};

} // namespace rstl

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_RSTL_RSTR_HPP__

