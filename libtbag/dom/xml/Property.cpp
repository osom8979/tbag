/**
 * @file   Property.cpp
 * @brief  Property class implementation.
 * @author zer0
 * @date   2016-10-30
 */

#include <libtbag/dom/xml/Property.hpp>
#include <libtbag/filesystem/Path.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace dom {
namespace xml {

Property::Property()
{
    _res.tag = default_tag_name();
}

Property::~Property()
{
    // EMPTY.
}

void Property::setDefault()
{
    _res.clear();
    _res.root = default_root_name();
    _res.tag = default_tag_name();
    updateDefault();
}

bool Property::load(std::string const & path)
{
    return _res.readFromXmlFile(path);
}

bool Property::save(std::string const & path)
{
    return _res.saveToXmlFile(path);
}

bool Property::loadOrCreate(std::string const & path)
{
    if (filesystem::Path(path).exists()) {
        return load(path);
    }
    setDefault();
    return save(path);
}

} // namespace xml
} // namespace dom

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

