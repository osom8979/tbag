/**
 * @file   Font.hpp
 * @brief  Font class prototype.
 * @author zer0
 * @date   2017-11-16
 *
 * @see <https://en.wikipedia.org/wiki/TrueType>
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TYPOGRAPHY_FONT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TYPOGRAPHY_FONT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/util/BufferInfo.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace typography {

/**
 * Font class prototype
 *
 * @author zer0
 * @date   2017-11-16
 */
class Font : private Noncopyable
{
public:
    Font();
    virtual ~Font();
};

/** Read TrueType font file. */
TBAG_API Err readTrueType(std::string const & path, Font & font);

/** Save TrueType font file. */
TBAG_API Err saveTrueType(std::string const & path, Font const & font);

/** String format. */
TBAG_API std::string drawTrueType(util::Buffer const & true_type, char c, int scale = 12);
TBAG_API std::string drawTrueType(util::Buffer const & true_type, std::string const & text, int scale = 12);

TBAG_API std::string  getCompressedProggyClean();
TBAG_API std::string  getProggyCleanMd5();
TBAG_API util::Buffer getProggyClean();

} // namespace typography

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TYPOGRAPHY_FONT_HPP__

