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

    using       iterator = typename WangSets::iterator;
    using const_iterator = typename WangSets::const_iterator;
    using       reverse_iterator = typename WangSets::reverse_iterator;
    using const_reverse_iterator = typename WangSets::const_reverse_iterator;

    iterator begin() TBAG_NOEXCEPT_SP_OP(wang_sets.begin())
    { return wang_sets.begin(); }
    iterator end() TBAG_NOEXCEPT_SP_OP(wang_sets.end())
    { return wang_sets.end(); }

    const_iterator begin() const TBAG_NOEXCEPT_SP_OP(wang_sets.begin())
    { return wang_sets.begin(); }
    const_iterator end() const TBAG_NOEXCEPT_SP_OP(wang_sets.end())
    { return wang_sets.end(); }

    const_iterator cbegin() const TBAG_NOEXCEPT_SP_OP(wang_sets.cbegin())
    { return wang_sets.cbegin(); }
    const_iterator cend() const TBAG_NOEXCEPT_SP_OP(wang_sets.cend())
    { return wang_sets.cend(); }

    reverse_iterator rbegin() TBAG_NOEXCEPT_SP_OP(wang_sets.rbegin())
    { return wang_sets.rbegin(); }
    reverse_iterator rend() TBAG_NOEXCEPT_SP_OP(wang_sets.rend())
    { return wang_sets.rend(); }

    const_reverse_iterator rbegin() const TBAG_NOEXCEPT_SP_OP(wang_sets.rbegin())
    { return wang_sets.rbegin(); }
    const_reverse_iterator rend() const TBAG_NOEXCEPT_SP_OP(wang_sets.rend())
    { return wang_sets.rend(); }

    const_reverse_iterator crbegin() const TBAG_NOEXCEPT_SP_OP(wang_sets.crbegin())
    { return wang_sets.crbegin(); }
    const_reverse_iterator crend() const TBAG_NOEXCEPT_SP_OP(wang_sets.crend())
    { return wang_sets.crend(); }

    bool empty() const;
    std::size_t size() const;
    void clear();

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

