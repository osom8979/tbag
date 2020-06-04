/**
 * @file   ModelLayer.cpp
 * @brief  ModelLayer class implementation.
 * @author zer0
 * @date   2018-10-22
 */

#include <libtbag/graph/ModelLayer.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace graph {

ModelLayer::ModelLayer() : ModelLayer(std::make_shared<LayerBase>())
{
    assert(static_cast<bool>(_base));
}

ModelLayer::ModelLayer(std::nullptr_t) TBAG_NOEXCEPT
        : _base(nullptr)
{
    assert(!static_cast<bool>(_base));
}

ModelLayer::ModelLayer(SharedBase base) TBAG_NOEXCEPT
        : _base(std::move(base))
{
    assert(static_cast<bool>(_base));
}

ModelLayer::ModelLayer(ModelLayer const & obj) TBAG_NOEXCEPT
        : _base(obj._base)
{
// EMPTY.
}

ModelLayer::ModelLayer(ModelLayer && obj) TBAG_NOEXCEPT
        : _base(std::move(obj._base))
{
    // EMPTY.
}

ModelLayer::~ModelLayer()
{
    // EMPTY.
}

ModelLayer & ModelLayer::operator =(ModelLayer const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _base = obj._base;
    }
    return *this;
}

ModelLayer & ModelLayer::operator =(ModelLayer && obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _base = std::move(obj._base);
    }
    return *this;
}

void ModelLayer::swap(ModelLayer & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _base.swap(obj._base);
    }
}

void ModelLayer::_assign_id(int id)
{
    assert(exists());
    _base->_assign_id = id;
}

int ModelLayer::id() const
{
    assert(exists());
    return _base->_assign_id;
}

Err ModelLayer::runner(RunnerInfo & info)
{
    assert(exists());
    return _base->runner(info);
}

} // namespace graph

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

