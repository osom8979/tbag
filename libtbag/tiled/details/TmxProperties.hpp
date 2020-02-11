/**
 * @file   TmxProperties.hpp
 * @brief  TmxProperties class prototype.
 * @author zer0
 * @date   2019-07-23
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXPROPERTIES_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXPROPERTIES_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/dom/xml/XmlHelper.hpp>
#include <libtbag/tiled/details/TmxProperty.hpp>

#include <string>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

/**
 * TmxProperties class prototype.
 *
 * @author zer0
 * @date   2019-07-23
 */
struct TBAG_API TmxProperties : protected libtbag::dom::xml::XmlHelper
{
    using Properties = std::vector<TmxProperty>;

    TBAG_CONSTEXPR static char const * const TAG_NAME = "properties";

    Properties properties;

    TmxProperties();
    TmxProperties(Properties const & p);
    ~TmxProperties();

    using       iterator = typename Properties::iterator;
    using const_iterator = typename Properties::const_iterator;
    using       reverse_iterator = typename Properties::reverse_iterator;
    using const_reverse_iterator = typename Properties::const_reverse_iterator;

    iterator begin() TBAG_NOEXCEPT_SP_OP(properties.begin())
    { return properties.begin(); }
    iterator end() TBAG_NOEXCEPT_SP_OP(properties.end())
    { return properties.end(); }

    const_iterator begin() const TBAG_NOEXCEPT_SP_OP(properties.begin())
    { return properties.begin(); }
    const_iterator end() const TBAG_NOEXCEPT_SP_OP(properties.end())
    { return properties.end(); }

    const_iterator cbegin() const TBAG_NOEXCEPT_SP_OP(properties.cbegin())
    { return properties.cbegin(); }
    const_iterator cend() const TBAG_NOEXCEPT_SP_OP(properties.cend())
    { return properties.cend(); }

    reverse_iterator rbegin() TBAG_NOEXCEPT_SP_OP(properties.rbegin())
    { return properties.rbegin(); }
    reverse_iterator rend() TBAG_NOEXCEPT_SP_OP(properties.rend())
    { return properties.rend(); }

    const_reverse_iterator rbegin() const TBAG_NOEXCEPT_SP_OP(properties.rbegin())
    { return properties.rbegin(); }
    const_reverse_iterator rend() const TBAG_NOEXCEPT_SP_OP(properties.rend())
    { return properties.rend(); }

    const_reverse_iterator crbegin() const TBAG_NOEXCEPT_SP_OP(properties.crbegin())
    { return properties.crbegin(); }
    const_reverse_iterator crend() const TBAG_NOEXCEPT_SP_OP(properties.crend())
    { return properties.crend(); }

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

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXPROPERTIES_HPP__

