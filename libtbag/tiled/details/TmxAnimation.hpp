/**
 * @file   TmxAnimation.hpp
 * @brief  TmxAnimation class prototype.
 * @author zer0
 * @date   2019-07-23
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXANIMATION_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXANIMATION_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/dom/xml/XmlHelper.hpp>
#include <libtbag/tiled/details/TmxFrame.hpp>

#include <string>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

/**
 * TmxAnimation class prototype.
 *
 * @author zer0
 * @date   2019-07-23
 */
struct TBAG_API TmxAnimation : protected libtbag::dom::xml::XmlHelper
{
    using Frames = std::vector<TmxFrame>;

    TBAG_CONSTEXPR static char const * const TAG_NAME = "animation";

    Frames frames;

    TmxAnimation();
    TmxAnimation(Frames const & f);
    ~TmxAnimation();

    bool empty() const;
    std::size_t size() const;

    Err read(Element const & elem);
    Err read(std::string const & xml);

    Err write(Element & elem) const;
    Err write(std::string & xml) const;
};

} // namespace details
} // namespace tiled

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXANIMATION_HPP__

