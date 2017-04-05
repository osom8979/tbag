/**
 * @file   DateTime.hpp
 * @brief  DateTime class prototype.
 * @author zer0
 * @date   2017-04-05
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TIME_DATETIME_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TIME_DATETIME_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
// Include preprocessor.

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace time {

// Forward declaration.

/**
 * DateTime class prototype.
 *
 * @author zer0
 * @date   2017-04-05
 */
class TBAG_API DateTime : public Noncopyable
{
private:
    // Insert member variables.

public:
    DateTime();
    virtual ~DateTime();
};

} // namespace time

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TIME_DATETIME_HPP__

