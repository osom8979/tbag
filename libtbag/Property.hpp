/**
 * @file   Property.hpp
 * @brief  Property class prototype.
 * @author zer0
 * @date   2016-04-12
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PROPERTY_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PROPERTY_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/dom/Resource.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/filesystem/Common.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

/**
 * Property class prototype.
 *
 * @author zer0
 * @date   2016-04-12
 */
class Property : public Noncopyable
{
public:
    static TBAG_CONSTEXPR char const * const getDefaultTagName() TBAG_NOEXCEPT
    { return "property"; }
    static TBAG_CONSTEXPR char const * const getDefaultFileName() TBAG_NOEXCEPT
    { return "property.xml"; }

private:
    dom::Resource _res;

public:
    Property() {
        this->_res.set_tag(getDefaultTagName());
    }

    virtual ~Property() {
        // EMPTY.
    }

protected:
    dom::Resource & getResource() {
        return this->_res;
    }

    dom::Resource const & getResource() const {
        return this->_res;
    }

public:
    void setDefault() {
        this->_res.clear();
        this->_res.set_tag(getDefaultTagName());
        updateDefault();
    }

public:
    bool load(std::string const & path) {
        return this->_res.readFile(path, std::string(getDefaultTagName()));
    }

    bool load() {
        return this->load(Property::getDefaultPath());
    }

public:
    bool save(std::string const & path) {
        return this->_res.save(path);
    }

    bool save() {
        return this->save(Property::getDefaultPath());
    }

public:
    bool loadOrCreate(std::string const & path) {
        using namespace filesystem;
        if (common::isExistsMode(path)) {
            return this->load(path);
        } else {
            this->setDefault();
            return this->save(path);
        }
    }

    bool loadOrCreate() {
        return this->loadOrCreate(Property::getDefaultPath());
    }

public:
    virtual void updateDefault() {
        // EMPTY.
    }

public:
    static std::string getDefaultPath() {
        using Path = filesystem::Path;
        Path path = Path::getExeDir();
        path /= getDefaultFileName();
        return path.getGenericString();
    }
};

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

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PROPERTY_HPP__

