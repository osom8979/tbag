/**
 * @file   ExtraPref.cpp
 * @brief  ExtraPref class implementation.
 * @author zer0
 * @date   2019-11-05
 */

#include <libtbag/pref/details/ExtraPref.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace pref    {
namespace details {

ExtraPref::ExtraPref()
{
    // EMPTY.
}

ExtraPref::~ExtraPref()
{
    // EMPTY.
}

std::string ExtraPref::name() const
{
    return ROOT_NAME;
}

bool ExtraPref::init()
{
    return true;
}

void ExtraPref::clear()
{
    _xml.clear();
}

void ExtraPref::load(Element const & element)
{
    writeElementToXmlText(element, _xml);
}

void ExtraPref::save(Element & element) const
{
    Document doc;
    if (isSuccess(XmlHelper::readFromXmlText(doc, _xml))) {
        element.InsertEndChild(doc.DeepClone(element.GetDocument()));
    }
}

} // namespace details
} // namespace pref

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

