/**
 * @file   ValuesPref.hpp
 * @brief  ValuesPref class prototype.
 * @author zer0
 * @date   2019-11-05
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PREF_DETAILS_VALUESPREF_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PREF_DETAILS_VALUESPREF_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/pref/Preferences.hpp>
#include <libtbag/dom/xml/Resource.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace pref    {
namespace details {

/**
 * ValuesPref class prototype.
 *
 * XML format:
 * @code{.xml}
 *  <values>
 *    <!-- insert your properties -->
 *    <value name='key1'>value1</value>
 *    <value name='key2'>value2</value>
 *    <value name='key3'>value3</value>
 *    <!-- ... -->
 *  </values>
 * @endcode
 *
 * @author zer0
 * @date   2019-11-05
 */
class TBAG_API ValuesPref : public libtbag::pref::Preferences::NodeInterface
{
public:
    using Resource = libtbag::dom::xml::Resource;

public:
    TBAG_CONSTEXPR static char const * const ROOT_NAME = "values";
    TBAG_CONSTEXPR static char const * const PROP_NAME = "value";
    TBAG_CONSTEXPR static char const * const ATTR_NAME = "name";

private:
    Resource _res;

public:
    ValuesPref();
    virtual ~ValuesPref();

public:
    std::string name() const override;
    bool init() override;
    void clear() override;
    void load(Element const & element) override;
    void save(Element & element) const override;

public:
    Resource getResource() const;
};

} // namespace details
} // namespace pref

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PREF_DETAILS_VALUESPREF_HPP__

