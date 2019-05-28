/**
 * @file   TmxJson.cpp
 * @brief  TmxJson class implementation.
 * @author zer0
 * @date   2019-05-29
 */

#include <libtbag/tiled/TmxJson.hpp>
#include <libtbag/log/Log.hpp>

#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled {

TmxJson::TmxJson()
{
    // EMPTY.
}

TmxJson::TmxJson(TmxJson const & obj) : TmxJson()
{
    (*this) = obj;
}

TmxJson::TmxJson(TmxJson && obj) TBAG_NOEXCEPT : TmxJson()
{
    (*this) = std::move(obj);
}

TmxJson::~TmxJson()
{
    // EMPTY.
}

TmxJson & TmxJson::operator =(TmxJson const & obj)
{
    copy(obj);
    return *this;
}

TmxJson & TmxJson::operator =(TmxJson && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void TmxJson::copy(TmxJson const & obj)
{
    if (this != &obj) {
        // TODO: Copy object.
    }
}

void TmxJson::swap(TmxJson & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        // TODO: Swap object.
    }
}

} // namespace tiled

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

