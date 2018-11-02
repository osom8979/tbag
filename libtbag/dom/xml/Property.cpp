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
    _res.setTag(getDefaultTagName());
}

Property::~Property()
{
    // EMPTY.
}

void Property::setDefault()
{
    _res.clear();
    _res.setRoot(getDefaultRootName());
    _res.setTag(getDefaultTagName());
    updateDefault();
}

bool Property::load(std::string const & path)
{
    return _res.readFile(path);
}

bool Property::save(std::string const & path)
{
    return _res.save(path);
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

