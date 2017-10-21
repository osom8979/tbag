/**
 * @file   Format.hpp
 * @brief  Extended formatting string.
 * @author zer0
 * @date   2017-10-21
 *
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_STRING_FORMAT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_STRING_FORMAT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace string {

/**
 * Extended Format class prototype.
 *
 * @author zer0
 * @date   2017-10-21
 */
class TBAG_API Format
{
public:
    Format();
    Format(Format const & obj);
    Format(Format && obj);
    virtual ~Format();

public:
    Format & operator =(Format const & obj);
    Format & operator =(Format && obj);
};

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_STRING_FORMAT_HPP__

