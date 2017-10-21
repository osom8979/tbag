/**
 * @file   Format.cpp
 * @brief  Format class implementation.
 * @author zer0
 * @date   2017-10-21
 */

#include <libtbag/string/Format.hpp>
#include <libtbag/log/Log.hpp>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace string {

Format::Format()
{
    // EMPTY.
}

Format::Format(Format const & obj)
{
    (*this) = obj;
}

Format::Format(Format && obj)
{
    (*this) = std::move(obj);
}

Format::~Format()
{
    // EMPTY.
}

Format & Format::operator =(Format const & obj)
{
    if (this != &obj) {
    }
    return *this;
}

Format & Format::operator =(Format && obj)
{
    if (this != &obj) {
    }
    return *this;
}

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

