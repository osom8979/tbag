/**
 * @file   Property.hpp
 * @brief  Property class prototype.
 * @author zer0
 * @date   2016-04-12
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_DOM_PROPERTY_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_DOM_PROPERTY_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/dom/Resource.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace dom {

/**
 * Property class prototype.
 *
 * @author zer0
 * @date   2016-04-12
 */
class TBAG_API Property : public Noncopyable
{
public:
    static TBAG_CONSTEXPR char const * const getDefaultTagName() TBAG_NOEXCEPT
    { return "property"; }

private:
    Resource _res;

public:
    Property();
    virtual ~Property();

protected:
    inline Resource & getResource()
    { return _res; }
    inline Resource const & getResource() const
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

} // namespace dom

#ifndef CREATE_PROPERTY
/** Create a property accessor & mutator macro. */
#define CREATE_PROPERTY(type, name, default_value)                          \
    public:                                                                 \
        type get_##name() const {                                           \
            return getResource().get(std::string(#name)       \
                                   , static_cast<type>(default_value));     \
        }                                                                   \
        void set_##name(type const & value = default_value) {               \
            getResource().set(std::string(#name)              \
                            , static_cast<type>(value));                    \
        }
#endif

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_DOM_PROPERTY_HPP__

