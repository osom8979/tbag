/**
 * @file   Convert.hpp
 * @brief  Convert class prototype.
 * @author zer0
 * @date   2016-07-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TEXT_CONVERT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TEXT_CONVERT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/Noncopyable.hpp>

#include <string>

#include <unicode/ucnv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace text {

/**
 * Convert class prototype.
 *
 * @author zer0
 * @date   2016-07-03
 */
class Convert : public Noncopyable
{
public:
    Convert();
    virtual ~Convert();
};

std::string convertUtf8ToCp949(std::string const & original)
{
    return original;
}

} // namespace text

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TEXT_CONVERT_HPP__

