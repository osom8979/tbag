/**
 * @file   ModelLayer.cpp
 * @brief  ModelLayer class implementation.
 * @author zer0
 * @date   2018-10-22
 */

#include <libtbag/graph/ModelLayer.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace graph {

ModelLayer::ModelLayer(LayerType type) TBAG_NOEXCEPT : ModelLayer(std::make_shared<LayerBase>(type))
{
    assert(static_cast<bool>(_base));
}

ModelLayer::ModelLayer(std::nullptr_t) TBAG_NOEXCEPT : _base(nullptr)
{
    // EMPTY.
}

ModelLayer::ModelLayer(LayerBase * base) TBAG_NOEXCEPT : ModelLayer(SharedBase(base))
{
    assert(static_cast<bool>(_base));
}

ModelLayer::ModelLayer(SharedBase const & base) TBAG_NOEXCEPT : _base(base)
{
    assert(static_cast<bool>(_base));
}

ModelLayer::ModelLayer(ModelLayer const & obj) TBAG_NOEXCEPT : ModelLayer(nullptr)
{
    (*this) = obj;
}

ModelLayer::ModelLayer(ModelLayer && obj) TBAG_NOEXCEPT : ModelLayer(nullptr)
{
    (*this) = std::move(obj);
}

ModelLayer::~ModelLayer()
{
    // EMPTY.
}

ModelLayer & ModelLayer::operator =(ModelLayer const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

ModelLayer & ModelLayer::operator =(ModelLayer && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void ModelLayer::copy(ModelLayer const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _base = obj._base;
    }
}

void ModelLayer::swap(ModelLayer & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _base.swap(obj._base);
    }
}

std::string ModelLayer::toString() const
{
    return std::string();
}

} // namespace graph

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

