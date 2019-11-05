/**
 * @file   ValuesPref.cpp
 * @brief  ValuesPref class implementation.
 * @author zer0
 * @date   2019-11-05
 */

#include <libtbag/pref/details/ValuesPref.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace pref    {
namespace details {

ValuesPref::ValuesPref() : _res(ROOT_NAME, PROP_NAME, ATTR_NAME)
{
    // EMPTY.
}

ValuesPref::~ValuesPref()
{
    // EMPTY.
}

std::string ValuesPref::name() const
{
    return _res.root;
}

bool ValuesPref::init()
{
    return true;
}

void ValuesPref::clear()
{
    _res.clear();
}

void ValuesPref::load(Element const & element)
{
    _res.readFromXmlElement(element);
}

void ValuesPref::save(Element & element) const
{
    _res.saveToXmlPropElements(element);
}

ValuesPref::Resource ValuesPref::getResource() const
{
    return _res;
}

} // namespace details
} // namespace pref

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

