/**
 * @file   TmxWangSets.hpp
 * @brief  TmxWangSets class prototype.
 * @author zer0
 * @date   2019-07-14
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXWANGSETS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXWANGSETS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/dom/xml/XmlHelper.hpp>
#include <libtbag/tiled/details/TmxWangSet.hpp>

#include <string>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

/**
 * TmxWangSets class prototype.
 *
 * @author zer0
 * @date   2019-07-14
 *
 * @remarks
 *  Contains the list of Wang sets defined for this tileset.
 */
struct TBAG_API TmxWangSets : protected libtbag::dom::xml::XmlHelper
{
    using WangSets = std::vector<TmxWangSet>;

    TBAG_CONSTEXPR static char const * const TAG_NAME = "wangsets";

    WangSets wang_sets;

    TmxWangSets();
    TmxWangSets(WangSets const & w);
    ~TmxWangSets();

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

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXWANGSETS_HPP__

