/**
 * @file   Archive.hpp
 * @brief  Archive class prototype.
 * @author zer0
 * @date   2019-02-25
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_ARCHIVE_ARCHIVE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_ARCHIVE_ARCHIVE_HPP__

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
 * Archive class prototype.
 *
 * @author zer0
 * @date   2019-02-25
 */
class TBAG_API Archive : private Noncopyable
{
public:
    Archive();
    virtual ~Archive();
};

} // namespace archive

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ARCHIVE_ARCHIVE_HPP__
