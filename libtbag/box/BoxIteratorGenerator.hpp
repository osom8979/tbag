/**
 * @file   BoxIteratorGenerator.hpp
 * @brief  BoxIteratorGenerator class prototype.
 * @author zer0
 * @date   2019-12-30
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_BOX_BOXITERATORGENERATOR_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_BOX_BOXITERATORGENERATOR_HPP__

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

namespace box {

/**
 * BoxIteratorGenerator class prototype.
 *
 * @author zer0
 * @date   2019-12-30
 */
class TBAG_API BoxIteratorGenerator : private Noncopyable
{
private:
    // Insert member variables.

public:
    BoxIteratorGenerator();
    virtual ~BoxIteratorGenerator();
};

} // namespace box

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_BOX_BOXITERATORGENERATOR_HPP__

