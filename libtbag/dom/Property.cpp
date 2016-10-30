/**
 * @file   Property.cpp
 * @brief  Property class implementation.
 * @author zer0
 * @date   2016-10-30
 */

#include <libtbag/dom/Property.hpp>
#include <libtbag/filesystem/Common.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace dom {

Property::Property()
{
    _res.set_tag(getDefaultTagName());
}

Property::~Property()
{
    // EMPTY.
}

void Property::setDefault()
{
    _res.clear();
    _res.set_tag(getDefaultTagName());
    updateDefault();
}

bool Property::load(std::string const & path)
{
    return _res.readFile(path, getDefaultTagName());
}

bool Property::save(std::string const & path)
{
    return _res.save(path);
}

bool Property::loadOrCreate(std::string const & path)
{
    if (filesystem::common::isExistsMode(path)) {
        return load(path);
    }

    setDefault();
    return save(path);
}

} // namespace dom

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

