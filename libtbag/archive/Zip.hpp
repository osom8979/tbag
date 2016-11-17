/**
 * @file   Zip.hpp
 * @brief  Zip class prototype.
 * @author zer0
 * @date   2016-11-17
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_ARCHIVE_ZIP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_ARCHIVE_ZIP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace archive {

/**
 * Zip class prototype.
 *
 * @author zer0
 * @date   2016-11-17
 */
class TBAG_API Zip : public Noncopyable
{
public:
    Zip();
    ~Zip();
};

} // namespace archive

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ARCHIVE_ZIP_HPP__

