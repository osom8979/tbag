/**
 * @file   NumString.cpp
 * @brief  NumString class implementation.
 * @author zer0
 * @date   2018-07-14
 */

#include <libtbag/string/NumString.hpp>
#include <libtbag/log/Log.hpp>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace string {

NumString::NumString() : _str()
{
    // EMPTY.
}

NumString::NumString(std::string const & str) : _str(str)
{
    // EMPTY.
}

NumString::NumString(char const * str) : _str(str)
{
    // EMPTY.
}

NumString::NumString(NumString const & obj)
{
    (*this) = obj;
}

NumString::NumString(NumString && obj)
{
    (*this) = std::move(obj);
}

NumString::~NumString()
{
    // EMPTY.
}

NumString & NumString::operator =(NumString const & obj)
{
    copy(obj);
    return *this;
}

NumString & NumString::operator =(NumString && obj)
{
    swap(obj);
    return *this;
}

void NumString::copy(NumString const & obj)
{
    if (this != &obj) {
        _str = obj._str;
    }
}

void NumString::swap(NumString & obj)
{
    if (this != &obj) {
        _str.swap(obj._str);
    }
}

std::string NumString::toString() const
{
    return _str;
}

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

