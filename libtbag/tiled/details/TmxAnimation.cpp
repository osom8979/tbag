/**
 * @file   TmxAnimation.cpp
 * @brief  TmxAnimation class implementation.
 * @author zer0
 * @date   2019-07-23
 */

#include <libtbag/tiled/details/TmxAnimation.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cstring>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

TmxAnimation::TmxAnimation()
{
    // EMPTY.
}

TmxAnimation::TmxAnimation(Frames const & f) : frames(f)
{
    // EMPTY.
}

TmxAnimation::~TmxAnimation()
{
    // EMPTY.
}

bool TmxAnimation::empty() const
{
    return frames.empty();
}

std::size_t TmxAnimation::size() const
{
    return frames.size();
}

Err TmxAnimation::read(Element const & elem)
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }

    foreachElement(elem, TmxFrame::TAG_NAME, [&](Element const & e){
        TmxFrame frame;
        if (isSuccess(frame.read(e))) {
            frames.push_back(std::move(frame));
        }
    });

    return E_SUCCESS;
}

Err TmxAnimation::read(std::string const & xml)
{
    Document doc;
    auto const CODE = readFromXmlText(doc, xml);
    if (isFailure(CODE)) {
        return CODE;
    }
    auto const * elem = doc.FirstChildElement(TAG_NAME);
    assert(elem != nullptr);
    return read(*elem);
}

Err TmxAnimation::write(Element & elem) const
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }

    for (auto & frame : frames) {
        newElement(elem, TmxFrame::TAG_NAME, [&](Element & p){
            frame.write(p);
        });
    }

    return E_SUCCESS;
}

Err TmxAnimation::write(std::string & xml) const
{
    Document doc;
    auto * new_elem = newElement(doc, TAG_NAME);
    assert(new_elem != nullptr);
    auto const CODE = write(*new_elem);
    if (isFailure(CODE)) {
        return CODE;
    }
    insertElement(doc, new_elem);
    return writeToXmlText(doc, xml);
}

} // namespace details
} // namespace tiled

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

