/**
 * @file   Property.hpp
 * @brief  Property class prototype.
 * @author zer0
 * @date   2016-04-12
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_DOM_XML_PROPERTY_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_DOM_XML_PROPERTY_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/dom/xml/Resource.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace dom {
namespace xml {

/**
 * Property class prototype.
 *
 * @author zer0
 * @date   2016-04-12
 */
class TBAG_API Property : private Noncopyable
{
public:
    static TBAG_CONSTEXPR char const * default_root_name() TBAG_NOEXCEPT
    { return "resource"; }
    static TBAG_CONSTEXPR char const * default_tag_name() TBAG_NOEXCEPT
    { return "property"; }

private:
    Resource _res;

public:
    Property();
    virtual ~Property();

protected:
    inline Resource & res() TBAG_NOEXCEPT
    { return _res; }
    inline Resource const & res() const TBAG_NOEXCEPT
    { return _res; }

public:
    void setDefault();

public:
    bool load(std::string const & path);
    bool save(std::string const & path);
    bool loadOrCreate(std::string const & path);

public:
    virtual void updateDefault()
    { /* EMPTY. */ }
};

} // namespace xml
} // namespace dom

/**
 * @def CREATE_PROPERTY
 *
 * Create a property accessor & mutator macro.
 */
#ifndef CREATE_PROPERTY
#define CREATE_PROPERTY(type, name, default_value)                                  \
    public:                                                                         \
        type get_##name() const {                                                   \
            return res().opt(std::string(#name), static_cast<type>(default_value)); \
        }                                                                           \
        void set_##name(type const & value = default_value) {                       \
            res().set(std::string(#name), static_cast<type>(value));                \
        }
#endif

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_DOM_XML_PROPERTY_HPP__

