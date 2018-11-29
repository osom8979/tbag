/**
 * @file   rstr.cpp
 * @brief  rstr class implementation.
 * @author zer0
 * @date   2018-11-29
 */

#include <libtbag/rstl/rstr.hpp>
#include <libtbag/log/Log.hpp>

#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace rstl {

rstr::rstr()
{
    // EMPTY.
}

rstr::rstr(rstr const & obj) : rstr()
{
    (*this) = obj;
}

rstr::rstr(rstr && obj) TBAG_NOEXCEPT : rstr()
{
    (*this) = std::move(obj);
}

rstr::~rstr()
{
    // EMPTY.
}

rstr & rstr::operator =(rstr const & obj)
{
    copy(obj);
    return *this;
}

rstr & rstr::operator =(rstr && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void rstr::copy(rstr const & obj)
{
    if (this != &obj) {
        // TODO: Copy object.
    }
}

void rstr::swap(rstr & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        // TODO: Swap object.
    }
}

} // namespace rstl

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

